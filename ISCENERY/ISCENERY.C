//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Database generator
//*
//***************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include <alloc.h>
#include "ikarus2.h"

int tower[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	 100,	   0,
  8, NONE, CONTACT, 300, 0, 0,
	-100,	 550,	-100,
	-100,	 550,	 100,
	 100,	 550,	 100,
	 100,	 550,	-100,
	-100,	 300,	-100,
	-100,	 300,	 100,
	 100,	 300,	 100,
	 100,	 300,	-100,
  4, POLYGON, CHECK, MAXINT, GREY + 10, GREY + 10,
	-200,	 350,	-200,
	-250,	 400,	-250,
	 250,	 400,	-250,
	 200,	 350,	-200,
  4, POLYGON, CHECK, MAXINT, GREY + 9, GREY + 9,
	 200,	 350,	-200,
	 250,	 400,	-250,
	 250,	 400,	 250,
	 200,	 350,	 200,
  4, POLYGON, CHECK, MAXINT, GREY + 10, GREY + 10,
	 200,	 350,	 200,
	 250,	 400,	 250,
	-250,	 400,	 250,
	-200,	 350,	 200,
  4, POLYGON, CHECK, MAXINT, GREY + 9, GREY + 9,
	-200,	 350,	 200,
	-250,	 400,	 250,
	-250,	 400,	-250,
	-200,	 350,	-200,
  4, POLYGON, CHECK, MAXINT, GREY + 12, GREY + 12,
	-200,	   0,	-200,
	-200,	 350,	-200,
	 200,	 350,	-200,
	 200,	   0,	-200,
  4, POLYGON, CHECK, MAXINT, GREY + 11, GREY + 11,
	 200,	   0,	-200,
	 200,	 350,	-200,
	 200,	 350,	 200,
	 200,	   0,	 200,
  4, POLYGON, CHECK, MAXINT, GREY + 12, GREY + 12,
	 200,	   0,	 200,
	 200,	 350,	 200,
	-200,	 350,	 200,
	-200,	   0,	 200,
  4, POLYGON, CHECK, MAXINT, GREY + 11, GREY + 11,
	-200,	   0,	 200,
	-200,	 350,	 200,
	-200,	 350,	-200,
	-200,	   0,	-200,
  4, POLYGON, CHECK, MAXINT, GREY + 8, GREY + 8,
	-250,	 400,	-250,
	-250,	 550,	-250,
	 250,	 550,	-250,
	 250,	 400,	-250,
  4, POLYGON, CHECK, MAXINT, GREY + 7, GREY + 7,
	 250,	 400,	-250,
	 250,	 550,	-250,
	 250,	 550,	 250,
	 250,	 400,	 250,
  4, POLYGON, CHECK, MAXINT, GREY + 8, GREY + 8,
	 250,	 400,	 250,
	 250,	 550,	 250,
	-250,	 550,	 250,
	-250,	 400,	 250,
  4, POLYGON, CHECK, MAXINT, GREY + 7, GREY + 7,
	-250,	 400,	 250,
	-250,	 550,	 250,
	-250,	 550,	-250,
	-250,	 400,	-250,
  3, POLYGON, CHECK, MAXINT, RED + 4, RED + 5,
	-250,	 550,	-250,
	   0,	 600,	   0,
	 250,	 550,	-250,
  3, POLYGON, CHECK, MAXINT, RED + 3, RED + 5,
	 250,	 550,	-250,
	   0,	 600,	   0,
	 250,	 550,	 250,
  3, POLYGON, CHECK, MAXINT, RED + 4, RED + 5,
	 250,	 550,	 250,
	   0,	 600,	   0,
	-250,	 550,	 250,
  3, POLYGON, CHECK, MAXINT, RED + 3, RED + 5,
	-250,	 550,	 250,
	   0,	 600,	   0,
	-250,	 550,	-250,
  0
};


