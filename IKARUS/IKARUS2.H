//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Definitionen fuer den gesammten Simulator
//*
//***************************************************************************

struct POINT
{
  int x;
  int y;
  int z;
};

struct MOBILE
{
  int number, state, timer;
  int steeringPower;
  int steeringDirX, steeringDirY;
  int power, xzSpeed, ySpeed;
  int fuel, collision;
  struct POINT position;
  struct POINT areaPos;
  struct POINT angle;
  struct POINT moveAngle;
  struct POINT targetPos;
  struct POINT targetArea;
  int landingHight;
  int contactHight;
  int contactCount;
  long contactDist2;
  struct POINT homePos;
  struct POINT homeArea;
  struct POINT homeAngle;
};

#define MINCLIPX	0
#define MINCLIPY	0
#define MAXCLIPX      319
#define MAXCLIPY      199

#define AREAMAPSIZE    50
#define AREAWIDE     4000
#define AREAVIEWWIDE	3

enum {NONE, POLYGON, POLYLINE, PIXEL, CIRCLE, TEXTURE};
enum {NOCHECK, CHECK, CONTACT, SURFACE, MIDPOINT, GROUND, WINDBAG};
enum {VERYSPARSE, SPARSE, COMPLEX, VERYCOMPLEX};
enum {FALSE, TRUE};
enum {NOWINDS, LIGHTWINDS, STRONGWINDS};

// Autopilot status
enum {PARKING, FLYING, LANDING, IDLE};

enum {NO_ERROR, GRAPHICS_ERROR, DB_FORMAT_ERROR,
      DB_NOT_FOUND_ERROR, MEMORY_ERROR, MOUSE_ERROR};

enum {FRONTVIEW, LEFTVIEW, RIGHTVIEW, BACKVIEW, TOWERVIEW, SPOTVIEW};

#define RED	  	     0x80
#define GREEN    	     0x90
#define BLUE	             0xA0
#define YELLOW	   	     0xB0
#define GREY	   	     0xC0
#define BROWN 	  	     0xD0
#define SKYBLUE      	     0xE0
#define FLASH 	             0xF0

#define SOLIDFILL	   0x0000
#define PLAIDFILL          0x0100
#define SHADOWFILL         0x0200
#define METALLICFILL       0x0300

#define VIEWPOINTS             50
#define MOBILES		       20
#define MAXDIST	    	    32767
#define MENUCOLOR   	      255
#define CHOPPERS                4
#define TEXTURES               64

enum {COM_NONE, COM_1, COM_2};
enum {NO_SOUND, ADLIB_SOUND, SPEAKER_SOUND};

#define CIRCLEDIST          15000