//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Durchqueren des Database
//*  - Objekthandling
//*
//***************************************************************************

#include <values.h>
#include <dos.h>
#include <mem.h>
#include "ikarus2.h"
#include "geometry.h"

extern struct MOBILE mobileMove[];
extern struct MOBILE mobileCopy[];
extern struct MOBILE observer;

extern char far *texturePtr[];

extern int complexity;
extern int clouds;
extern int mobileCount;
extern int groundDots;
extern long polygonCounter;

extern int **(*objectMap)[AREAMAPSIZE];

int groundColor;

extern void mapPoly (struct POINT *, char far *);

struct POINT viewPos, viewAngle;

void displayObject (int *object, long dist)
{
  int lastPolyVisible = TRUE;

  setPosition (-viewPos.x, -viewPos.y, -viewPos.z);
  setAngle (-viewAngle.x, -viewAngle.y, -viewAngle.z);
  setOrder ("tyxz");

  while (*object)
  {
    int  polySize = *object++;
    int  polyType = *object++;
    int  check    = *object++;
    long minDist  = *object++;
    int  color	  = *object++;
    int  misc	  = *object++;
    int *nextPtr  = object + 3 * polySize;
    long polyDist = minDist * minDist;

    struct POINT poly1[20];
    struct POINT poly2[20];

    if (
	 (check != MIDPOINT)
	 &&
	 (check != CONTACT)
	 &&
	 (
	   ((minDist >= 0) && (dist <= polyDist)) ||
	   ((minDist <  0) && (dist >  polyDist))
	 )
	 &&
	 (
	   (check == NOCHECK)
	   ||
	   (
	     (check == SURFACE) && (lastPolyVisible == TRUE)
	   )
	   ||
	   (
	     (check == CHECK) && testVisible ((struct POINT *)object)
	   )
	 )
       )
    {
      polygonCounter++;

      lastPolyVisible = TRUE;

      memcpy (poly1, object, polySize * sizeof (struct POINT));

      if (polyType == PIXEL)
      {
	transform (polySize, poly1);

	while (polySize--)
	{
	  struct POINT *p = &poly1[polySize];

	  if (p->z > 0)
	  {
	    perspective (1, p);
	    putPixel (p->x, p->y, color);
	  }
	}
      }

      else if (polyType == TEXTURE)
      {
	transform (polySize, poly1);
	if ((poly1[0].z > 0) && (poly1[1].z > 0) &&
	    (poly1[2].z > 0) && (poly1[3].z > 0))
	{
	  perspective (polySize, poly1);
	  mapPoly (poly1, texturePtr[misc]);
	}
      }

      else if (polyType == CIRCLE)
      {
	transform (polySize, poly1);

	while (polySize--)
	{
	  struct POINT *p = &poly1[polySize];

	  if (p->z > 0)
	  {
	    poly2[0].x = p->x;
	    poly2[0].y = p->y;
	    poly2[0].z = p->z;
	    poly2[1].x = misc;
	    poly2[1].y = p->y;
	    poly2[1].z = p->z;
	    perspective (2, poly2);
	    fillCircle (poly2[0].x, poly2[0].y, poly2[1].x - 160, color);
	  }
	}
      }

      else if (polySize > 0)
      {
	transform (polySize, poly1);

	if ((polySize = clipFront (polySize, poly1, poly2)) >= 2)
	{
	  perspective (polySize, &poly2[0]);

	  if (((polySize = clipLeft   (polySize, poly2, poly1)) >= 2) &&
	      ((polySize = clipRight  (polySize, poly1, poly2)) >= 2) &&
	      ((polySize = clipTop    (polySize, poly2, poly1)) >= 2) &&
	      ((polySize = clipBottom (polySize, poly1, poly2)) >= 2))
	  {
	    if (polyType == POLYGON)
	      fillPoly (polySize, &poly2, color, misc);
	    else
	      polyLine (polySize, &poly2, color);
	  }
	}
      }
    }
    else
      lastPolyVisible = FALSE;

    object = nextPtr;
  }
}


void transformObject (int *objectIn, int *objectOut)
{
  while (*objectIn)
  {
    int polySize = *objectIn++;
    *objectOut++ = polySize;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;

    memcpy (objectOut, objectIn, polySize * sizeof (struct POINT));

    transform (polySize, (struct POINT *)objectOut);

    objectIn += (3 * polySize);
    objectOut += (3 * polySize);
  }

  *objectOut = 0;
}