int hangar[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	   0,	   0,
  9, NONE, CONTACT, 350, 0, 0,
	-200,	 350,	   0,
	-150,	 350,	   0,
	-100,	 350,	   0,
	 -50,	 350,	   0,
	   0,	 350,	   0,
	  50,	 350,	   0,
	 100,	 350,	   0,
	 150,	 350,	   0,
	 200,	 350,	   0,
  6, POLYGON, CHECK, MAXINT, GREY + 10, GREY + 8,
	 700,	   0,	 300,
	 700,	 250,	 300,
	 400,	 350,	 300,
	-400,	 350,	 300,
	-700,	 250,	 300,
	-700,	   0,	 300,
  4, POLYGON, SURFACE, MAXINT, BROWN + 9, BROWN + 7,
	-350,	 250,	 300,
	-350,	   0,	 300,
	   0,	   0,	 300,
	   0,	 250,	 300,
  4, POLYGON, SURFACE, MAXINT, BROWN + 9, BROWN + 7,
	   0,	 250,	 300,
	   0,	   0,	 300,
	 350,	   0,	 300,
	 350,	 250,	 300,
  6, POLYGON, CHECK, MAXINT, GREY + 10, GREY + 8,
	-700,	   0,	-300,
	-700,	 250,	-300,
	-400,	 350,	-300,
	 400,	 350,	-300,
	 700,	 250,	-300,
	 700,	   0,	-300,
  4, POLYGON, CHECK, MAXINT, GREY + 9, GREY + 8,
	-700,	   0,	-300,
	-700,	   0,	 300,
	-700,	 250,	 300,
	-700,	 250,	-300,
  4, POLYGON, CHECK, MAXINT, BROWN + 7, BROWN + 7,
	-700,	 250,	-300,
	-700,	 250,	 300,
	-400,	 350,	 300,
	-400,	 350,	-300,
  4, POLYGON, CHECK, MAXINT, BROWN + 7, BROWN + 7,
	-400,	 350,	-300,
	-400,	 350,	 300,
	 400,	 350,	 300,
	 400,	 350,	-300,
  4, POLYGON, CHECK, MAXINT, BROWN + 7, BROWN + 7,
	 400,	 350,	-300,
	 400,	 350,	 300,
	 700,	 250,	 300,
	 700,	 250,	-300,
  4, POLYGON, CHECK, MAXINT, GREY + 9, GREY + 8,
	 700,	 250,	-300,
	 700,	 250,	 300,
	 700,	   0,	 300,
	 700,	   0,	-300,
  0
};


int heliPlace[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  4, POLYGON, NOCHECK, MAXINT, GREY + 10, GREY + 14,
	-440,	   0,	-440,
	-440,	   0,	 440,
	 440,	   0,	 440,
	 440,	   0,	-440,
  4, POLYGON, NOCHECK, 8000, RED + 13, RED + 13,
	-160,	   0,	 240,
	-160,	   0,	-240,
	 -80,	   0,	-240,
	 -80,	   0,	 240,
  4, POLYGON, NOCHECK, 8000, RED + 13, RED + 13,
	 160,	   0,	 240,
	 160,	   0,	-240,
	  80,	   0,	-240,
	  80,	   0,	 240,
  4, POLYGON, NOCHECK, 8000, RED + 13, RED + 13,
	  80,	   0,	  40,
	  80,	   0,	 -40,
	 -80,	   0,	 -40,
	 -80,	   0,	  40,
  0
};


int lake[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  13, POLYGON, NOCHECK, MAXINT, BLUE + 14, BLUE + 14,
	-600,	   0,	1400,
	 300,	   0,	1300,
	 800,	   0,	 800,
	 900,	   0,	   0,
	 600,	   0,	-400,
	 100,	   0,	-800,
	-300,	   0,	-900,
       -1000,	   0,	-900,
       -1700,	   0,	-700,
       -1900,	   0,	-500,
       -2000,	   0,	 200,
       -1800,	   0,	 700,
       -1500,	   0,	1100,
  6, POLYGON, NOCHECK, MAXINT, BLUE + 14, BLUE + 14,
	1000,	   0,	 800,
	1300,	   0,	 700,
	1100,	   0,	-100,
	1000,	   0,	-100,
	 900,	   0,	   0,
	 800,	   0,	 800,
  9, POLYGON, NOCHECK, MAXINT, BLUE + 14, BLUE + 14,
	1600,	   0,	 800,
	1900,	   0,	 600,
	2000,	   0,	   0,
	1900,	   0,	-500,
	1800,	   0,	-700,
	1600,	   0,	-700,
	1400,	   0,	-600,
	1100,	   0,	-100,
	1300,	   0,	 700,
  4, POLYGON, NOCHECK, 8000, BROWN + 10, BROWN + 9,
	 100,	   0,	-300,
	 400,	   0,	-600,
	 300,	   0,	-700,
	   0,	   0,	-400,
  0
};


