//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Windhose
//*
//***************************************************************************

#include <stdlib.h>
#include "ikarus2.h"
#include "geometry.h"

extern struct MOBILE wind;

int windbagTower[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	 340,	   0,
  1, NONE, CONTACT, 0, 0, 0,
	   0,	 200,	   0,
  4, POLYGON, NOCHECK, MAXDIST, GREY + 7, GREY + 6,
	 -50,	   0,	 -50,
	 -50,	   0,	  50,
	  50,	   0,	  50,
	  50,	   0,	 -50,
  5, POLYLINE, NOCHECK, MAXDIST, METALLICFILL + GREY, 0,
	 -40,	   0,	  40,
	  30,	 100,	  30,
	 -20,	 200,	  20,
	  10,	 300,	  10,
	   0,	 400,	   0,
  5, POLYLINE, NOCHECK, MAXDIST, METALLICFILL + GREY, 0,
	  40,	   0,	 -40,
	 -30,	 100,	 -30,
	  20,	 200,	 -20,
	 -10,	 300,	 -10,
	   0,	 400,	   0,
  5, POLYLINE, NOCHECK, MAXDIST, METALLICFILL + GREY, 0,
	  40,	   0,	  40,
	  30,	 100,	 -30,
	  20,	 200,	  20,
	  10,	 300,	 -10,
	   0,	 400,	   0,
  5, POLYLINE, NOCHECK, MAXDIST, METALLICFILL + GREY, 0,
	 -40,	   0,	 -40,
	 -30,	 100,	  30,
	 -20,	 200,	 -20,
	 -10,	 300,	  10,
	   0,	 400,	   0,
  0
};

int windbag[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	 -60,	   0,
  8, POLYGON, NOCHECK, MAXDIST, RED + 10, RED + 10,
	  10,	 -60,	  20,
	 -10,	 -60,	  20,
	 -20,	 -60,	  10,
	 -20,	 -60,	 -10,
	 -10,	 -60,	 -20,
	  10,	 -60,	 -20,
	  20,	 -60,	 -10,
	  20,	 -60,	  10,
  8, POLYGON, NOCHECK, MAXDIST, RED + 10, RED + 10,
	  -5,	-160,	  10,
	   5,	-160,	  10,
	  10,	-160,	   5,
	  10,	-160,	  -5,
	   5,	-160,	 -10,
	  -5,	-160,	 -10,
	 -10,	-160,	  -5,
	 -10,	-160,	   5,
  4, POLYGON, NOCHECK, MAXDIST, RED + 10, RED + 10,
	 -20,	 -60,	   0,
	  20,	 -60,	   0,
	  10,	-160,	   0,
	 -10,	-160,	   0,
  4, POLYGON, NOCHECK, MAXDIST, RED + 10, RED + 10,
	   0,	 -60,	 -20,
	   0,	 -60,	  20,
	   0,	-160,	  10,
	   0,	-160,	 -10,
  4, POLYGON, CHECK, MAXDIST, RED + 3, RED + 8,
	 -15,	 -60,	  15,
	  15,	 -60,	  15,
	  15,	 -60,	 -15,
	 -15,	 -60,	 -15,
  4, POLYGON, CHECK, MAXDIST, RED + 3, RED + 8,
	  -7,	-160,	  -7,
	   7,	-160,	  -7,
	   7,	-160,	   7,
	  -7,	-160,	   7,
  3, POLYLINE, NOCHECK, MAXDIST, RED + 10, 0,
	   0,	   0,	   0,
	 -20,	 -60,	   0,
	  20,	 -60,	   0,
  0
};

int tWindbagTower[sizeof (windbagTower) / sizeof (int)];
int tWindbag[sizeof (windbag) / sizeof (int)];

void displayWindbag (struct POINT *pos)
{
  setPosition (0, 400, 0);
  setAngle (minmax (wind.xzSpeed * 50, 200, 900), wind.angle.y, 0);
  setOrder ("xyt");
  transformObject (windbag, tWindbag);

  setPosition (pos->x, pos->y, pos->z);
  setOrder ("t");
  transformObject (windbagTower, &tWindbagTower);
  transformObject (tWindbag, tWindbag);
  addToObjectList (tWindbagTower);
  addToObjectList (tWindbag);
}


