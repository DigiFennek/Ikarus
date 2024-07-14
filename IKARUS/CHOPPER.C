//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Helikopter Aussenansicht
//*  - Helikopter Simulator
//*  - Helikopter Autopilot
//*
//***************************************************************************

#include <stdlib.h>
#include "ikarus2.h"
#include "geometry.h"

extern struct MOBILE wind;

extern int easyControl;
extern int collision;

static int tail[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	 -80,
  1, NONE, CONTACT, 300, 0, 0,
	   0,	   0,	-200,
  4, POLYGON, NOCHECK, -2000, RED + 12, RED + 12,
	   0,	 -24,	 -94,
	   0,	  18,	 -88,
	   0,	  38,	-454,
	   0,	  26,	-452,
  4, POLYGON, CHECK, 2000, RED + 10, GREY + 5,
	  60,	  34,	-340,
	  60,	  34,	-316,
	 -60,	  34,	-316,
	 -60,	  34,	-340,
  4, POLYGON, SURFACE, 4000, FLASH + 5, FLASH + 5,
	 -55,	  34,	-328,
	 -55,	  34,	-328,
	 -50,	  34,	-328,
	 -50,	  34,	-328,
  4, POLYGON, SURFACE, 4000, FLASH + 4, FLASH + 4,
	  55,	  34,	-328,
	  55,	  34,	-328,
	  50,	  34,	-328,
	  50,	  34,	-328,
  11, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	   0,	 -24,	 -94,
	 -18,	  18,	 -88,
	   0,	 -14,	-156,
	 -16,	  22,	-154,
	   0,	  -4,	-222,
	 -14,	  26,	-220,
	   0,	   6,	-288,
	 -12,	  30,	-286,
	   0,	  16,	-354,
	 -10,	  34,	-352,
	   0,	  26,	-420,
  11, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	  18,	  18,	 -88,
	   0,	 -14,	-156,
	  16,	  22,	-154,
	   0,	  -4,	-222,
	  14,	  26,	-220,
	   0,	   6,	-288,
	  12,	  30,	-286,
	   0,	  16,	-354,
	  10,	  34,	-352,
	   0,	  26,	-420,
	   8,	  38,	-420,
  11, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	 -18,	  18,	 -88,
	  18,	  18,	 -88,
	 -16,	  22,	-154,
	  16,	  22,	-154,
	 -14,	  26,	-220,
	  14,	  26,	-220,
	 -12,	  30,	-286,
	  12,	  30,	-286,
	 -10,	  34,	-352,
	  10,	  34,	-352,
	  -8,	  38,	-420,
  3, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	   0,	  26,	-420,
	   8,	  38,	-420,
	  -8,	  38,	-420,
  7, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	   0,	  12,	-320,
	   0,	  34,	-450,
	   0,	  20,	-454,
	   0,	   0,	-452,
	   0,	 -20,	-436,
	   0,	 -28,	-420,
	   0,	 -28,	-400,
  2, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	  10,	  34,	-352,
	   0,	 -18,	-380,
  2, POLYLINE, NOCHECK, 2000, RED + 12, 0,
	 -10,	  34,	-352,
	   0,	 -18,	-380,
  4, POLYGON, CHECK, 2000, RED + 10, GREY + 5,
	 -60,	  34,	-340,
	 -60,	  34,	-316,
	  60,	  34,	-316,
	  60,	  34,	-340,
  4, POLYGON, SURFACE, 4000, FLASH + 5, FLASH + 5,
	 -55,	  34,	-328,
	 -55,	  34,	-328,
	 -50,	  34,	-328,
	 -50,	  34,	-328,
  4, POLYGON, SURFACE, 4000, FLASH + 4, FLASH + 4,
	  55,	  34,	-328,
	  55,	  34,	-328,
	  50,	  34,	-328,
	  50,	  34,	-328,
  0
};