int tempel[] =
{
  1, COMPLEX, MIDPOINT, 0, 0, 0,
	   0,	 200,	   0,
 14, NONE, CONTACT, 300, 0, 0,
	 600,	 550,	   0,
	 400,	 550,	   0,
	 200,	 550,	   0,
	   0,	 550,	   0,
	 200,	 550,	   0,
	 400,	 550,	   0,
	 600,	 550,	   0,
	 600,	 200,	   0,
	 400,	 200,	   0,
	 200,	 200,	   0,
	   0,	 200,	   0,
	 200,	 200,	   0,
	 400,	 200,	   0,
	 600,	 200,	   0,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 10, YELLOW + 9,
	 700,	   0,	 500,
	 700,	   0,	-500,
	-700,	   0,	-500,
	-700,	   0,	 500,
  4, TEXTURE, NOCHECK, MAXINT, 0, 20,
	 650,	   0,	 450,
	 650,	   0,	-450,
	-650,	   0,	-450,
	-650,	   0,	 450,
  4, POLYGON, CHECK, 8000, YELLOW + 10, YELLOW + 9,
	-600,	 400,	 400,
	-600,	 400,	-400,
	 600,	 400,	-400,
	 600,	 400,	 400,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 -40,	   0,	 300,
	 -40,	 400,	 300,
	  40,	 400,	 300,
	  40,	   0,	 300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	   0,	   0,	 340,
	   0,	 400,	 340,
	   0,	 400,	 260,
	   0,	   0,	 260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 -40,	   0,	-300,
	 -40,	 400,	-300,
	  40,	 400,	-300,
	  40,	   0,	-300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	   0,	   0,	-340,
	   0,	 400,	-340,
	   0,	 400,	-260,
	   0,	   0,	-260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 290,	   0,	 300,
	 290,	 400,	 300,
	 210,	 400,	 300,
	 210,	   0,	 300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	 250,	   0,	 340,
	 250,	 400,	 340,
	 250,	 400,	 260,
	 250,	   0,	 260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 290,	   0,	-300,
	 290,	 400,	-300,
	 210,	 400,	-300,
	 210,	   0,	-300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	 250,	   0,	-340,
	 250,	 400,	-340,
	 250,	 400,	-260,
	 250,	   0,	-260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 540,	   0,	 300,
	 540,	 400,	 300,
	 460,	 400,	 300,
	 460,	   0,	 300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	 500,	   0,	 340,
	 500,	 400,	 340,
	 500,	 400,	 260,
	 500,	   0,	 260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	 540,	   0,	-300,
	 540,	 400,	-300,
	 460,	 400,	-300,
	 460,	   0,	-300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	 500,	   0,	-340,
	 500,	 400,	-340,
	 500,	 400,	-260,
	 500,	   0,	-260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	-290,	   0,	 300,
	-290,	 400,	 300,
	-210,	 400,	 300,
	-210,	   0,	 300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	-250,	   0,	 340,
	-250,	 400,	 340,
	-250,	 400,	 260,
	-250,	   0,	 260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	-290,	   0,	-300,
	-290,	 400,	-300,
	-210,	 400,	-300,
	-210,	   0,	-300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	-250,	   0,	-340,
	-250,	 400,	-340,
	-250,	 400,	-260,
	-250,	   0,	-260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	-540,	   0,	 300,
	-540,	 400,	 300,
	-460,	 400,	 300,
	-460,	   0,	 300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	-500,	   0,	 340,
	-500,	 400,	 340,
	-500,	 400,	 260,
	-500,	   0,	 260,
  4, POLYGON, NOCHECK, MAXINT, YELLOW + 11, YELLOW + 11,
	-540,	   0,	-300,
	-540,	 400,	-300,
	-460,	 400,	-300,
	-460,	   0,	-300,
  4, POLYGON, NOCHECK, 4000, YELLOW + 11, YELLOW + 11,
	-500,	   0,	-340,
	-500,	 400,	-340,
	-500,	 400,	-260,
	-500,	   0,	-260,
  3, POLYGON, CHECK, MAXINT, YELLOW + 10, YELLOW + 9,
	 600,	 400,	-400,
	 600,	 600,	   0,
	 600,	 400,	 400,
  3, POLYGON, SURFACE, 4000, YELLOW + 11, YELLOW + 9,
	 600,	 420,	 310,
	 600,	 570,	   0,
	 600,	 420,	-310,
  3, POLYGON, CHECK, MAXINT, YELLOW + 10, YELLOW + 9,
	-600,	 400,	 400,
	-600,	 600,	   0,
	-600,	 400,	-400,
  3, POLYGON, SURFACE, 4000, YELLOW + 11, YELLOW + 9,
	-600,	 420,	-310,
	-600,	 570,	   0,
	-600,	 420,	 310,
  4, POLYGON, CHECK, MAXINT, YELLOW + 10, YELLOW + 9,
	-600,	 400,	-400,
	-600,	 600,	   0,
	 600,	 600,	   0,
	 600,	 400,	-400,
  4, POLYGON, CHECK, MAXINT, YELLOW + 10, YELLOW + 9,
	 600,	 400,	 400,
	 600,	 600,	   0,
	-600,	 600,	   0,
	-600,	 400,	 400,
  0
};


