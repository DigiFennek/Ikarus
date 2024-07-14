//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Main
//*
//***************************************************************************

#include <stdlib.h>
#include <dos.h>
#include "ikarus2.h"
#include "geometry.h"

struct MOBILE viewPoint[VIEWPOINTS];
struct MOBILE *currentViewPoint = NULL;

struct MOBILE mobileMove[MOBILES];
struct MOBILE mobileCopy[MOBILES];

struct MOBILE wind;
struct MOBILE observer;

int error = NO_ERROR;
int firstFrame = TRUE;

unsigned char *mainPalette = NULL;

extern int quit;
extern int viewMode;
extern int complexity;
extern int winds;
extern int clouds;
extern int easyControl;
extern int newView;
extern int dynamicScenery;
extern int linkPort;
extern int soundType;
extern int mobileCount;
extern int objects;
extern int viewPoints;
extern int autoLanding;
extern int autoPilot;

volatile int sync = FALSE;
volatile int timer = 0;

volatile int timerActive = FALSE;
volatile int flashStep = 0;

int mouseX, mouseY;

long polygonCounter = 0;
long timerTickCounter = 0;
long frameCounter = 0;

void checkMouse (void)
{
  int x, y, key;

  if (mobileMove[0].position.y <= 0)
    setMousePos (320, 100);

  getMousePos (&x, &y, &key);

  if (!autoLanding && !autoPilot)
  {
    mouseX = -(y - 100);
    mouseY = -(x - 320);

    if ((key & 0x07) == 0x01) mobileMove[0].steeringPower -= 1;
    if ((key & 0x07) == 0x02) mobileMove[0].steeringPower += 1;
  }

  if ((key & 0x07) == 0x03) menu (NULL);
}


int adjustAngle (int angle)
{
  if (angle >= 3600)
    return angle - 3600;

  if (angle < 0)
    return angle + 3600;

  return angle;
}


int getAngleDiff (int angle1, int angle2)
{
  int diff = (adjustAngle (angle2) - adjustAngle (angle1));

  if (diff <= -1800)
    return (diff + 3600);

  if (diff >= 1800)
    return (diff - 3600);

  return diff;
}


int followAngle (int *angle, int newAngle, int flow)
{
  int diff = getAngleDiff (*angle, newAngle);

  flow = max (1, flow);

  if (diff > 0)
    *angle = adjustAngle ((diff / flow) + 1 + *angle);

  if (diff < 0)
    *angle = adjustAngle ((diff / flow) - 1 + *angle);

  return *angle;
}


int minmax (int v, int min, int max)
{
  if (v < min) return min;
  if (v > max) return max;
  return v;
}


void adjustArea (struct MOBILE *plane)
{
  if (plane->position.z > (AREAWIDE / 2))
  {
    plane->position.z -= AREAWIDE;
    plane->areaPos.z++;
  }

  if (plane->position.z < (-AREAWIDE / 2))
  {
    plane->position.z += AREAWIDE;
    plane->areaPos.z--;
  }

  if (plane->position.x > (AREAWIDE / 2))
  {
    plane->position.x -= AREAWIDE;
    plane->areaPos.x++;
  }

  if (plane->position.x < (-AREAWIDE / 2))
  {
    plane->position.x += AREAWIDE;
    plane->areaPos.x--;
  }
}


void checkWind (void)
{
  int maxSpeed = 2;

  switch (winds)
  {
    case STRONGWINDS : maxSpeed = 30; break;
    case LIGHTWINDS  : maxSpeed = 10; break;
  }

  if (!random(1000))
    wind.moveAngle.y = random (3600);

  if (!random(100))
    wind.power = random (maxSpeed);

  followAngle (&wind.angle.y, wind.moveAngle.y, 30);

  if (wind.xzSpeed < wind.power) wind.xzSpeed++;
  if (wind.xzSpeed > wind.power) wind.xzSpeed--;
}


