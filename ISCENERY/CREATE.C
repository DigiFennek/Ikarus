//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*
//*
//***************************************************************************

#include <stdlib.h>
#include "ikarus2.h"
#include "geometry.h"

int *object = NULL;
int *size = NULL;


void createObject3d (int *workSpace, int complexity, int x, int y, int z)
{
  object = workSpace;

  size = NULL;
  *object++ = 1;
  *object++ = complexity;
  *object++ = MIDPOINT;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = x;
  *object++ = y;
  *object++ = z;
}


void createObject2d (int *workSpace, int complexity)
{
  object = workSpace;

  size = NULL;
  *object++ = 1;
  *object++ = complexity;
  *object++ = GROUND;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
}


int *closeObject (void)
{
  *object++ = 0;
  return object;
}


void createPolygon (int control, int viewDist,
		    int fillColor, int borderColor)
{
  size = object;
  *object++ = 0;
  *object++ = POLYGON;
  *object++ = control;
  *object++ = viewDist;
  *object++ = fillColor;
  *object++ = borderColor;
}


void createTexture (int control, int viewDist, int texture)
{
  size = object;
  *object++ = 0;
  *object++ = TEXTURE;
  *object++ = control;
  *object++ = viewDist;
  *object++ = 0;
  *object++ = texture;
}


void createContact (int radius)
{
  size = object;
  *object++ = 0;
  *object++ = NONE;
  *object++ = CONTACT;
  *object++ = radius;
  *object++ = 0;
  *object++ = 0;
}


void createPolyLine (int control, int viewDist, int color)
{
  size = object;
  *object++ = 0;
  *object++ = POLYLINE;
  *object++ = control;
  *object++ = viewDist;
  *object++ = color;
  *object++ = 0;
}


void createCircle (int control, int viewDist, int diameter, int color)
{
  size = object;
  *object++ = 0;
  *object++ = CIRCLE;
  *object++ = control;
  *object++ = viewDist;
  *object++ = color;
  *object++ = diameter;
}


void createPixel (int control, int viewDist, int color)
{
  size = object;
  *object++ = 0;
  *object++ = PIXEL;
  *object++ = control;
  *object++ = viewDist;
  *object++ = color;
  *object++ = 0;
}


void addPoint (int x, int y, int z)
{
  *object++ = x;
  *object++ = y;
  *object++ = z;
  (*size)++;
}


int *createHouseA (int *object, int lenght, int wide, int hight)
{
  int hight2 = hight * 2 / 3;
  lenght /= 2; wide /= 2;

  createObject3d (object, SPARSE, 0, hight / 2, 0);
    // Beruehrungspunkte
    createContact (wide);
      addPoint (-lenght / 2, hight, -wide / 2);
      addPoint (-lenght / 2, hight,  wide / 2);
      addPoint ( lenght / 2, hight,  wide / 2);
      addPoint ( lenght / 2, hight, -wide / 2);
    // Wand
    createPolygon (CHECK, MAXDIST, GREY + 10, GREY + 10);
      addPoint (-lenght,      0, -wide);
      addPoint (-lenght, hight2, -wide);
      addPoint ( lenght, hight2, -wide);
      addPoint ( lenght,      0, -wide);
    createPolygon (CHECK, MAXDIST, GREY + 8, GREY + 8);
      addPoint ( lenght,      0,  wide);
      addPoint ( lenght, hight2,  wide);
      addPoint (-lenght, hight2,  wide);
      addPoint (-lenght,      0,  wide);
    createPolygon (CHECK, MAXDIST, GREY + 9, GREY + 9);
      addPoint (-lenght,      0,  wide);
      addPoint (-lenght, hight2,  wide);
      addPoint (-lenght,  hight,     0);
      addPoint (-lenght, hight2, -wide);
      addPoint (-lenght,      0, -wide);
    createPolygon (CHECK, MAXDIST, GREY + 9, GREY + 9);
      addPoint ( lenght,      0, -wide);
      addPoint ( lenght, hight2, -wide);
      addPoint ( lenght,  hight,     0);
      addPoint ( lenght, hight2,  wide);
      addPoint ( lenght,      0,  wide);
    // Dach
    createPolygon (CHECK, MAXDIST, RED + 7, RED + 6);
      addPoint (-lenght, hight2, -wide);
      addPoint (-lenght,  hight,     0);
      addPoint ( lenght,  hight,     0);
      addPoint ( lenght, hight2, -wide);
    createPolygon (CHECK, MAXDIST, RED + 8, RED + 6);
      addPoint ( lenght, hight2,  wide);
      addPoint ( lenght,  hight,     0);
      addPoint (-lenght,  hight,     0);
      addPoint (-lenght, hight2,  wide);

  return closeObject ();
}

