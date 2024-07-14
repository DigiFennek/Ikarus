//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Setup Menu
//*
//***************************************************************************

#include <stdlib.h>
#include <dos.h>
#include "ikarus2.h"

int quit = FALSE;
int newView = FALSE;
int viewMode = FRONTVIEW;
int complexity = VERYCOMPLEX;
int cockpitOn = TRUE;
int winds = NOWINDS;
int easyControl = FALSE;
int dynamicScenery = TRUE;
int groundDots = TRUE;
int clouds = TRUE;
int autoPilot = FALSE;
int autoLanding = FALSE;
int collision = TRUE;

extern int firstFrame;
extern int timerActive;
extern int mouseFlag;
extern int linkPort;
extern int soundType;

unsigned char *menuPicture = NULL;
unsigned char *menuPalette = NULL;

enum
{
  M_NONE, M_TITLE, M_TEXT, M_LISTEND,
  M_VERYCOMPLEX, M_COMPLEX, M_SPARSE, M_VERYSPARSE, M_DYNAMIC,
  M_EXITMENU, M_EXITDOS, M_MAINMENU, M_HELP, M_ABOUT, M_MORE1,
  M_COCKPIT, M_GROUNDDOTS, M_CLOUDS,
  M_FRONTVIEW, M_LEFTVIEW, M_RIGHTVIEW, M_BACKVIEW, M_TOWERVIEW, M_SPOTVIEW,
  M_NOWINDS, M_STRONGWINDS, M_LIGHTWINDS,
  M_AUTOPILOT, M_EASYCONTROL, M_LANDME, M_COLLISION,
  M_CONNECTCOM1, M_CONNECTCOM2, M_CONNECTOFF,
  M_ADLIBSOUND,
};

struct MENU
{
  int x; int y;
  int key;
  char *string;
  int opcode;
};


struct MENU mainMenu[] =
{
  100,	10,  0,    "SCENERY",              M_TITLE,
  100,	22,  404,  "VERY COMPLEX",         M_VERYCOMPLEX,
  100,	34,  405,  "COMPLEX",              M_COMPLEX,
  100,	46,  406,  "SPARSE",               M_SPARSE,
  100,	58,  407,  "VERY SPARSE",          M_VERYSPARSE,
  100,	70,  408,  "DYNAMIC OBJECTS",      M_DYNAMIC,
   10,	10,  0,    "VIEW",                 M_TITLE,
   10,	22,  372,  "COCKPIT",              M_FRONTVIEW,
   10,	34,  375,  "LEFT",                 M_LEFTVIEW,
   10,	46,  377,  "RIGHT",                M_RIGHTVIEW,
   10,	58,  380,  "BACK",                 M_BACKVIEW,
   10,	70,  'T',  "TOWER",                M_TOWERVIEW,
   10,	82,  'S',  "SPOT",                 M_SPOTVIEW,
  110,  94,  0,    "SIMULATION",           M_TITLE,
  110, 106,  0,    "EASY CONTROL",         M_EASYCONTROL,
  110, 118,  'A',  "AUTOPILOT",            M_AUTOPILOT,
  110, 130,  'L',  "LAND ME",              M_LANDME,
  110, 142,  0,    "COLLISION",            M_COLLISION,
   10, 106,  0,    "WINDS",                M_TITLE,
   10, 118,  0,    "STILL",                M_NOWINDS,
   10, 130,  0,    "LIGHT",                M_LIGHTWINDS,
   10, 142,  0,    "STRONG",               M_STRONGWINDS,
  240,	10,  0,    "DISPLAY",              M_TITLE,
  240,	22,  9,    "COCKPIT",              M_COCKPIT,
  240,	34,  409,  "DOTS",                 M_GROUNDDOTS,
  240,	46,  410,  "CLOUDS",               M_CLOUDS,
  240, 106,  0,    "CONNECT",              M_TITLE,
  240, 118,  394,  "COM 1",                M_CONNECTCOM1,
  240, 130,  395,  "COM 2",                M_CONNECTCOM2,
  240, 142,  396,  "OFF",                  M_CONNECTOFF,
  240,  70,  0,    "SOUND",                M_TITLE,
  240,  82,  403,  "AdLib",                M_ADLIBSOUND,
  230, 180,  27,   "EXIT MENU",            M_EXITMENU,
   10, 180,  345,  "EXIT TO DOS",          M_EXITDOS,
  135, 180,  359,  "HELP",                 M_HELP,
    0,	 0,  0,    "",                     M_LISTEND
};


