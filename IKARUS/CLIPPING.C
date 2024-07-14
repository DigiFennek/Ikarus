//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Clipping von Polygonen
//*
//***************************************************************************

#include "ikarus2.h"
#include "geometry.h"

struct POINT minClip = {MINCLIPX, MINCLIPY, 0};
struct POINT maxClip = {MAXCLIPX, MAXCLIPY, 0};

int clipLeft (int polySize, struct POINT polyIn[], struct POINT polyOut[])
{
  int newSize = 0;

  struct POINT *P1 = &polyIn[polySize - 1];
  struct POINT *P0 = &polyIn[0];
  struct POINT *PN = &polyOut[0];

  while (polySize > 0)
  {
    if (P0->x >= minClip.x)
    {
      if (P1->x >= minClip.x)
      {
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
      else
      {
	PN->x = minClip.x;
	PN->y = intersect (P0->x, P0->y, P1->x, P1->y, minClip.x);
	PN++; newSize++;
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
    }
    else if (P1->x >= minClip.x)
    {
      PN->x = minClip.x;
      PN->y = intersect (P0->x, P0->y, P1->x, P1->y, minClip.x);
      PN++; newSize++;
    }

    P1 = P0;
    P0++; polySize--;
  }

  return newSize;
}


int clipRight (int polySize, struct POINT polyIn[], struct POINT polyOut[])
{
  int newSize = 0;

  struct POINT *P1 = &polyIn[polySize - 1];
  struct POINT *P0 = &polyIn[0];
  struct POINT *PN = &polyOut[0];

  while (polySize > 0)
  {
    if (P0->x <= maxClip.x)
    {
      if (P1->x <= maxClip.x)
      {
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
      else
      {
	PN->x = maxClip.x;
	PN->y = intersect (P1->x, P1->y, P0->x, P0->y, maxClip.x);
	PN++; newSize++;
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
    }
    else if (P1->x <= maxClip.x)
    {
      PN->x = maxClip.x;
      PN->y = intersect (P1->x, P1->y, P0->x, P0->y, maxClip.x);
      PN++; newSize++;
    }

    P1 = P0;
    P0++; polySize--;
  }

  return newSize;
}


int clipTop (int polySize, struct POINT polyIn[], struct POINT polyOut[])
{
  int newSize = 0;

  struct POINT *P1 = &polyIn[polySize - 1];
  struct POINT *P0 = &polyIn[0];
  struct POINT *PN = &polyOut[0];

  while (polySize > 0)
  {
    if (P0->y >= minClip.y)
    {
      if (P1->y >= minClip.y)
      {
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
      else
      {
	PN->x = intersect (P0->y, P0->x, P1->y, P1->x, minClip.y);
	PN->y = minClip.y;
	PN++; newSize++;
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
    }
    else if (P1->y >= minClip.y)
    {
      PN->x = intersect (P0->y, P0->x, P1->y, P1->x, minClip.y);
      PN->y = minClip.y;
      PN++; newSize++;
    }

    P1 = P0;
    P0++; polySize--;
  }

  return newSize;
}


int clipBottom (int polySize, struct POINT polyIn[], struct POINT polyOut[])
{
  int newSize = 0;

  struct POINT *P1 = &polyIn[polySize - 1];
  struct POINT *P0 = &polyIn[0];
  struct POINT *PN = &polyOut[0];

  while (polySize > 0)
  {
    if (P0->y <= maxClip.y)
    {
      if (P1->y <= maxClip.y)
      {
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
      else
      {
	PN->x = intersect (P1->y, P1->x, P0->y, P0->x, maxClip.y);
	PN->y = maxClip.y;
	PN++; newSize++;
	PN->x = P0->x;
	PN->y = P0->y;
	PN++; newSize++;
      }
    }
    else if (P1->y <= maxClip.y)
    {
      PN->x = intersect (P1->y, P1->x, P0->y, P0->x, maxClip.y);
      PN->y = maxClip.y;
      PN++; newSize++;
    }

    P1 = P0;
    P0++; polySize--;
  }

  return newSize;
}


int clipFront (int polySize, struct POINT polyIn[], struct POINT polyOut[])
{
  int newSize = 0;

  struct POINT *P1 = &polyIn[polySize - 1];
  struct POINT *P0 = &polyIn[0];
  struct POINT *PN = &polyOut[0];

  while (polySize > 0)
  {
    if (P0->z >= minClip.z)
    {
      if (P1->z >= minClip.z)
      {
	PN->x = P0->x;
	PN->y = P0->y;
	PN->z = P0->z;
	PN++; newSize++;
      }
      else
      {
	PN->x = intersect (P0->z, P0->x, P1->z, P1->x, minClip.z);
	PN->y = intersect (P0->z, P0->y, P1->z, P1->y, minClip.z);
	PN->z = minClip.z;
	PN++; newSize++;
	PN->x = P0->x;
	PN->y = P0->y;
	PN->z = P0->z;
	PN++; newSize++;
      }
    }
    else if (P1->z >= minClip.z)
    {
      PN->x = intersect (P0->z, P0->x, P1->z, P1->x, minClip.z);
      PN->y = intersect (P0->z, P0->y, P1->z, P1->y, minClip.z);
      PN->z = minClip.z;
      PN++; newSize++;
    }

    P1 = P0;
    P0++; polySize--;
  }

  return newSize;
}

