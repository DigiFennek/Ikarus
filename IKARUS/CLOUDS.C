//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Wolken
//*
//***************************************************************************

#include <stdlib.h>
#include <alloc.h>
#include <dos.h>
#include "ikarus2.h"
#include "geometry.h"

extern struct MOBILE observer;

extern char far *texturePtr[];

char cloudMap[AREAMAPSIZE][AREAMAPSIZE];

static int cloud[] =
{
  1, VERYSPARSE, MIDPOINT, 0, 0, 0,
	   0,	8000,	   0,
  4, TEXTURE, NOCHECK, MAXDIST, 0, TEXTURES - 1,
       -2000,	   0,  -2000,
       -2000,	   0,	2000,
	2000,	   0,   2000,
	2000,	   0,  -2000,
  0
};

static int tCloud[sizeof (cloud) / sizeof (int)];

char cMap[8][8] =
{
  0,0,1,1,1,1,0,0,
  0,1,2,2,2,2,1,0,
  1,2,2,3,3,2,1,1,
  1,2,2,3,4,2,2,1,
  1,2,3,4,4,3,2,1,
  1,2,2,2,2,2,2,1,
  0,1,2,2,2,2,1,0,
  0,0,1,1,1,1,0,0,
};

char far *initCloudTexture (int n)
{
  int xWide = 32, yWide = 32;
  int size = xWide * yWide;
  int i, x1, x2, y1, y2;
  char far *texture;

  if ((texture = farmalloc (size + 2 + yWide)) != NULL)
  {
    *texture++ = xWide;
    *texture++ = yWide;

    for (i = 0; i < (size + yWide); i++)
      texture[i] = 0;

    for (i = 0; i < n; i++)
    {
      x1 = random (xWide - 8);
      y1 = random (yWide - 8);

      for (x2 = 0; x2 < 8; x2++)
	for (y2 = 0; y2 < 8; y2++)
	{
	  char far *t = &texture[(y1 + y2) * xWide + x1 + x2];

	  if (*t < (15 - cMap[x2][y2]))
	    *t += cMap[x2][y2];
	}
    }

    for (i = 0; i < size; i++)
      if (texture[i])
	texture[i] += SKYBLUE;
  }

  return texture - 2;
}


void initClouds (int n)
{
  setmem (cloudMap, sizeof (cloudMap), 0);

  while (n--)
    cloudMap[random(AREAMAPSIZE)][random(AREAMAPSIZE)] = random (7) + 1;

  texturePtr[1] = initCloudTexture (10);
  texturePtr[2] = initCloudTexture (15);
  texturePtr[3] = initCloudTexture (20);
  texturePtr[4] = initCloudTexture (25);
  texturePtr[5] = initCloudTexture (30);
  texturePtr[6] = initCloudTexture (35);
  texturePtr[7] = initCloudTexture (40);
}


void displayClouds (int areax, int areaz)
{
  if (cloudMap[areax][areaz])
  {
    int hight = observer.position.y - (observer.position.y >> 2);
    hight += 2000 + cloudMap[areax][areaz] * 100;
    cloud[14] = cloudMap[areax][areaz];
    setPosition (0, hight, 0);
    setOrder ("t");
    transformObject (cloud, tCloud);
    addToObjectList (tCloud);
  }
}


struct POINT fogPoly[] =
{
  0, 0, 0,
  319, 0, 0,
  319, 199, 0,
  0, 199, 0
};


void displayCloudFog ()
{
  if (cloudMap[observer.areaPos.x][observer.areaPos.z])
    if ((observer.position.y > 7000) && (observer.position.y < 9000))
      fillPoly (4, fogPoly, GREY + 15 + PLAIDFILL, GREY + 15 + PLAIDFILL);
}