struct MENU helpMenu[] =
{
   10,	10,  0,    "HELP",                 M_TITLE,
   10,	34,  0,    "CONTROL OF THE HELICOPTER:",            	     M_TEXT,
   10,	46,  0,    " - USE THE MOUSE FOR CONTROL OF THE DIRECTION",  M_TEXT,
   10,	58,  0,    " - THE LEFT MOUSE BUTTON LETS POWER GO DOWN",    M_TEXT,
   10,	70,  0,    " - THE RIGHT MOUSE BUTTON LETS POWER GO UP",     M_TEXT,
   10,	94,  0,    "FOR MENU PRESS BOTH MOUSE KEYS AT THE SAME TIME",M_TEXT,
  230, 180,  27,   "EXIT HELP",            M_EXITMENU,
  135, 180,  359,  "MORE",                 M_MORE1,
   10, 180,  345,  "EXIT TO DOS",          M_EXITDOS,
    0,	 0,  0,    "",                     M_LISTEND
};


struct MENU more1Menu[] =
{
   10,	10,  0,    "KEYBOARD ASSIGNEMENTS",M_TITLE,
   47,	29,  0,    "A : AUTOPILOT",      			     M_TEXT,
   49,	41,  0,    "L : LAND ME",        			     M_TEXT,
   37,	53,  0,    "Tab : COCKPIT ON / OFF",                         M_TEXT,
   30,	65,  0,    "Alt-X : EXIT TO DOS",                            M_TEXT,
   37,  77,  0,    "Esc : MENU / EXIT MENU",                         M_TEXT,
   45,  89,  0,    "F1 : HELP",                                      M_TEXT,
   41, 101,  0,    "UP : COCKPIT VIEW",                              M_TEXT,
   24, 113,  0,    "DOWN : BACK VIEW",                               M_TEXT,
   31, 125,  0,    "LEFT : LEFT VIEW",                               M_TEXT,
   25, 137,  0,    "RIGHT : RIGHT VIEW",                             M_TEXT,
   49, 149,  0,    "T : TOWER OR GROUND VIEW",                       M_TEXT,
   48, 161,  0,    "S : SPOT VIEW",                                  M_TEXT,
  230, 180,  27,   "EXIT HELP",            M_EXITMENU,
  135, 180,  359,  "ABOUT",                M_ABOUT,
   10, 180,  345,  "EXIT TO DOS",          M_EXITDOS,
    0,	 0,  0,    "",                     M_LISTEND
};


struct MENU aboutMenu[] =
{
   10,	10,  0,    "ABOUT",                M_TITLE,
   10,	34,  0,    "THIS HELICOPTER SIMULATOR IS WRITTEN BY",        M_TEXT,
   10,	46,  0,    "STEFAN NAEF FOR THE  I B M  COMPETITION 1995.",  M_TEXT,
   10,	70,  0,    "VERSION 2.1b.",                                  M_TEXT,
   10,	94,  0,    "COPYRIGHT 1994 BY PRISCUS SOFTART, ZURICH.",     M_TEXT,
   10, 118,  0,    "My address:",                                    M_TEXT,
   10, 130,  0,    "Stefan Naef",                                    M_TEXT,
   10, 142,  0,    "Aemtlerstr.114",                                 M_TEXT,
   10, 154,  0,    "CH-8003 Zuerich",                                M_TEXT,
  230, 180,  27,   "EXIT ABOUT",           M_EXITMENU,
  135, 180,  359,  "MENU",                 M_MAINMENU,
   10, 180,  345,  "EXIT TO DOS",          M_EXITDOS,
    0,	 0,  0,    "",                     M_LISTEND
};


struct MENU *activeMenu = mainMenu;


void checkBox (int x, int y, int active)
{
  if (active)
    writeTextXY (x, y, "\1\2", MENUCOLOR);
  else
    writeTextXY (x, y, "\3\4", MENUCOLOR);
}

int lastMouseKey = 0;

