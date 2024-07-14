//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Texture Mapping
//*
//***************************************************************************

#include "ikarus2.h"
#include <dos.h>
#include <alloc.h>
#include <stdlib.h>

extern struct POINT minClip;
extern struct POINT maxClip;

extern char far *scrBuffer;

long slope (long x1, long y1, long x2, long y2)
{
  long div;

  if ((div = (y2 - y1) + 1) != 0)
    return ((x2 - x1)) / div;
  else
    return 0;
}

void scanMap (long x1, long x2, int y, char *map, int dxMap,
	      long mapXs1, long mapYs1, long mapXs2, long mapYs2)
{
  long temp, x;

  x1 >>= 16;
  x2 >>= 16;

  if (x1 > x2)
  {
    temp = x1; x1 = x2; x2 = temp;
    temp = mapXs1; mapXs1 = mapXs2; mapXs2 = temp;
    temp = mapYs1; mapYs1 = mapYs2; mapYs2 = temp;
  }

  {
    char far *scr;
    int color;

    long mapSlopeX = slope (mapXs1, x1, mapXs2, x2);
    long mapSlopeY = slope (mapYs1, x1, mapYs2, x2);

    if (x1 < minClip.x)
    {
      int dx = minClip.x - x1;
      mapXs1 += mapSlopeX * dx;
      mapYs1 += mapSlopeY * dx;
      x1 = minClip.x;
    }

    x2 = min (x2, maxClip.x);

    scr = scrBuffer + 320 * y + x1;

    for (x = x1; x <= x2; x++)
    {
      int mapXint = (mapXs1 += mapSlopeX) >> 16;
      int mapYint = (mapYs1 += mapSlopeY) >> 16;

      if ((color = *(map + (mapYint * dxMap) + mapXint)) != 0)
	*scr = color;

      scr++;
    }
  }
}


void mapPoly (struct POINT poly[], unsigned char far *map)
{
  int nextEdge2 = 0, nextEdge1 = 0;
  int i, y1, y2;
  int dxMap = *map++;
  int dyMap = *map++;
  long mapX[4], mapY[4];

  long scrPos1, scrSlope1;
  long scrPos2, scrSlope2;

  long mapPos1x, mapSlope1x;
  long mapPos1y, mapSlope1y;
  long mapPos2x, mapSlope2x;
  long mapPos2y, mapSlope2y;

  mapX[0] = 0;
  mapY[0] = 0;
  mapX[1] = 0;
  mapY[1] = (long) dyMap << 16;
  mapX[2] = (long) dxMap << 16;
  mapY[2] = (long) dyMap << 16;
  mapX[3] = (long) dxMap << 16;
  mapY[3] = 0;

  for (i = 1; i <= 3; i++)
    if (poly[i].y < poly[nextEdge1].y) nextEdge1 = nextEdge2 = i;

  y1 = y2 = poly[nextEdge1].y;

  for (i = 0; i <= 3; i++)
  {
    if (y2 == poly[nextEdge1].y)
    {
      int activeEdge = nextEdge1;
      nextEdge1 = (nextEdge1 == 3) ? 0 : nextEdge1 + 1;

      scrPos1 = (long)poly[activeEdge].x << 16;

      scrSlope1 = slope (scrPos1, poly[activeEdge].y,
	(long)poly[nextEdge1].x << 16, poly[nextEdge1].y);

      mapPos1x = mapX[activeEdge];
      mapPos1y = mapY[activeEdge];

      mapSlope1x = slope (mapPos1x, poly[activeEdge].y,
	mapX[nextEdge1], poly[nextEdge1].y);

      mapSlope1y = slope (mapPos1y, poly[activeEdge].y,
	mapY[nextEdge1], poly[nextEdge1].y);

      y1 = poly[activeEdge].y;

      if (y1 < minClip.y)
      {
	long dy = minClip.y - y1;
	scrPos1 += scrSlope1 * dy;
	mapPos1x += mapSlope1x * dy;
	mapPos1y += mapSlope1y * dy;
	y1 = minClip.y;
      }
    }

    if (y2 == poly[nextEdge2].y)
    {
      int activeEdge = nextEdge2;

      nextEdge2 = (nextEdge2 == 0) ? 3 : nextEdge2 - 1;

      scrPos2 = (long)poly[activeEdge].x << 16;

      scrSlope2 = slope (scrPos2, poly[activeEdge].y,
	(long)poly[nextEdge2].x << 16, poly[nextEdge2].y);

      mapPos2x = mapX[activeEdge];
      mapPos2y = mapY[activeEdge];

      mapSlope2x = slope (mapPos2x, poly[activeEdge].y,
	mapX[nextEdge2], poly[nextEdge2].y);

      mapSlope2y = slope (mapPos2y, poly[activeEdge].y,
	mapY[nextEdge2], poly[nextEdge2].y);

      y1 = poly[activeEdge].y;

      if (y1 < minClip.y)
      {
	long dy = minClip.y - y1;
	scrPos2 += scrSlope2 * dy;
	mapPos2x += mapSlope2x * dy;
	mapPos2y += mapSlope2y * dy;
	y1 = minClip.y;
      }
    }

    y2 = min (poly[nextEdge1].y, poly[nextEdge2].y);
    y2 = min (y2, maxClip.y);

    while (y1 < y2)
    {
      scrPos1 += scrSlope1;
      scrPos2 += scrSlope2;

      mapPos1x += mapSlope1x;	mapPos1y += mapSlope1y;
      mapPos2x += mapSlope2x;	mapPos2y += mapSlope2y;

      scanMap (scrPos1, scrPos2, y1, map, dxMap,
	mapPos1x, mapPos1y, mapPos2x, mapPos2y);

      y1++;
    }
  }
}