int *createHouseB (int *object, int lenght, int wide, int hight)
{
  int hight2 = hight * 2 / 3;
  lenght /= 2; wide /= 2;

  createObject3d (object, SPARSE, 0, hight / 2, 0);
    // Beruehrungspunkte
    createContact (300);
      addPoint (-lenght / 2, hight2, -wide / 2);
      addPoint (-lenght / 2, hight2,  wide / 2);
      addPoint ( lenght / 2, hight2,  wide / 2);
      addPoint ( lenght / 2, hight2, -wide / 2);
    // Wand
    createPolygon (CHECK, MAXDIST, GREY + 10, GREY + 10);
      addPoint (-lenght,      0, -wide);
      addPoint (-lenght, hight2, -wide);
      addPoint ( lenght, hight2, -wide);
      addPoint ( lenght,      0, -wide);
    createPolygon (CHECK, MAXDIST, GREY + 8, GREY + 8);
      addPoint ( lenght,      0,  wide);
      addPoint ( lenght, hight2,  wide);
      addPoint (-lenght, hight2,  wide);
      addPoint (-lenght,      0,  wide);
    createPolygon (CHECK, MAXDIST, GREY + 9, GREY + 9);
      addPoint (-lenght,      0,  wide);
      addPoint (-lenght, hight2,  wide);
      addPoint (-lenght, hight2, -wide);
      addPoint (-lenght,      0, -wide);
    createPolygon (CHECK, MAXDIST, GREY + 9, GREY + 9);
      addPoint ( lenght,      0, -wide);
      addPoint ( lenght, hight2, -wide);
      addPoint ( lenght, hight2,  wide);
      addPoint ( lenght,      0,  wide);
    // Dach
    createPolygon (CHECK, MAXDIST, RED + 7, RED + 6);
      addPoint (-lenght, hight2, -wide);
      addPoint (      0,  hight,     0);
      addPoint ( lenght, hight2, -wide);
    createPolygon (CHECK, MAXDIST, RED + 7, RED + 6);
      addPoint ( lenght, hight2,  wide);
      addPoint (      0,  hight,     0);
      addPoint (-lenght, hight2,  wide);
    createPolygon (CHECK, MAXDIST, RED + 8, RED + 6);
      addPoint (-lenght, hight2,  wide);
      addPoint (      0,  hight,     0);
      addPoint (-lenght, hight2, -wide);
    createPolygon (CHECK, MAXDIST, RED + 8, RED + 6);
      addPoint ( lenght, hight2, -wide);
      addPoint (      0,  hight,     0);
      addPoint ( lenght, hight2,  wide);

  return closeObject ();
}


int *createTree (int *object, int hight, int shade)
{
  int trunkWide = hight / 20;
  int trunkHight = hight / 5;
  int treeWide = hight / 5;

  createObject3d (object, SPARSE, 0, hight / 2, 0);
    createContact (300);
      addPoint (0, hight / 2, 0);
      addPoint (0, hight, 0);
    createPolygon (NOCHECK, CIRCLEDIST, BROWN + shade, BROWN + shade);
      addPoint (-trunkWide, 0, 0);
      addPoint (-trunkWide, trunkHight, 0);
      addPoint (trunkWide, trunkHight, 0);
      addPoint (trunkWide, 0, 0);
    createPolygon (NOCHECK, 4000, BROWN + shade, BROWN + shade);
      addPoint (0, 0, -trunkWide);
      addPoint (0, trunkHight, -trunkWide);
      addPoint (0, trunkHight, trunkWide);
      addPoint (0, 0, trunkWide);
    createPolygon (NOCHECK, 3000, GREEN + shade, GREEN + shade);
      addPoint (-treeWide, trunkHight, 0);
      addPoint (0, trunkHight, treeWide);
      addPoint (treeWide, trunkHight, 0);
      addPoint (0, trunkHight, -treeWide);
    createPolygon (NOCHECK, CIRCLEDIST, GREEN + shade, GREEN + shade);
      addPoint (treeWide, trunkHight, 0);
      addPoint (0, hight, 0);
      addPoint (-treeWide, trunkHight, 0);
    createPolygon (NOCHECK, 4000, GREEN + shade, GREEN + shade);
      addPoint (0, trunkHight, treeWide);
      addPoint (0, hight, 0);
      addPoint (0, trunkHight, -treeWide);
  return closeObject ();
}