int searchViewPoint (struct MOBILE *target, int next)
{
  int i, areax = AREAVIEWWIDE, areaz = AREAVIEWWIDE;
  struct MOBILE *lastViewPoint = currentViewPoint;

  if ((abs (target->areaPos.x - currentViewPoint->areaPos.x) > areax) ||
      (abs (target->areaPos.z - currentViewPoint->areaPos.z) > areaz) || next)
  {
    for (i = 0; i < viewPoints; i++)
    {
      if ((abs (target->areaPos.x - viewPoint[i].areaPos.x) <= areax) &&
	  (abs (target->areaPos.z - viewPoint[i].areaPos.z) <= areaz) &&
	  (currentViewPoint != &viewPoint[i]))
      {
	areax = abs (target->areaPos.x - viewPoint[i].areaPos.x);
	areaz = abs (target->areaPos.z - viewPoint[i].areaPos.z);
	currentViewPoint = &viewPoint[i];
      }
    }
  }

  if (currentViewPoint == NULL)
    currentViewPoint = &viewPoint[0];

  return (currentViewPoint != lastViewPoint);
}


int targetDirection (int *alpha, int *beta, struct MOBILE *mobile)
{
  long xDist, yDist, zDist, xzDist, yzDist;

  if ((abs (mobile->targetArea.x - mobile->areaPos.x) <= AREAVIEWWIDE) &&
      (abs (mobile->targetArea.z - mobile->areaPos.z) <= AREAVIEWWIDE))
  {
    xDist = mobile->targetPos.x - mobile->position.x;
    yDist = mobile->targetPos.y - mobile->position.y;
    zDist = mobile->targetPos.z - mobile->position.z;
    xDist += AREAWIDE * (mobile->targetArea.x - mobile->areaPos.x);
    zDist += AREAWIDE * (mobile->targetArea.z - mobile->areaPos.z);
  }
  else
  {
    xDist = mobile->targetArea.x - mobile->areaPos.x;
    yDist = mobile->targetArea.y - mobile->areaPos.y;
    zDist = mobile->targetArea.z - mobile->areaPos.z;
  }

  xzDist = intRoot (xDist * xDist + zDist * zDist);
  yzDist = intRoot (yDist * yDist + xzDist * xzDist);

  *alpha = intArcCos ((zDist << 15) / (xzDist + 1));
  *beta  = intArcSin ((yDist << 15) / (yzDist + 1));

  if (xDist > 0) *alpha = -*alpha;

  return xzDist;
}


void pursueView (struct MOBILE *source, struct MOBILE *target, int initView)
{
  int alpha, beta;

  observer.position   = source->position;
  observer.areaPos    = source->areaPos;
  observer.targetPos  = target->position;
  observer.targetArea = target->areaPos;
  observer.number     = source->number;
  observer.angle.z    = 0;

  if ((abs (target->areaPos.x - source->areaPos.x) <= AREAVIEWWIDE) &&
      (abs (target->areaPos.z - source->areaPos.z) <= AREAVIEWWIDE))
  {
    targetDirection (&alpha, &beta, &observer);

    if (initView)
    {
      observer.angle.x = -beta;
      observer.angle.y = -alpha;
    }

    followAngle (&observer.angle.x, -beta, 5);
    followAngle (&observer.angle.y, -alpha, 5);
  }

  else
  {
    if (initView)
      observer.angle.x = 0;
    else
      followAngle (&observer.angle.x, 0, 20);
  }

  adjustArea (&observer);
}