static int body1[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	   0,
  1, NONE, CONTACT, 300, 0, 0,
	   0,	   0,	   0,
  5, POLYGON, NOCHECK, -8000, BLUE + 15, RED + 12,
	   0,	  52,	  16,
	   0,	  52,	 156,
	   0,	   4,	 172,
	   0,	 -44,	 156,
	   0,	 -44,	  16,
  5, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	   0,	  52,	  48,
	  42,	  36,	  32,
	  26,	  24,	  16,
	 -26,	  24,	  16,
	 -42,	  36,	  32,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	 -42,	  36,	 116,
	 -42,	  36,	  32,
	 -38,	 -30,	  52,
	 -38,	 -30,	 136,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	  38,	 -30,	 136,
	  38,	 -30,	  52,
	  42,	  36,	  32,
	  42,	  36,	 116,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	 -32,	   4,	 156,
	 -42,	  36,	 116,
	 -38,	 -30,	 136,
	 -32,	 -10,	 156,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	  32,	 -10,	 156,
	  38,	 -30,	 136,
	  42,	  36,	 116,
	  32,	   4,	 156,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	 -38,	 -30,	 136,
	 -38,	 -30,	  52,
	 -26,	 -44,	  40,
	 -26,	 -44,	 130,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	  26,	 -44,	 130,
	  26,	 -44,	  40,
	  38,	 -30,	  52,
	  38,	 -30,	 136,
  5, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	   0,	 -14,	 172,
	 -32,	 -10,	 156,
	 -38,	 -30,	 136,
	 -26,	 -44,	 130,
	   0,	 -44,	 142,
  5, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	   0,	 -44,	 142,
	  26,	 -44,	 130,
	  38,	 -30,	 136,
	  32,	 -10,	 156,
	   0,	 -14,	 172,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	   0,	   8,	 172,
	 -32,	   4,	 156,
	 -32,	 -10,	 156,
	   0,	 -14,	 172,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	   0,	 -14,	 172,
	  32,	 -10,	 156,
	  32,	   4,	 156,
	   0,	   8,	 172,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	   0,	   8,	 172,
	   0,	  52,	 124,
	 -42,	  36,	 116,
	 -32,	   4,	 156,
  4, POLYGON, CHECK, 8000, BLUE + 15, RED + 12,
	  32,	   4,	 156,
	  42,	  36,	 116,
	   0,	  52,	 124,
	   0,	   8,	 172,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	   0,	  52,	 124,
	   0,	  52,	  48,
	 -42,	  36,	  32,
	 -42,	  36,	 116,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	  42,	  36,	 116,
	  42,	  36,	  32,
	   0,	  52,	  48,
	   0,	  52,	 124,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	 -42,	  36,	  32,
	 -26,	  24,	  16,
	 -26,	 -44,	  40,
	 -38,	 -30,	  52,
  4, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	  38,	 -30,	  52,
	  26,	 -44,	  40,
	  26,	  24,	  16,
	  42,	  36,	  32,
  5, POLYGON, CHECK, 8000, RED + 12, RED + 12,
	 -26,	 -44,	 130,
	 -26,	 -44,	 -32,
	  26,	 -44,	 -32,
	  26,	 -44,	 130,
	   0,	 -44,	 142,
  4, POLYGON, SURFACE, 8000, FLASH + 2, FLASH + 2,
	  -5,	 -44,	 100,
	  -5,	 -44,	 110,
	   5,	 -44,	 110,
	   5,	 -44,	 100,
  0
};