#define OBJECTS 100

int *objectList[OBJECTS];
int  objectCount;

void addToObjectList (int *object)
{
  if (object[1] <= complexity)
  {
    if (object[2] == MIDPOINT)
    {
      if (objectCount < OBJECTS)
      {
	objectList[objectCount] = object;
	objectCount++;
      }
    }

    else if (object[2] == GROUND)
    {
      long objectDist = longScalarProd (&viewPos,
	(struct POINT *) &object[6]);

      displayObject (object, objectDist);
    }

    else if (object[2] == WINDBAG)
    {
      displayWindbag ((struct POINT *) &object[6]);
    }
  }
}


void resetObjectList (void)
{
  objectCount = 0;
}


int groundDot[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  1, PIXEL, NOCHECK, 12000, GREY, 0,
	   0,	   0,	   0,
  0
};


void displayObjectList ()
{
  long objectDist[OBJECTS], dist;
  int i, k;

  if (objectCount)
  {
    for (i = 0; i < objectCount; i++)
    {
      objectDist[i] = longScalarProd (&viewPos,
	(struct POINT *) &objectList[i][6]);
    }

    for (i = 0; i < objectCount; i++)
      for (k = 0; k < objectCount - 1; k++)
	if (objectDist[k] < objectDist[k + 1])
	{
	  int *objectTemp = objectList[k];
	  long distTemp = objectDist[k];
	  objectList[k] = objectList[k + 1];
	  objectDist[k] = objectDist[k + 1];
	  objectList[k + 1] = objectTemp;
	  objectDist[k + 1] = distTemp;
	}

    for (i = 0; i < objectCount; i++)
      displayObject (objectList[i], objectDist[i]);
  }
}


void checkCollision (struct MOBILE *mobile)
{
  int i;

  mobile->contactHight = 0;
  mobile->contactCount = 0;
  mobile->contactDist2 = (long)MAXDIST * MAXDIST;

  for (i = 0; i < objectCount; i++)
  {
    int *object = &objectList[i][9];

    if (object[2] == CONTACT)
    {
      int count = object[0];
      long radius = (long)object[3] * (long)object[3];
      object += 6;

      while (count-- )
      {
	long dist = longScalarProd (&mobile->position,
	  (struct POINT *)object);

	if (dist < radius)
	{
	  if (mobile->contactDist2 > dist)
	  {
	    mobile->contactHight = *(object + 1);
	    mobile->contactDist2 = dist;
	  }

	  mobile->contactCount++;
	}

	object += 3;
      }
    }
  }
}


void displayAreaXY (int x, int z)
{
  int areax = observer.areaPos.x + x;
  int areaz = observer.areaPos.z + z;
  int n;

  viewPos = observer.position;
  viewPos.x -= x * AREAWIDE;
  viewPos.z -= z * AREAWIDE;
  viewAngle = observer.angle;

  resetObjectList ();

  if ((areax >= 0) && (areax < AREAMAPSIZE) &&
      (areaz >= 0) && (areaz < AREAMAPSIZE))
  {
    int **objectPtrPtr;

    if ((objectPtrPtr = objectMap[areax][areaz]) != NULL)
      while (*objectPtrPtr != NULL)
	addToObjectList (*objectPtrPtr++);
  }

  for (n = 1; n < mobileCount; n++)
  {
    if ((mobileCopy[n].areaPos.x == areax) &&
	(mobileCopy[n].areaPos.z == areaz))
      displayChopper (&mobileCopy[n]);
  }

  if ((mobileCopy[0].areaPos.x == areax) &&
      (mobileCopy[0].areaPos.z == areaz))
  {
    checkCollision (&mobileCopy[0]);

    mobileMove[0].contactHight = mobileCopy[0].contactHight;
    mobileMove[0].contactCount = mobileCopy[0].contactCount;
    mobileMove[0].contactDist2 = mobileCopy[0].contactDist2;

    if (mobileCopy[0].number != observer.number)
      displayChopper (&mobileCopy[0]);
  }

  if (clouds)
    displayClouds (areax, areaz);

  if (groundDots)
    addToObjectList (groundDot);

  displayObjectList ();
}