int eiffelTower[] =
{
  1, COMPLEX, MIDPOINT, 0, 0, 0,
	   0,	 200,	   0,
  3, NONE, CONTACT, 300, 0, 0,
	   0,	 200,	   0,
	   0,	 700,	   0,
	   0,	1200,	   0,
  4, POLYGON, NOCHECK, MAXINT, GREY + 6, GREY + 5,
	-700,	   0,	-700,
	-700,	   0,	 700,
	 700,	   0,	 700,
	 700,	   0,	-700,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	-200,
	 400,	   0,	 250,
	 400,	   0,	 400,
	 200,	 400,	 200,
	 400,	   0,	-250,
	 400,	   0,	-400,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	 200,
	 100,	 700,	-100,
	 200,	 400,	-200,
	 100,	 700,	 100,
	 200,	 400,	 200,
	 200,	 400,	-200,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 100,	 700,	 100,
	  50,	1200,	 -50,
	 100,	 700,	-100,
	  50,	1200,	  50,
	 100,	 700,	 100,
	 100,	 700,	-100,
  3, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	  50,	1200,	 -50,
	   0,	1700,	   0,
	  50,	1200,	  50,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	-200,	 400,	-200,
	-400,	   0,	 250,
	-400,	   0,	 400,
	-200,	 400,	 200,
	-400,	   0,	-250,
	-400,	   0,	-400,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	-200,	 400,	 200,
	-100,	 700,	-100,
	-200,	 400,	-200,
	-100,	 700,	 100,
	-200,	 400,	 200,
	-200,	 400,	-200,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	-100,	 700,	 100,
	- 50,	1200,	 -50,
	-100,	 700,	-100,
	- 50,	1200,	  50,
	-100,	 700,	 100,
	-100,	 700,	-100,
  3, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 -50,	1200,	 -50,
	   0,	1700,	   0,
	 -50,	1200,	  50,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	 200,
	-250,	   0,	 400,
	-400,	   0,	 400,
	-200,	 400,	 200,
	 250,	   0,	 400,
	 400,	   0,	 400,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	 200,
	-100,	 700,	 100,
	-200,	 400,	 200,
	 100,	 700,	 100,
	 200,	 400,	 200,
	-200,	 400,	 200,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 100,	 700,	 100,
	 -50,	1200,	  50,
	-100,	 700,	 100,
	  50,	1200,	  50,
	 100,	 700,	 100,
	-100,	 700,	 100,
  3, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	  50,	1200,	  50,
	   0,	1700,	   0,
	 -50,	1200,	  50,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	-200,
	-250,	   0,	-400,
	-400,	   0,	-400,
	-200,	 400,	-200,
	 250,	   0,	-400,
	 400,	   0,	-400,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 200,	 400,	-200,
	-100,	 700,	-100,
	-200,	 400,	-200,
	 100,	 700,	-100,
	 200,	 400,	-200,
	-200,	 400,	-200,
  6, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	 100,	 700,	-100,
	 -50,	1200,	 -50,
	-100,	 700,	-100,
	  50,	1200,	 -50,
	 100,	 700,	-100,
	-100,	 700,	-100,
  3, POLYLINE, NOCHECK, MAXINT, METALLICFILL + GREY, 0,
	  50,	1200,	 -50,
	   0,	1700,	   0,
	 -50,	1200,	 -50,
  1, CIRCLE, NOCHECK, MAXINT, FLASH, 50,
	   0,	1700,	   0,
  0
};