static int body2[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	 -60,
  4, POLYGON, NOCHECK, -8000, RED + 12, GREY,
	   0,	  24,	  16,
	   0,	  24,	 -94,
	   0,	 -44,	 -94,
	   0,	 -44,	  40,
  4, POLYGON, CHECK,  8000, RED + 12, GREY,
	 -26,	  24,	  16,
	 -26,	  24,	 -32,
	 -26,	 -44,	 -32,
	 -26,	 -44,	  40,
  4, POLYGON, CHECK,  8000, RED + 12, GREY,
	  26,	 -44,	  40,
	  26,	 -44,	 -32,
	  26,	  24,	 -32,
	  26,	  24,	  16,
  4, POLYGON, CHECK,  8000, RED + 12, GREY,
	  26,	  24,	  16,
	  26,	  24,	 -32,
	 -26,	  24,	 -32,
	 -26,	  24,	  16,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	 -26,	  24,	 -32,
	   0,	 -24,	 -94,
	 -26,	 -44,	 -32,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	  26,	 -44,	 -32,
	   0,	 -24,	 -94,
	  26,	  24,	 -32,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	   0,	 -24,	 -94,
	  18,	  18,	 -88,
	  26,	  24,	 -32,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	 -26,	  24,	 -32,
	 -18,	  18,	 -88,
	   0,	 -24,	 -94,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	 -26,	 -44,	 -32,
	   0,	 -24,	 -94,
	  26,	 -44,	 -32,
  4, POLYGON, CHECK,  8000, RED + 12, GREY,
	  26,	  24,	 -32,
	  18,	  18,	 -88,
	 -18,	  18,	 -88,
	 -26,	  24,	 -32,
  3, POLYGON, CHECK,  8000, RED + 12, GREY,
	   0,	 -24,	 -94,
	 -18,	  18,	 -88,
	  18,	  18,	 -88,
  0
};

static int rack[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	-100,	   0,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -56,	 -44,	  60,
	 -56,	 -44,	  70,
	  56,	 -44,	  70,
	  56,	 -44,	  60,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -56,	 -44,    -30,
	 -56,	 -44,	 -40,
	  56,	 -44,	 -40,
	  56,	 -44,	 -30,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -56,	 -44,	 -30,
	 -56,	 -44,	 -40,
	 -56,	 -76,	 -40,
	 -56,	 -76,	 -30,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	  56,	 -44,	 -30,
	  56,	 -44,	 -40,
	  56,	 -76,	 -40,
	  56,	 -76,	 -30,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -56,	 -44,	  60,
	 -56,	 -44,	  70,
	 -56,	 -76,	  70,
	 -56,	 -76,	  60,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	  56,	 -44,	  60,
	  56,	 -44,	  70,
	  56,	 -76,	  70,
	  56,	 -76,	  60,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	  60,	 -76,	 -60,
	  60,	 -76,	 108,
	  52,	 -76,	 108,
	  52,	 -76,	 -60,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -60,	 -76,	 -60,
	 -60,	 -76,	 108,
	 -52,	 -76,	 108,
	 -52,	 -76,	 -60,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	  60,	 -68,	 120,
	  60,	 -76,	 108,
	  52,	 -76,	 108,
	  52,	 -68,	 120,
  4, POLYGON, NOCHECK, 8000, GREY + 6, GREY + 6,
	 -60,	 -68,	 120,
	 -60,	 -76,	 108,
	 -52,	 -76,	 108,
	 -52,	 -68,	 120,
  0
};

static int tailRotor[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	-200,	   0,	   0,
  4, POLYGON, NOCHECK, 8000, GREY + 7, GREY + 7,
	   0,	   0,	  -6,
	   0,	   0,	   6,
	   0,	 -60,	   6,
	   0,	 -60,	  -6,
  4, POLYGON, NOCHECK, 8000, GREY + 7, GREY + 7,
	   0,	   4,	  -4,
	   0,	  -4,	   4,
	   0,	  24,	  56,
	   0,	  36,	  48,
  4, POLYGON, NOCHECK, 8000, GREY + 7, GREY + 7,
	   0,	   4,	   4,
	   0,	  -4,	  -4,
	   0,	  24,	 -56,
	   0,	  36,	 -48,
  0
};

static int tailShadow[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	-200,	   0,	   0,
 12, POLYGON, NOCHECK, 4000, SHADOWFILL, SHADOWFILL,
	   0,	   0,	  60,
	   0,	  30,	  51,
	   0,	  51,	  30,
	   0,	  60,	   0,
	   0,	  51,	 -30,
	   0,     30,	 -51,
	   0,	   0,	 -60,
	   0,	 -30,	 -51,
	   0,	 -51,	 -30,
	   0,	 -60,	   0,
	   0,	 -51,	  30,
	   0,    -30,	  51,
  0
};