int getMouseOpcode (void)
{
  struct MENU *menu = activeMenu;
  int mx, my, mKey;

  getMousePos (&mx, &my, &mKey);

  mx /= 2;

  if (mKey == lastMouseKey)
    return M_NONE;

  lastMouseKey = mKey;

  if ((mKey & 0x07) == 0x03)
    return M_EXITMENU;

  if (mKey)
  {
    while (menu->opcode != M_LISTEND)
    {
      if ((mx >= menu->x) && ((mx - 10) <= menu->x) &&
	  (my >= menu->y) && ((my - 10) <= menu->y))
	return (menu->opcode);

      menu++;
    }
  }

  return M_NONE;
}

int getKeyboardOpcode (void)
{
  struct MENU *menu = activeMenu;
  int key, opcode = M_NONE;

  if (kbhit())
  {
    key = toupper (getch());

    if ((key == 0) && (kbhit ()))
      key = getch () + 300;

    while (menu->opcode != M_LISTEND)
    {
      if (menu->key == key)
	opcode = menu->opcode;

      menu++;
    }

    while (kbhit()) getch ();
  }

  return opcode;
}


void displayItems (void)
{
  struct MENU *menu = activeMenu;

  while (menu->opcode != M_LISTEND)
  {
    switch (menu->opcode)
    {
      case M_TITLE :
	writeTextXY (menu->x, menu->y, menu->string, MENUCOLOR);
	drawLine (menu->x, menu->y + 10,
	  menu->x + textWide (menu->string), menu->y + 10, MENUCOLOR);
	break;
      case M_TEXT :
	writeTextXY (menu->x, menu->y, menu->string, MENUCOLOR);
	break;
      default :
	if (menu->string[0])
	  writeTextXY (menu->x + 16, menu->y, menu->string, MENUCOLOR);
    }

    menu++;
  }
}

void displaySettings (void)
{
  struct MENU *menu = activeMenu;

  while (menu->opcode != M_LISTEND)
  {
    int set = FALSE;

    switch (menu->opcode)
    {
      case M_VERYCOMPLEX : set = (complexity == VERYCOMPLEX);	break;
      case M_COMPLEX	 : set = (complexity == COMPLEX);	break;
      case M_SPARSE	 : set = (complexity == SPARSE);	break;
      case M_VERYSPARSE  : set = (complexity == VERYSPARSE);	break;
      case M_DYNAMIC	 : set = dynamicScenery;		break;
      case M_COCKPIT	 : set = cockpitOn;			break;
      case M_CLOUDS 	 : set = clouds;			break;
      case M_FRONTVIEW	 : set = (viewMode == FRONTVIEW);	break;
      case M_LEFTVIEW	 : set = (viewMode == LEFTVIEW);	break;
      case M_RIGHTVIEW	 : set = (viewMode == RIGHTVIEW);	break;
      case M_BACKVIEW	 : set = (viewMode == BACKVIEW);	break;
      case M_TOWERVIEW	 : set = (viewMode == TOWERVIEW);	break;
      case M_SPOTVIEW	 : set = (viewMode == SPOTVIEW);	break;
      case M_NOWINDS	 : set = (winds == NOWINDS);		break;
      case M_LIGHTWINDS  : set = (winds == LIGHTWINDS); 	break;
      case M_STRONGWINDS : set = (winds == STRONGWINDS);	break;
      case M_GROUNDDOTS  : set = groundDots;			break;
      case M_EASYCONTROL : set = easyControl;			break;
      case M_CONNECTCOM1 : set = (linkPort == COM_1);		break;
      case M_CONNECTCOM2 : set = (linkPort == COM_2);		break;
      case M_CONNECTOFF  : set = (linkPort == COM_NONE);	break;
      case M_ADLIBSOUND	 : set = (soundType == ADLIB_SOUND);    break;
      case M_LANDME    	 : set = autoLanding;                   break;
      case M_AUTOPILOT	 : set = autoPilot;			break;
      case M_COLLISION	 : set = collision;			break;
    }

    if ((menu->opcode != M_NONE) &&
	(menu->opcode != M_TITLE) &&
	(menu->opcode != M_TEXT))
      checkBox (menu->x, menu->y, set);

    menu++;
  }
}