void displayAreas (void)
{
  int i, j, k;
  int direction = ((3590 - observer.angle.y + 220) / 450) % 8;

  switch (direction)
  {
    case 0 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  | * | * | * | * | * |  */
      { 				   /*	    ---------------------  */
	for (k = AREAVIEWWIDE; k > 0; k--) /*	 1  | * | * | * | * | * |  */
	{				   /*	    ---------------------  */
	  displayAreaXY ( k, i);	   /*	 0  | * | * | * | * | * |  */
	  displayAreaXY (-k, i);	   /*	    ---------------------  */
	}				   /*	-1  |	|   |	|   |	|  */
	displayAreaXY ( k, i);		   /* ^     ---------------------  */
      } 				   /* | -2  |	|   |	|   |	|  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 1 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  | * | * | * | * | * |  */
      { 				   /*	    ---------------------  */
	displayAreaXY (-i, i);		   /*	 1  | * | * | * | * |	|  */
	for (k = i; k > -i; k--)	   /*	    ---------------------  */
	{				   /*	 0  | * | * | * |   |	|  */
	  displayAreaXY (-k + 1, i);	   /*	    ---------------------  */
	  displayAreaXY (-i, k - 1);	   /*	-1  | * | * |	|   |	|  */
	}				   /* ^     ---------------------  */
      } 				   /* | -2  | * |   |	|   |	|  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 2 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  | * | * | * |   |	|  */
      { 				   /*	    ---------------------  */
	for (k = AREAVIEWWIDE; k > 0; k--) /*	 1  | * | * | * |   |	|  */
	{				   /*	    ---------------------  */
	  displayAreaXY (-i, k);	   /*	 0  | * | * | * |   |	|  */
	  displayAreaXY (-i,-k);	   /*	    ---------------------  */
	}				   /*	-1  | * | * | * |   |	|  */
	displayAreaXY (-i,-k);		   /* ^     ---------------------  */
      } 				   /* | -2  | * | * | * |   |	|  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 3 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  | * |   |	|   |	|  */
      { 				   /*	    ---------------------  */
	displayAreaXY (-i,-i);		   /*	 1  | * | * |	|   |	|  */
	for (k = i; k > -i; k--)	   /*	    ---------------------  */
	{				   /*	 0  | * | * | * |   |	|  */
	  displayAreaXY (-k + 1,-i);	   /*	    ---------------------  */
	  displayAreaXY (-i,-k + 1);	   /*	-1  | * | * | * | * |	|  */
	}				   /* ^     ---------------------  */
      } 				   /* | -2  | * | * | * | * | * |  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 4 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  |	|   |	|   |	|  */
      { 				   /*	    ---------------------  */
	for (k = AREAVIEWWIDE; k > 0; k--) /*	 1  |	|   |	|   |	|  */
	{				   /*	    ---------------------  */
	  displayAreaXY ( k,-i);	   /*	 0  | * | * | * | * | * |  */
	  displayAreaXY (-k,-i);	   /*	    ---------------------  */
	}				   /*	-1  | * | * | * | * | * |  */
	displayAreaXY ( k,-i);		   /* ^     ---------------------  */
      } 				   /* | -2  | * | * | * | * | * |  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 5 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  |	|   |	|   | * |  */
      { 				   /*	    ---------------------  */
	displayAreaXY ( i, -i); 	   /*	 1  |	|   |	| * | * |  */
	for (k = i; k > -i; k--)	   /*	    ---------------------  */
	{				   /*	 0  |	|   | * | * | * |  */
	  displayAreaXY ( k - 1,-i);	   /*	    ---------------------  */
	  displayAreaXY ( i,-k + 1);	   /*	-1  |	| * | * | * | * |  */
	}				   /* ^     ---------------------  */
      } 				   /* | -2  | * | * | * | * | * |  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 6 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  |	|   | * | * | * |  */
      { 				   /*	    ---------------------  */
	for (k = AREAVIEWWIDE; k > 0; k--) /*	 1  |	|   | * | * | * |  */
	{				   /*	    ---------------------  */
	  displayAreaXY ( i, k);	   /*	 0  |	|   | * | * | * |  */
	  displayAreaXY ( i,-k);	   /*	    ---------------------  */
	}				   /*	-1  |	|   | * | * | * |  */
	displayAreaXY ( i, k);		   /* ^     ---------------------  */
      } 				   /* | -2  |	|   | * | * | * |  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */

    case 7 :				   /*	    ---------------------  */
      for (i = AREAVIEWWIDE; i >= 0; i--)  /*	 2  | * | * | * | * | * |  */
      { 				   /*	    ---------------------  */
	displayAreaXY ( i, i);		   /*	 1  |	| * | * | * | * |  */
	for (k = i; k > -i; k--)	   /*	    ---------------------  */
	{				   /*	 0  |	|   | * | * | * |  */
	  displayAreaXY ( k - 1, i);	   /*	    ---------------------  */
	  displayAreaXY ( i, k - 1);	   /*	-1  |	|   |	| * | * |  */
	}				   /* ^     ---------------------  */
      } 				   /* | -2  |	|   |	|   | * |  */
					   /* y     ---------------------  */
      break;				   /*  x->   -2  -1   0   1   2    */
  }
}