static int mainRotor[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	 200,	   0,
  1, NONE, CONTACT, 300, 0, 0,
	   0,	   0,	   0,
  5, POLYGON, NOCHECK, MAXDIST, GREY + 7, GREY + 7,
	   0,	   0,	   0,
	 -20,	   0,	 -60,
	 -20,	   0,	-300,
	  20,	   0,	-300,
	  20,	   0,	   0,
  5, POLYGON, NOCHECK, MAXDIST, GREY + 7, GREY + 7,
	   0,	   0,	   0,
	  60,	   0,	  12,
	 268,	   0,	 132,
	 248,	   0,	 164,
	   8,	   0,	  16,
  5, POLYGON, NOCHECK, MAXDIST, GREY + 7, GREY + 7,
	   0,	   0,	   0,
	 -40,	   0,	  48,
	-248,	   0,	 164,
	-268,	   0,	 132,
	   8,	   0,	 -16,
  0
};

static int mainShadow[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	 200,	   0,
 12, POLYGON, NOCHECK, 8000, SHADOWFILL, SHADOWFILL,
	   0,	   0,	 300,
	 150,	   0,	 260,
	 260,	   0,	 150,
	 300,	   0,	   0,
	 260,	   0,	-150,
	 150,      0,	-260,
	   0,	   0,	-300,
	-150,	   0,	-260,
	-260,	   0,	-150,
	-300,	   0,	   0,
	-260,	   0,	 150,
	-150,      0,	 260,
  0
};

static int mainAxle[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	  50,	   0,
  4, POLYGON, NOCHECK, MAXDIST, GREY + 3, GREY + 3,
	  -7,	  26,	   0,
	  -7,	  78,	   0,
	   7,	  78,	   0,
	   7,	  26,	   0,
  4, POLYGON, NOCHECK, MAXDIST, GREY + 3, GREY + 3,
	   0,	  26,	  -7,
	   0,	  78,	  -7,
	   0,	  78,	   7,
	   0,	  26,	   7,
  0
};

static int engine[] =
{
  1, COMPLEX, MIDPOINT, 0, 0, 0,
	   0,	  40,	 -60,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -16,	  56,	 -66,
	  16,	  56,	 -66,
	  16,	  24,	 -70,
	 -16,	  24,	 -70,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -16,	  56,	 -20,
	  16,	  56,	 -20,
	  16,	  56,	 -66,
	 -16,	  56,	 -66,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -16,	  24,	 -10,
	  16,	  24,	 -10,
	  16,	  44,	 -10,
	 -16,	  44,	 -10,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -16,	  44,	 -10,
	  16,	  44,	 -10,
	  16,	  56,	 -20,
	 -16,	  56,	 -20,
  5, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -16,	  56,	 -20,
	 -16,	  56,	 -66,
	 -16,	  24,	 -70,
	 -16,	  24,	 -10,
	 -16,	  44,	 -10,
  5, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	  16,	  44,	 -10,
	  16,	  24,	 -10,
	  16,	  24,	 -70,
	  16,	  56,	 -66,
	  16,	  56,	 -20,
  6, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	   0,	  56,	-128,
	  12,	  50,	-130,
	  12,	  38,	-132,
	   0,	  32,	-134,
	 -12,	  38,	-132,
	 -12,	  50,	-130,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -12,	  44,	 -68,
	 -12,	  50,	-130,
	 -12,	  38,	-132,
	 -12,	  32,	 -70,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	  12,	  32,	 -70,
	  12,	  38,	-132,
	  12,	  50,	-130,
	  12,	  44,	 -68,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	   0,	  50,	 -66,
	   0,	  56,	-128,
	 -12,	  50,	-130,
	 -12,	  44,	 -68,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	  12,	  44,	 -68,
	  12,	  50,	-130,
	   0,	  56,	-128,
	   0,	  50,	 -66,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	 -12,	  32,	 -70,
	 -12,	  38,	-132,
	   0,	  32,	-134,
	   0,	  26,	 -72,
  4, POLYGON, CHECK, 3000, GREY + 5, GREY + 4,
	   0,	  26,	 -72,
	   0,	  32,	-134,
	  12,	  38,	-132,
	  12,	  32,	 -70,
  0
};