int *createHill (int *object, int hight, int color)
{
  int x[5][5], y[5][5], z[5][5];
  int c[28], i, k;

  for (i = 0; i <= 4; i++)
    for (k = 0; k <= 4; k++)
    {
      x[i][k] = random (200) + (i * 500) - 1100;
      y[i][k] = 0;
      z[i][k] = random (200) + (k * 500) - 1100;
    }

  for (i = 0; i <= 27; i++)
    c[i] = random (5) + 6 + color;

  y[1][1] = random (hight / 4) + (hight * 2) / 3;
  y[1][3] = random (hight / 4) + (hight * 2) / 3;
  y[3][1] = random (hight / 4) + (hight * 2) / 3;
  y[3][3] = random (hight / 4) + (hight * 2) / 3;
  y[2][1] = random (hight / 3) + (hight * 2) / 3;
  y[1][2] = random (hight / 3) + (hight * 2) / 3;
  y[2][3] = random (hight / 3) + (hight * 2) / 3;
  y[3][2] = random (hight / 3) + (hight * 2) / 3;
  y[2][2] = hight;

  createObject3d (object, SPARSE, 0, hight / 2, 0);
    createContact (800);
      for (i = 0; i <= 4; i++)
	for (k = 0; k <= 4; k++)
	  addPoint (x[i][k], y[i][k], z[i][k]);
    createPolygon (CHECK, MAXDIST, c[0], c[0] + 1);
      addPoint (x[0][1], y[0][1], z[0][1]);
      addPoint (x[1][1], y[1][1], z[1][1]);
      addPoint (x[1][0], y[1][0], z[1][0]);
    createPolygon (CHECK, MAXDIST, c[1], c[1] + 1);
      addPoint (x[1][0], y[1][0], z[1][0]);
      addPoint (x[1][1], y[1][1], z[1][1]);
      addPoint (x[2][0], y[2][0], z[2][0]);
    createPolygon (CHECK, MAXDIST, c[2], c[2] + 1);
      addPoint (x[2][0], y[2][0], z[2][0]);
      addPoint (x[1][1], y[1][1], z[1][1]);
      addPoint (x[2][1], y[2][1], z[2][1]);
    createPolygon (CHECK, MAXDIST, c[3], c[3] + 1);
      addPoint (x[2][0], y[2][0], z[2][0]);
      addPoint (x[2][1], y[2][1], z[2][1]);
      addPoint (x[3][1], y[3][1], z[3][1]);
    createPolygon (CHECK, MAXDIST, c[4], c[4] + 1);
      addPoint (x[2][0], y[2][0], z[2][0]);
      addPoint (x[3][1], y[3][1], z[3][1]);
      addPoint (x[3][0], y[3][0], z[3][0]);
    createPolygon (CHECK, MAXDIST, c[5], c[5] + 1);
      addPoint (x[3][0], y[3][0], z[3][0]);
      addPoint (x[3][1], y[3][1], z[3][1]);
      addPoint (x[4][1], y[4][1], z[4][1]);
    createPolygon (CHECK, MAXDIST, c[6], c[6] + 1);
      addPoint (x[0][1], y[0][1], z[0][1]);
      addPoint (x[0][2], y[0][2], z[0][2]);
      addPoint (x[1][1], y[1][1], z[1][1]);
    createPolygon (CHECK, MAXDIST, c[7], c[7] + 1);
      addPoint (x[1][1], y[1][1], z[1][1]);
      addPoint (x[0][2], y[0][2], z[0][2]);
      addPoint (x[1][2], y[1][2], z[1][2]);
    createPolygon (CHECK, MAXDIST, c[8], c[8] + 1);
      addPoint (x[1][1], y[1][1], z[1][1]);
      addPoint (x[1][2], y[1][2], z[1][2]);
      addPoint (x[2][1], y[2][1], z[2][1]);
    createPolygon (CHECK, MAXDIST, c[9], c[9] + 1);
      addPoint (x[2][1], y[2][1], z[2][1]);
      addPoint (x[1][2], y[1][2], z[1][2]);
      addPoint (x[2][2], y[2][2], z[2][2]);
    createPolygon (CHECK, MAXDIST, c[10], c[10] + 1);
      addPoint (x[2][1], y[2][1], z[2][1]);
      addPoint (x[2][2], y[2][2], z[2][2]);
      addPoint (x[3][2], y[3][2], z[3][2]);
    createPolygon (CHECK, MAXDIST, c[11], c[11] + 1);
      addPoint (x[2][1], y[2][1], z[2][1]);
      addPoint (x[3][2], y[3][2], z[3][2]);
      addPoint (x[3][1], y[3][1], z[3][1]);
    createPolygon (CHECK, MAXDIST, c[12], c[12] + 1);
      addPoint (x[3][1], y[3][1], z[3][1]);
      addPoint (x[3][2], y[3][2], z[3][2]);
      addPoint (x[4][2], y[4][2], z[4][2]);
    createPolygon (CHECK, MAXDIST, c[13], c[13] + 1);
      addPoint (x[3][1], y[3][1], z[3][1]);
      addPoint (x[4][2], y[4][2], z[4][2]);
      addPoint (x[4][1], y[4][1], z[4][1]);
    createPolygon (CHECK, MAXDIST, c[14], c[14] + 1);
      addPoint (x[0][2], y[0][2], z[0][2]);
      addPoint (x[0][3], y[0][3], z[0][3]);
      addPoint (x[1][3], y[1][3], z[1][3]);
    createPolygon (CHECK, MAXDIST, c[15], c[15] + 1);
      addPoint (x[0][2], y[0][2], z[0][2]);
      addPoint (x[1][3], y[1][3], z[1][3]);
      addPoint (x[1][2], y[1][2], z[1][2]);
    createPolygon (CHECK, MAXDIST, c[16], c[16] + 1);
      addPoint (x[1][2], y[1][2], z[1][2]);
      addPoint (x[1][3], y[1][3], z[1][3]);
      addPoint (x[2][3], y[2][3], z[2][3]);
    createPolygon (CHECK, MAXDIST, c[17], c[17] + 1);
      addPoint (x[1][2], y[1][2], z[1][2]);
      addPoint (x[2][3], y[2][3], z[2][3]);
      addPoint (x[2][2], y[2][2], z[2][2]);
    createPolygon (CHECK, MAXDIST, c[18], c[18] + 1);
      addPoint (x[2][2], y[2][2], z[2][2]);
      addPoint (x[2][3], y[2][3], z[2][3]);
      addPoint (x[3][2], y[3][2], z[3][2]);
    createPolygon (CHECK, MAXDIST, c[19], c[19] + 1);
      addPoint (x[3][2], y[3][2], z[3][2]);
      addPoint (x[2][3], y[2][3], z[2][3]);
      addPoint (x[3][3], y[3][3], z[3][3]);
    createPolygon (CHECK, MAXDIST, c[20], c[20] + 1);
      addPoint (x[3][2], y[3][2], z[3][2]);
      addPoint (x[3][3], y[3][3], z[3][3]);
      addPoint (x[4][2], y[4][2], z[4][2]);
    createPolygon (CHECK, MAXDIST, c[21], c[21] + 1);
      addPoint (x[3][3], y[3][3], z[3][3]);
      addPoint (x[4][3], y[4][3], z[4][3]);
      addPoint (x[4][2], y[4][2], z[4][2]);
    createPolygon (CHECK, MAXDIST, c[22], c[22] + 1);
      addPoint (x[0][3], y[0][3], z[0][3]);
      addPoint (x[1][4], y[1][4], z[1][4]);
      addPoint (x[1][3], y[1][3], z[1][3]);
    createPolygon (CHECK, MAXDIST, c[23], c[23] + 1);
      addPoint (x[1][3], y[1][3], z[1][3]);
      addPoint (x[1][4], y[1][4], z[1][4]);
      addPoint (x[2][4], y[2][4], z[2][4]);
    createPolygon (CHECK, MAXDIST, c[24], c[24] + 1);
      addPoint (x[1][3], y[1][3], z[1][3]);
      addPoint (x[2][4], y[2][4], z[2][4]);
      addPoint (x[2][3], y[2][3], z[2][3]);
    createPolygon (CHECK, MAXDIST, c[25], c[25] + 1);
      addPoint (x[2][3], y[2][3], z[2][3]);
      addPoint (x[2][4], y[2][4], z[2][4]);
      addPoint (x[3][3], y[3][3], z[3][3]);
    createPolygon (CHECK, MAXDIST, c[26], c[26] + 1);
      addPoint (x[3][3], y[3][3], z[3][3]);
      addPoint (x[2][4], y[2][4], z[2][4]);
      addPoint (x[3][4], y[3][4], z[3][4]);
    createPolygon (CHECK, MAXDIST, c[27], c[27] + 1);
      addPoint (x[3][3], y[3][3], z[3][3]);
      addPoint (x[3][4], y[3][4], z[3][4]);
      addPoint (x[4][3], y[4][3], z[4][3]);
  return closeObject ();
}


int *createPoster (int *object, int texture)
{
  createObject3d (object, SPARSE, 0, 200, 0);
    createContact (200);
      addPoint (-200, 200, 0);
      addPoint ( 200, 200, 0);
    createTexture (NOCHECK, CIRCLEDIST, texture);
      addPoint (-300, 1000, 0);
      addPoint (-300, 0, 0);
      addPoint ( 300, 0, 0);
      addPoint ( 300, 1000, 0);
  return closeObject ();
}