int runway[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  4, POLYGON, NOCHECK, MAXINT, GREY + 4, GREY + 7,
	-300,	   0,  -2000,
	-300,	   0,	2000,
	 300,	   0,	2000,
	 300,	   0,  -2000,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	-270,	   0,  -1900,
	-270,	   0,  -1400,
	-230,	   0,  -1400,
	-230,	   0,  -1900,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	-160,	   0,  -1900,
	-160,	   0,  -1400,
	-120,	   0,  -1400,
	-120,	   0,  -1900,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 270,	   0,  -1900,
	 270,	   0,  -1400,
	 230,	   0,  -1400,
	 230,	   0,  -1900,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 160,	   0,  -1900,
	 160,	   0,  -1400,
	 120,	   0,  -1400,
	 120,	   0,  -1900,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 0, 0,
	 -50,	   0,  -1900,
	  50,	   0,  -1900,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 1, 0,
	 -50,	   0,  -1800,
	  50,	   0,  -1800,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 2, 0,
	 -50,	   0,  -1700,
	  50,	   0,  -1700,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 3, 0,
	 -50,	   0,  -1600,
	  50,	   0,  -1600,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 4, 0,
	 -50,	   0,  -1500,
	  50,	   0,  -1500,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 5, 0,
	 -50,	   0,  -1400,
	  50,	   0,  -1400,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 6, 0,
	 -50,	   0,  -1300,
	  50,	   0,  -1300,
  2, PIXEL, NOCHECK, MAXINT, FLASH + 7, 0,
	 -50,	   0,  -1200,
	  50,	   0,  -1200,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 -10,	   0,  -1000,
	  10,	   0,  -1000,
	  10,	   0,	-600,
	 -10,	   0,	-600,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 -10,	   0,	-400,
	  10,	   0,	-400,
	  10,	   0,	   0,
	 -10,	   0,	   0,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 -10,	   0,	 200,
	  10,	   0,	 200,
	  10,	   0,	 600,
	 -10,	   0,	 600,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 -10,	   0,	 800,
	  10,	   0,	 800,
	  10,	   0,	1200,
	 -10,	   0,	1200,
  4, POLYGON, NOCHECK, 6000, GREY + 15, GREY + 15,
	 -10,	   0,	1400,
	  10,	   0,	1400,
	  10,	   0,	1800,
	 -10,	   0,	1800,
  0
};


int street[] =
{
  1, VERYSPARSE, GROUND, 0, 0, 0,
	   0,	   0,	   0,
  4, POLYGON, NOCHECK, MAXINT, GREY + 5, GREY + 5,
	-300,	   0,  -2000,
	-300,	   0,	2000,
	 300,	   0,	2000,
	 300,	   0,  -2000,
  4, POLYGON, NOCHECK, MAXINT, GREY + 14, GREY + 14,
	 -10,	   0,  -1800,
	  10,	   0,  -1800,
	  10,	   0,  -1400,
	 -10,	   0,  -1400,
  4, POLYGON, NOCHECK, MAXINT, GREY + 14, GREY + 14,
	 -10,	   0,  -1000,
	  10,	   0,  -1000,
	  10,	   0,	-600,
	 -10,	   0,	-600,
  4, POLYGON, NOCHECK, MAXINT, GREY + 14, GREY + 14,
	 -10,	   0,	-200,
	  10,	   0,	-200,
	  10,	   0,	 200,
	 -10,	   0,	 200,
  4, POLYGON, NOCHECK, MAXINT, GREY + 14, GREY + 14,
	 -10,	   0,	 600,
	  10,	   0,	 600,
	  10,	   0,	1000,
	 -10,	   0,	1000,
  4, POLYGON, NOCHECK, MAXINT, GREY + 14, GREY + 14,
	 -10,	   0,	1400,
	  10,	   0,	1400,
	  10,	   0,	1800,
	 -10,	   0,	1800,
  0
};