static int shadow[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,   -200,	   0,
  6, POLYGON, NOCHECK, 6000, SHADOWFILL, SHADOWFILL,
	 -75,	   0,	 200,
	 -75,	   0,	 -45,
	   0,	   0,	-105,
	  75,	   0,	 -45,
	  75,	   0,	 200,
	   0,	   0,	 220,
  4, POLYGON, NOCHECK, 6000, SHADOWFILL, SHADOWFILL,
	 -40,	   0,	 -60,
	  40,	   0,	 -60,
	  15,	   0,	-420,
	 -15,	   0,	-420,
  0
};

static int tBody1      [CHOPPERS][sizeof (body1     ) / sizeof (int)];
static int tBody2      [CHOPPERS][sizeof (body2     ) / sizeof (int)];
static int tTail       [CHOPPERS][sizeof (tail      ) / sizeof (int)];
static int tRack       [CHOPPERS][sizeof (rack      ) / sizeof (int)];
static int tTailRotor  [CHOPPERS][sizeof (tailRotor ) / sizeof (int)];
static int tTailShadow [CHOPPERS][sizeof (tailShadow) / sizeof (int)];
static int tMainRotor  [CHOPPERS][sizeof (mainRotor ) / sizeof (int)];
static int tMainShadow [CHOPPERS][sizeof (mainShadow) / sizeof (int)];
static int tMainAxle   [CHOPPERS][sizeof (mainAxle  ) / sizeof (int)];
static int tEngine     [CHOPPERS][sizeof (engine    ) / sizeof (int)];
static int tShadow     [CHOPPERS][sizeof (shadow    ) / sizeof (int)];

int rotation[CHOPPERS];

void displayChopper (struct MOBILE *ch)
{
  int n = ch->number;

  if (n < CHOPPERS)
  {
    // Rotor drehen
    if (ch->power > 300)
      rotation[n] = rotation[n] + 550;
    else
      rotation[n] = rotation[n] + max ((ch->power << 1) - 200, 0);

    rotation[n] %= 3600;

    // Heckrotor
    setAngle (-rotation[n], 0, 0);
    setOrder ("xt");
    setPosition (-20, 40, -410);
    transformObject (tailRotor, &tTailRotor[n]);
    transformObject (tailShadow, &tTailShadow[n]);

    // Hauptrotor
    setAngle (0, rotation[n], 0);
    setOrder ("yt");
    setPosition (0, 80, 0);
    transformObject (mainRotor, &tMainRotor[n]);
    transformObject (mainShadow, &tMainShadow[n]);

    // Karosserie
    setPosition (ch->position.x, ch->position.y + 80, ch->position.z);
    setAngle (ch->angle.x, ch->angle.y, ch->angle.z);
    setOrder ("xzyt");

    transformObject (&tTailRotor[n], &tTailRotor[n]);
    transformObject (&tTailShadow[n], &tTailShadow[n]);
    transformObject (&tMainRotor[n], &tMainRotor[n]);
    transformObject (&tMainShadow[n], &tMainShadow[n]);
    transformObject (mainAxle, &tMainAxle[n]);
    transformObject (body1, &tBody1[n]);
    transformObject (body2, &tBody2[n]);
    transformObject (tail, &tTail[n]);
    transformObject (rack, &tRack[n]);
    transformObject (engine, &tEngine[n]);

    addToObjectList (&tMainRotor[n]);
    addToObjectList (&tMainAxle[n]);
    addToObjectList (&tRack[n]);
    addToObjectList (&tBody1[n]);
    addToObjectList (&tBody2[n]);
    addToObjectList (&tEngine[n]);
    addToObjectList (&tTail[n]);
    addToObjectList (&tTailRotor[n]);

    // Rotorschatten
    if (ch->power > 400)
    {
      addToObjectList (&tMainShadow[n]);
      addToObjectList (&tTailShadow[n]);
    }

    // Schatten am Boden
    if (ch->position.y < (ch->landingHight + 1000))
    {
      setPosition (ch->position.x, ch->landingHight, ch->position.z);
      transformObject (shadow, &tShadow[n]);
      addToObjectList (&tShadow[n]);
    }
  }
}