void cockpitView (struct MOBILE *plane, int dir)
{
  observer.position.x = 0;
  observer.position.y = 160;
  observer.position.z = 60;

  setPosition (plane->position.x, plane->position.y, plane->position.z);
  setAngle (plane->angle.x, plane->angle.y, plane->angle.z);
  setOrder ("xzyt");
  transform (1, &observer.position);

  observer.areaPos = plane->areaPos;
  observer.number = plane->number;

  switch (dir)
  {
    case FRONTVIEW :
      observer.angle.x = plane->angle.x;
      observer.angle.y = plane->angle.y;
      observer.angle.z = plane->angle.z; break;
    case LEFTVIEW :
      observer.angle.x = plane->angle.z;
      observer.angle.y = adjustAngle (plane->angle.y - 900);
      observer.angle.z = -plane->angle.x; break;
    case RIGHTVIEW :
      observer.angle.x = -plane->angle.z;
      observer.angle.y = adjustAngle (plane->angle.y + 900);
      observer.angle.z = plane->angle.x; break;
    case BACKVIEW :
      observer.angle.x = -plane->angle.x;
      observer.angle.y = adjustAngle (plane->angle.y + 1800);
      observer.angle.z = -plane->angle.z; break;
  }

  adjustArea (&observer);
}


void spotView (struct MOBILE *plane, int initView)
{
  int alpha, beta;

  observer.position.x = 400;
  observer.position.y = 300;
  observer.position.z = 400;

  setPosition (plane->position.x, plane->position.y, plane->position.z);
  setAngle (0, plane->angle.y, 0);
  setOrder ("yt");
  transform (1, &observer.position);

  observer.areaPos = plane->areaPos;
  observer.targetPos = plane->position;
  observer.targetArea = plane->areaPos;
  observer.number = -1;
  observer.angle.z = 0;

  targetDirection (&alpha, &beta, &observer);

  if (initView)
  {
    observer.angle.x = -beta;
    observer.angle.y = -alpha;
  }

  followAngle (&observer.angle.x, -beta, 10);
  followAngle (&observer.angle.y, -alpha, 10);

  adjustArea (&observer);
}


void checkView (void)
{
  switch (viewMode)
  {
    case TOWERVIEW :
      newView = searchViewPoint (&mobileCopy[0], newView);
      pursueView (currentViewPoint, &mobileCopy[0], newView);
      break;
    case SPOTVIEW :
      spotView (&mobileCopy[0], newView);
      currentViewPoint = NULL;
      break;
    default :
      cockpitView (&mobileCopy[0], viewMode);
      currentViewPoint = NULL;
  }
}


void checkFlashLight (void)
{
  if (flashStep <= 7)
    setPalette (flashStep + FLASH, 0, 0, 0, TRUE);

  flashStep = (flashStep >= 15) ? 0 : flashStep + 1;

  if (flashStep <= 7)
    setPalette (flashStep + FLASH, 255, 255, 255, TRUE);
}


void resetMobile (struct MOBILE *mobile)
{
  mobile->power = mobile->steeringPower = 0;
  mobile->steeringDirX = 0;
  mobile->steeringDirY = 0;
  mobile->angle = mobile->homeAngle;
  mobile->moveAngle = mobile->homeAngle;
  mobile->position = mobile->homePos;
  mobile->areaPos = mobile->homeArea;
  mobile->targetPos = mobile->homePos;
  mobile->targetArea = mobile->homeArea;
  mobile->landingHight = 0;
  mobile->contactHight = 0;
  mobile->contactCount = 0;
  mobile->state = PARKING;
  mobile->fuel = 30000;
  mobile->collision = FALSE;
  mobile->xzSpeed = 0;
  mobile->ySpeed = 0;
}


void interrupt (*oldTimerHandler) (void);

void interrupt timerHandler (void)
{
  if (timerActive)
  {
    int n;

    mobileMove[0].steeringDirX = mouseX;
    mobileMove[0].steeringDirY = mouseY;

    if (linkPort == COM_NONE)
      for (n = 0; n < mobileCount; n++)
	simulateChopper (&mobileMove[n]);
    else
      simulateChopper (&mobileMove[0]);

    checkFlashLight ();

    timerTickCounter++;
  }

  if (timer)
    timer--;

  sync = TRUE;

  oldTimerHandler ();
}


void timeoutWait (int seconds)
{
  int x, y, mouseKey = 0;
  timer = seconds * 11;
  while (timer && !kbhit() && !mouseKey)
    getMousePos (&x, &y, &mouseKey);
  if (kbhit()) getch ();
}