int mast[] =
{
  1, SPARSE, MIDPOINT, 0, 0, 0,
	   0,	 100,	   0,
  2, NONE, CONTACT, 300, 0, 0,
	   0,	 100,	   0,
	   0,	 400,	   0,
  4, POLYGON, NOCHECK, MAXINT, GREY + 8, GREY + 8,
	 -70,	 400,	   0,
	  70,	 400,	   0,
	  70,	 410,	   0,
	 -70,	 410,	   0,
  4, POLYGON, NOCHECK, MAXINT, BROWN + 8, BROWN + 8,
	 -15,	   0,	   0,
	  15,	   0,	   0,
	  15,	 450,	   0,
	 -15,	 450,	   0,
  4, POLYGON, NOCHECK, MAXINT, BROWN + 8, BROWN + 8,
	   0,	   0,	 -15,
	   0,	   0,	  15,
	   0,	 450,	  15,
	   0,	 450,	 -15,
  0
};

int wire[] =
{
  1, SPARSE, MIDPOINT, 0, 0, 0,
	   0,	 400,	 400,
  3, NONE, CONTACT, 200, 0, 0,
	   0,	 400,	 400,
	   0,	 400,	1000,
	   0,	 400,	1600,
 10, POLYLINE, NOCHECK, MAXINT, GREY + 4, GREY + 4,
	 -65,	 400,	   0,
	 -65,	 325,	 500,
	 -65,	 300,	1000,
	 -65,	 325,	1500,
	 -65,	 400,	2000,
	  65,	 400,	2000,
	  65,	 325,	1500,
	  65,	 300,	1000,
	  65,	 325,	 500,
	  65,	 400,	   0,
  0
};

int workObject[20000];

#define pran random (4000) - 2000
#define wran random (3600)
#define aran random (AREAMAPSIZE - 1) + 1

void randomTreeArea (int xArea, int zArea, int count, int complexity)
{
  assignArea (xArea, zArea, count * 3);

  while (count--)
  {
    createTree (workObject, random (300) + 300, random (8) + 4);
    placeObject (workObject, pran, 0, pran, 0, wran, 0, complexity);
    placeObject (workObject, pran, 0, pran, 0, wran, 0, complexity);
    placeObject (workObject, pran, 0, pran, 0, wran, 0, complexity);
  }
}