void simulateChopper (struct MOBILE *ch)
{
  // am Boden ist keine Steuerung moeglich
  if (ch->position.y <= ch->landingHight)
    ch->steeringDirX = ch->steeringDirY = 0;

  // Steuerwinkel und Steuerleistung beschraenken
  ch->steeringDirX = minmax (ch->steeringDirX, -20, 80);
  ch->steeringDirY = minmax (ch->steeringDirY, -150, 150);
  ch->steeringPower = minmax (ch->steeringPower, 600, 800);

  // Landehoehe bestimmen
  if (ch->contactCount >= 3)
    ch->landingHight = ch->contactHight;
  else
    ch->landingHight = 0;

  // Kollision unterhalb der Landehoehe oder zu schnell gelandet
  if ((ch->contactCount && (ch->contactHight > ch->position.y)) ||
  ((ch->ySpeed < -20) && ((ch->position.y + ch->ySpeed) < ch->landingHight)))
  {
    if ((collision) && (!ch->collision))
    {
      ch->timer = 100;
      ch->collision = TRUE;
    }
  }

  if (!ch->collision)
  {
    // Winkel der Heli-Grafik der Steuerrichtung folgen lassen
    followAngle (&ch->angle.x, ch->steeringDirX, 50);
    followAngle (&ch->angle.z, ch->steeringDirY, 50);

    // Winkel der Bewegungsrichtung der Steuerrichtung traege folgen lassen
    followAngle (&ch->moveAngle.x, ch->steeringDirX, 150);
    followAngle (&ch->moveAngle.z, ch->steeringDirY, 150);

    // Wenn der Sprit am ende ist...
    if (ch->fuel == 0) ch->steeringPower = 0;

    // Ist-Leistung der Soll-Leistung folgen lassen
    if (ch->steeringPower > 600)
    {
      if (ch->power < ch->steeringPower) ch->power += 1;
      if (ch->power > ch->steeringPower) ch->power -= 1;
    }
    else
      ch->power = max (0, ch->power - 1);

    // Drehung der Heli-Grafik um die y-Achse
    ch->angle.y += getAngleDiff (ch->angle.z, 0) >> 2;
    ch->angle.y = adjustAngle (ch->angle.y);

    // Geschwindigkeit in xz-Richtung
    ch->xzSpeed = intSinMul (ch->moveAngle.x, ch->power);

    // Drehung um die y-Achse
    followAngle (&ch->moveAngle.y, ch->angle.y, ch->xzSpeed >> 2);

    // Geschwindigkeit in y-Richtung
    ch->ySpeed = intCosMul (ch->moveAngle.x, ch->power - 700) >> 1;

    // Steuerung leicht erschweren
    if (!easyControl)
    {
      ch->ySpeed -= abs (intSinMul (ch->moveAngle.x, ch->xzSpeed));
      ch->ySpeed -= abs (intSinMul (ch->moveAngle.z, ch->xzSpeed));
    }

    // Bewegung in xz-Richtung
    ch->position.z += intCosMul (ch->moveAngle.y, ch->xzSpeed);
    ch->position.x += intSinMul (ch->moveAngle.y, ch->xzSpeed);

    // Bewegung in y-Richtung
    ch->position.y += ch->ySpeed;

    // Wind zumischen
    if (ch->position.y > (ch->landingHight + 50))
    {
      ch->position.z -= intCosMul (wind.angle.y, wind.xzSpeed);
      ch->position.x -= intSinMul (wind.angle.y, wind.xzSpeed);
    }

    // ySpeed ist am Boden 0
    if (ch->position.y <= ch->landingHight)
      ch->ySpeed = 0;

    // Hoehe einschraenken
    ch->position.y = minmax (ch->position.y, ch->landingHight, 10000);

    // Treibstoffverbrauch
    if (ch->power)
      ch->fuel = max (0, ch->fuel - 1);
  }

  else
  // crash !!!
  {
    ch->power = 0;

    // Ein paar tragische Bewegungen
    if (ch->position.y <= ch->landingHight)
    {
      followAngle (&ch->angle.x, 200, 5);
      followAngle (&ch->angle.y, 900, 5);
      followAngle (&ch->angle.z, 800, 5);
    }
    else
    {
      followAngle (&ch->angle.x, 800, 5);
      followAngle (&ch->angle.y, 800, 5);
      followAngle (&ch->angle.z, 300, 5);
    }

    // Auf den Boden fallen
    ch->position.y = max (ch->position.y - 20, ch->landingHight);
  }

  // Timer
  ch->timer = max (ch->timer - 1, 0);

  // Arealkoordinaten anpassen
  adjustArea (ch);
}