int ground[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  4, POLYGON, NOCHECK, MAXDIST, 0, 0,
     -16000, -200, -12000,
     -16000, -200,  12000,
      16000, -200,  12000,
      16000, -200, -12000,
  0
};


int sky[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	   0,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 0, SKYBLUE + 0,
     -16000, 1300, -12000,
     -16000, 1300,  12000,
      16000, 1300,  12000,
      16000, 1300, -12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 1, SKYBLUE + 1,
     -16000, 1200,  12000,
     -16000, 1300,  12000,
      16000, 1300,  12000,
      16000, 1200,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 2, SKYBLUE + 2,
     -16000, 1100,  12000,
     -16000, 1200,  12000,
      16000, 1200,  12000,
      16000, 1100,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 3, SKYBLUE + 3,
     -16000, 1000,  12000,
     -16000, 1100,  12000,
      16000, 1100,  12000,
      16000, 1000,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 4, SKYBLUE + 4,
     -16000,  900,  12000,
     -16000, 1000,  12000,
      16000, 1000,  12000,
      16000,  900,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 5, SKYBLUE + 5,
     -16000,  800,  12000,
     -16000,  900,  12000,
      16000,  900,  12000,
      16000,  800,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 6, SKYBLUE + 6,
     -16000,  700,  12000,
     -16000,  800,  12000,
      16000,  800,  12000,
      16000,  700,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 7, SKYBLUE + 7,
     -16000,  600,  12000,
     -16000,  700,  12000,
      16000,  700,  12000,
      16000,  600,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 8, SKYBLUE + 8,
     -16000,  500,  12000,
     -16000,  600,  12000,
      16000,  600,  12000,
      16000,  500,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 9, SKYBLUE + 9,
     -16000,  400,  12000,
     -16000,  500,  12000,
      16000,  500,  12000,
      16000,  400,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 10, SKYBLUE + 10,
     -16000,  300,  12000,
     -16000,  400,  12000,
      16000,  400,  12000,
      16000,  300,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 11, SKYBLUE + 11,
     -16000,  200,  12000,
     -16000,  300,  12000,
      16000,  300,  12000,
      16000,  200,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 12, SKYBLUE + 12,
     -16000,  100,  12000,
     -16000,  200,  12000,
      16000,  200,  12000,
      16000,  100,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 13, SKYBLUE + 13,
     -16000,    0,  12000,
     -16000,  100,  12000,
      16000,  100,  12000,
      16000,    0,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 14, SKYBLUE + 14,
     -16000, -100,  12000,
     -16000,    0,  12000,
      16000,    0,  12000,
      16000, -100,  12000,
  4, POLYGON, NOCHECK, MAXDIST, SKYBLUE + 15, SKYBLUE + 15,
     -16000, -200,  12000,
     -16000, -100,  12000,
      16000, -100,  12000,
      16000, -200,  12000,
  0
};

int sun[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	   0,
  1, CIRCLE, NOCHECK, MAXDIST, YELLOW + 15, 2000,
       6000, 8000, 8000,
  0,
};

void displayWorld (void)
{
  viewPos.x = 0; viewPos.y = 0; viewPos.z = 0;
  viewAngle = observer.angle; viewAngle.y = 0;

  ground[13] = ground[14] = groundColor;

  displayObject (sky, 0);
  displayObject (ground, 0);

  viewAngle = observer.angle;

  displayObject (sun, 0);

  displayAreas ();

  if (clouds)
    displayCloudFog ();
}