void checkAutoPilot (void)
{
  // Landing
  if (autoLanding)
    mobileMove[0].state = LANDING;

  // Autopilot
  else if (autoPilot)
  {
    if (mobileMove[0].state == IDLE)
      mobileMove[0].state = FLYING;
  }

  else
    mobileMove[0].state = IDLE;

  autoPilotChopper (&mobileMove[0]);

  if (mobileMove[0].state == PARKING)
    autoLanding = FALSE;
}


void checkDynamicObjects (void)
{
  int i;

  if (linkPort == COM_NONE)
  {
    if (dynamicScenery)
    {
      for (i = 1; i < CHOPPERS; i++)
      {
	mobileCount = objects;
	autoPilotChopper (&mobileMove[i]);
      }
    }
    else
      mobileCount = 1;
  }
}


void checkObjectMove (void)
{
  int i;

  for (i = 0; i < mobileCount; i++)
    mobileCopy[i] = mobileMove[i];

  // Kollision zuruecksetzen
  if ((mobileCopy[0].collision) && !mobileCopy[0].timer)
  {
    fadePalette (-100);
    resetMobile (&mobileMove[0]);
    resetMobile (&mobileCopy[0]);
    firstFrame = TRUE;
  }
}


int main (int argc, char **argv)
{
  oldTimerHandler = getvect(0x1C);
  setvect (0x1C, timerHandler);

  if (!initVGA ())
    error = GRAPHICS_ERROR;

  if (!initMouse ())
    error = MOUSE_ERROR;

  if (!error)
  {
    initClouds (2000);

    fadePalette (-1);

    if (argc > 1)
      error = initDatabase (argv[1]);
    else
      error = initDatabase ("ikarus.scn");
  }

  if (!error)
  {
    initCockpit ();
    initSound (ADLIB_SOUND);
    setMousePos (160, 100);

    wind.xzSpeed = 0;
    wind.moveAngle.y = 0;
    wind.power = 0;

    mobileMove[0].steeringPower = mobileMove[0].power = 700;
  }

  while (!quit && !error)
  {
    while (!sync);
    sync = FALSE;

    checkWind ();
    checkAutoPilot ();
    checkDynamicObjects ();
    checkConnection ();
    checkObjectMove ();
    checkView ();
    checkSound (&mobileCopy[0]);
    checkSound (&mobileCopy[1]);
    checkSound (&mobileCopy[2]);

    displayWorld ();

    if (viewMode == FRONTVIEW)
      displayCockpit (&mobileCopy[0]);

    displayStatus (&mobileCopy[0]);

    flipScreen ();

    frameCounter++;

    if (firstFrame)
    {
      initColorPalette (mainPalette, 0, 255);
      fadePalette (30);
      timerActive = TRUE;
      firstFrame = FALSE;
      initSound (soundType);
    }

    newView = FALSE;

    checkMouse ();
    checkKeyboard ();
  }

  com_exit ();

  setvect (0x1C, oldTimerHandler);

  if (!error && !firstFrame)
    fadePalette (-30);

  muteSound ();
  setMousePointer (FALSE);
  initText ();

  if (!error && frameCounter && (timerTickCounter / 18))
  {
    printf ("frames/second  : %li\n", frameCounter/(timerTickCounter/18));
    printf ("polygons/frame : %li\n\n", polygonCounter/frameCounter);
  }

  switch (error)
  {
    case NO_ERROR	    : printf ("hope you enjoyed it - STN\n"); break;
    case GRAPHICS_ERROR     : printf ("graphics error\n"); break;
    case DB_FORMAT_ERROR    : printf ("database format error\n"); break;
    case DB_NOT_FOUND_ERROR : printf ("database not found\n"); break;
    case MEMORY_ERROR	    : printf ("not enough memory\n"); break;
    case MOUSE_ERROR	    : printf ("mouse was eaten by the cat\n"); break;
  }


  return (error);
}