void initMenu (struct MENU *menu)
{
  activeMenu = menu;
  setMousePointer (FALSE);
  putImage (0, 0, menuPicture, 320, 200);
  displayItems ();
  displaySettings ();
  flipScreen ();
  setMousePointer (TRUE);
}


int checkSettings (int opcode)
{
  switch (opcode)
  {
    case M_VERYCOMPLEX : complexity = VERYCOMPLEX;		break;
    case M_COMPLEX     : complexity = COMPLEX;			break;
    case M_SPARSE      : complexity = SPARSE;			break;
    case M_VERYSPARSE  : complexity = VERYSPARSE;		break;
    case M_DYNAMIC     : dynamicScenery = !dynamicScenery;
			 makeLink (COM_NONE);
			 break;
    case M_EXITDOS     : quit = TRUE; opcode = M_EXITMENU;	break;
    case M_COCKPIT     : cockpitOn = !cockpitOn;		break;
    case M_FRONTVIEW   : viewMode = FRONTVIEW; newView = TRUE;	break;
    case M_LEFTVIEW    : viewMode = LEFTVIEW;  newView = TRUE;	break;
    case M_RIGHTVIEW   : viewMode = RIGHTVIEW; newView = TRUE;	break;
    case M_BACKVIEW    : viewMode = BACKVIEW;  newView = TRUE;	break;
    case M_TOWERVIEW   : viewMode = TOWERVIEW; newView = TRUE;  break;
    case M_SPOTVIEW    : viewMode = SPOTVIEW;  newView = TRUE;	break;
    case M_NOWINDS     : winds = NOWINDS;			break;
    case M_LIGHTWINDS  : winds = LIGHTWINDS;			break;
    case M_STRONGWINDS : winds = STRONGWINDS;			break;
    case M_GROUNDDOTS  : groundDots = !groundDots;		break;
    case M_CLOUDS      : clouds = !clouds;			break;
    case M_EASYCONTROL : easyControl = !easyControl;		break;
    case M_AUTOPILOT   : autoPilot = !autoPilot;
			 autoLanding = FALSE;
			 break;
    case M_LANDME      : autoLanding = !autoLanding;
			 autoPilot = FALSE;
			 break;
    case M_COLLISION   : collision = !collision;		break;
    case M_CONNECTCOM1 : makeLink (COM_1);
			 dynamicScenery = FALSE;
			 break;
    case M_CONNECTCOM2 : makeLink (COM_2);
			 dynamicScenery = FALSE;
			 break;
    case M_CONNECTOFF  : makeLink (COM_NONE);			break;
    case M_ADLIBSOUND  : if (soundType == NO_SOUND)
			   initSound (ADLIB_SOUND);
			 else
			   initSound (NO_SOUND);
			 break;
    case M_MAINMENU    : initMenu (mainMenu);			break;
    case M_HELP        : initMenu (helpMenu);			break;
    case M_MORE1       : initMenu (more1Menu);			break;
    case M_ABOUT       : initMenu (aboutMenu);			break;
  }

  return opcode;
}



void menu (struct MENU *menu)
{
  int opcode = M_NONE;

  if (menu == NULL)
    menu = mainMenu;

  timerActive = FALSE;
  fadePalette (-30);
  muteSound ();
  setMousePos (320, 100);
  initMenu (menu);
  initColorPalette (menuPalette, 0, 255);
  setColorPalette (MENUCOLOR, 255, 255, 255, FALSE);
  fadePalette (30);

  while (opcode != M_EXITMENU)
  {
    opcode = getKeyboardOpcode ();

    if (opcode == M_NONE)
      opcode = getMouseOpcode ();

    if (opcode != M_NONE)
    {
      opcode = checkSettings (opcode);

      if (opcode != M_EXITMENU)
      {
	initMenu (activeMenu);
	dingSound ();
      }
    }
  }

  fadePalette (-30);
  setMousePointer (FALSE);
  setMousePos (320, 100);
  firstFrame = TRUE;
}

void checkKeyboard (void)
{
  int opcode;

  activeMenu = mainMenu;
  opcode = getKeyboardOpcode ();

  switch (opcode)
  {
    case M_EXITMENU : menu (mainMenu); break;
    case M_HELP     : menu (helpMenu); break;
    default         : checkSettings (opcode);
  }
}