void main (void)
{
  int i;

  // Scenery eroeffnen
  createDatabase ("ikarus.scn");

  // Titelbild und Menuhintergrund
  titlePicture ("bitmaps\\title.bmp", 10);
  menuBackground ("bitmaps\\menu.bmp", 0);

  // Farbpalette
  colorPalette ();

  // Texturen
  texture ("bitmaps\\david1.bmp",  10);
  texture ("bitmaps\\teppich.bmp",  20);

  // Landeplatz mit Hangar usw.
  assignArea (30, 30, 9);
  placeObject (hangar, -1200, 0, 800, 0, 900, 0, SPARSE);
  placeObject (heliPlace, 0, 0, 500, 0, 900, 0, VERYSPARSE);
  placeObject (heliPlace, 0, 0, 1500, 0, 900, 0, VERYSPARSE);
  placeObject (heliPlace, 1000, 0, 1500, 0, 900, 0, VERYSPARSE);
  placeObject (tower, 100, 0, -500, 0, 0, 0, VERYSPARSE);
  createHouseB (workObject, 500, 400, 300);
  placeObject (workObject, 1100, 0,-1100, 0, 3000, 0, SPARSE);
  createHouseA (workObject, 500, 300, 300);
  placeObject (workObject, 0, 0,-1100, 0, 0, 0, SPARSE);
  placeObject (workObject, -900, 0,-1100, 0, 0, 0, SPARSE);
  startPoint (0, 0, 500, 0, 900, 0);
  startPoint (0, 0, 1500, 0, 900, 0);
  startPoint (1000, 0, 1500, 0, 0, 0);
  viewPoint (0, 500, -500);
  dynamicObject (workObject, WINDBAG);
  placeObject (workObject, 1700, 0, 1000, 0, 0, 0, VERYSPARSE);

  // david
  assignArea (34, 34, 1);
  createPoster (workObject, 10);
  placeObject (workObject, 0, 0, 0, 0, 1500, 0, COMPLEX);

  // tempel
  assignArea (35, 35, 1);
  placeObject (tempel, 0, 0, 0, 0, 2300, 0, SPARSE);
  viewPoint (1000, 200, 1300);

  // turm
  assignArea (30, 28, 1);
  placeObject (eiffelTower, 0, 0, 0, 0, 2300, 0, SPARSE);

  // Flugplatz mit Tower
  assignArea (35, 25, 2);
  placeObject (runway, 0, 0, 0, 0, 0, 0, VERYSPARSE);
  placeObject (tower, 1000, 0, 1300, 0, 0, 0, VERYSPARSE);
  viewPoint (1000, 400, 1300);

  // Strasse mit Leitung
  assignArea (32, 0, 5);
  placeObject (street, 0, 0, 0, 0, 0, 0, SPARSE);
  placeObject (mast, -550, 0, 0, 0, 0, 0, COMPLEX);
  placeObject (wire, -550, 0, 0, 0, 0, 0, COMPLEX);
  placeObject (mast, -550, 0, -2000, 0, 0, 0, COMPLEX);
  placeObject (wire, -550, 0, -2000, 0, 0, 0, COMPLEX);

  // Strasse kopieren
  for (i = 1; i <= 63; i++)
  {
    copyArea (32, 0, 32, i);
    if ((i % 10) == 0)
      viewPoint (-1000, 200, 0);
    if ((i % 10) == 5)
      viewPoint (1000, 200, 0);
  }

  // See mit Haus
  assignArea (34, 32, 4);
  placeObject (lake, 0, 0, 0, 0, 0, 0, VERYSPARSE);
  createTree (workObject, 500, 12);
  placeObject (workObject, 900, 0, -300, 0, 900, 0, SPARSE);
  createTree (workObject, 300, 10);
  placeObject (workObject, 1200, 0, -500, 0, 900, 0, VERYSPARSE);
  createHouseA (workObject, 500, 300, 250);
  placeObject (workObject, 700, 0, -900, 0, 450, 0, VERYSPARSE);
  dynamicObject (workObject, WINDBAG);
  placeObject (workObject, 1700, 0, 1700, 0, 0, 0, VERYSPARSE);
  viewPoint (-1800, 200, -1800);

  // ein kleiner Berg
  assignArea (29, 31, 1);
  createHill (workObject, random (1000) + 500, GREY);
  placeObject (workObject, 0, 0, 0, 0, wran, 0, VERYSPARSE);

  // und noch ein paar Berge
  for (i = 0; i < 50; i++)
  {
    assignArea (aran, aran, 1);

    switch (random (3))
    {
      case 0 : createHill (workObject, random (1000) + 500, GREY);   break;
      case 1 : createHill (workObject, random (1000) + 500, YELLOW); break;
      case 2 : createHill (workObject, random (1000) + 500, GREEN);  break;
    }

    switch (random (3))
    {
      case 0 : placeObject (workObject, 0, 0, 0, 0, wran, 0, SPARSE);
      case 1 : placeObject (workObject, 0, 0, 0, 0, wran, 0, COMPLEX);
      case 2 : placeObject (workObject, 0, 0, 0, 0, wran, 0, VERYCOMPLEX);
    }
  }


  // Baume zufaellig platzieren
  randomTreeArea (0, 0, 8, SPARSE);
  randomTreeArea (1, 0, 8, SPARSE);
  randomTreeArea (2, 0, 8, COMPLEX);
  randomTreeArea (3, 0, 8, VERYCOMPLEX);

  for (i = 0; i < 1000; i++)
  {
    copyArea (0, 0, aran, aran);
    copyArea (1, 0, aran, aran);
    copyArea (2, 0, aran, aran);
    copyArea (3, 0, aran, aran);
  }

  // Bodenfarbe
  setGroundColor (YELLOW + 8);
  setComplexity (COMPLEX);

  // Scenery abschliessen
  closeDatabase ();
}