void autoPilotChopper (struct MOBILE *ch)
{   
  int alpha, beta, dist;

  if (ch->state != IDLE)
  {
    // Leistung entsprechend soll - und isthoehe regeln
    dist = ch->targetPos.y - ch->position.y;
    ch->steeringPower = 690 + minmax (dist / 10, -10, 30);

    // Flugrichtung bestimmen
    dist = targetDirection (&alpha, &beta, ch);
    ch->steeringDirY = getAngleDiff (-alpha, ch->angle.y) / 10;
  }
  else
  {
    ch->targetArea = ch->homeArea;
    ch->targetPos  = ch->homePos;
  }

  if (ch->state == FLYING)
  {
    // wenn das Zielareal erreicht ist wird ein neues ermittelt
    if ((ch->areaPos.x == ch->targetArea.x) &&
	(ch->areaPos.z == ch->targetArea.z))
    {
      // Neues Areal ermitteln oder zurueck zur Base
      if (random (10))
      {
	ch->targetArea.x = random (AREAMAPSIZE);
	ch->targetArea.z = random (AREAMAPSIZE);
	ch->targetPos.y = ch->landingHight + 2000 + random (100) * 30;
      }
      else
	ch->state = LANDING;

      // Treibstoff bald am ende ?
      if (ch->fuel < 10000)
	ch->state = LANDING;
    }

    // die Steuerung ist erst ab einer minimalen Hoehe frei
    if (ch->position.y < (ch->landingHight + 500))
    {
      ch->steeringDirX = 5;
      ch->steeringDirY = 0;
      ch->targetPos.y = ch->landingHight + 1000;
    }
    else
      ch->steeringDirX = 60;
  }

  if (ch->state == LANDING)
  {
    // das Ziel ist der Heimflugplatz
    ch->targetArea = ch->homeArea;
    ch->targetPos  = ch->homePos;

    // ist der Flugplatz in Sichtweite ?
    if ((abs (ch->targetArea.x - ch->areaPos.x) <= AREAVIEWWIDE) &&
	(abs (ch->targetArea.z - ch->areaPos.z) <= AREAVIEWWIDE))
    {
      if (dist > 100)
	ch->targetPos.y += min (dist, 1000);

      if ((dist > 500) && (ch->position.y < (ch->landingHight + 100)))
	ch->steeringDirX = ch->steeringDirY = 0;
      else
	ch->steeringDirX = minmax (dist / 100, 5, 40);

      if ((dist < 500) && (abs (ch->steeringDirY) > 450))
      {
	ch->steeringDirX = -ch->steeringDirX;
	ch->steeringDirY = 0;
      }

      if ((ch->position.y == ch->landingHight) && (dist < 200))
	ch->state = PARKING;
    }
    else
    {
      ch->targetPos.y = ch->landingHight + 2000;

      // die Steuerung ist erst ab einer minimalen Hoehe frei
      if (ch->position.y < (ch->landingHight + 400))
      {
	ch->steeringDirX = 5;
	ch->steeringDirY = 0;
      }
      else
	ch->steeringDirX = 60;
    }
  }

  if (ch->state == PARKING)
  {
    // Turbine abschalten
    ch->steeringPower = 0;

    if (ch->position.y > 0)
      ch->state = FLYING;

    // Zufaellig starten
    if ((ch->fuel > 29000) && (!random (200)))
      ch->state = FLYING;
  }

  if ((ch->areaPos.x == ch->homeArea.x) &&
      (ch->areaPos.z == ch->homeArea.z) &&
      (ch->power == 0) && (ch->position.y == ch->landingHight))
  {
    // Tank auffuellen
    ch->fuel = min (30000, ch->fuel + 50);
  }
}
