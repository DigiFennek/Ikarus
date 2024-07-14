//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Database reader
//*
//***************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>
#include "ikarus2.h"

int **(*objectMap)[AREAMAPSIZE];
char far *texturePtr[TEXTURES];

extern struct MOBILE viewPoint[VIEWPOINTS];
extern struct MOBILE mobileMove[];

extern int viewMode;
extern int complexity;
extern int groundColor;

int objects = 0;
int mobileCount = 0;
int viewPoints = 0;

extern unsigned char *menuPicture;
extern unsigned char *menuPalette;
extern unsigned char *mainPalette;

int readDatabase (char *fileName)
{
  FILE *scn; int done = FALSE;

  if ((scn = fopen (fileName, "rb")) == NULL)
    return DB_NOT_FOUND_ERROR;

  while (!done)
  {
    unsigned char opcode, marker;

    fread (&marker, sizeof (marker), 1, scn);
    fread (&opcode, sizeof (opcode), 1, scn);

    if (marker != '#')
      return DB_FORMAT_ERROR;

    switch (opcode)
    {
      case 0x01 : // assign area
      {
	unsigned int areax, areaz, objectCount;

	fread (&areax, sizeof (areax), 1, scn);
	fread (&areaz, sizeof (areaz), 1, scn);
	fread (&objectCount, sizeof (objectCount), 1, scn);

	if ((areax < AREAMAPSIZE) && (areaz < AREAMAPSIZE))
	{
	  int i, **objectPtrPtr;

	  objectPtrPtr = farmalloc ((objectCount + 1) * sizeof (int **));

	  if (objectPtrPtr)
	  {
	    for (i = 0; i <= objectCount; i++)
	      objectPtrPtr[i] = NULL;

	    objectMap[areax][areaz] = objectPtrPtr;
	  }
	}
      }
      break;

      case 0x02 : // object
      {
	unsigned int areax, areaz, number, objectSize;
	int *object;

	fread (&areax, sizeof (areax), 1, scn);
	fread (&areaz, sizeof (areaz), 1, scn);
	fread (&number, sizeof (number), 1, scn);

	fread (&objectSize, sizeof (objectSize), 1, scn);

	object = farmalloc (objectSize * sizeof (int));

	if ((areax < AREAMAPSIZE) && (areaz < AREAMAPSIZE) &&
	    (object != NULL) && (objectMap[areax][areaz] != NULL))
	{
	  int **objectPtrPtr = objectMap[areax][areaz];

	  objectPtrPtr[number] = object;

	  fread (object, sizeof (int), objectSize, scn);
	}
	else
	  fseek (scn, objectSize * sizeof (int), SEEK_CUR);
      }
      break;

      case 0x03 : // copy area
      {
	unsigned int areax, areaz, srcAreax, srcAreaz;

	fread (&srcAreax, sizeof (srcAreax), 1, scn);
	fread (&srcAreaz, sizeof (srcAreaz), 1, scn);

	fread (&areax, sizeof (areax), 1, scn);
	fread (&areaz, sizeof (areaz), 1, scn);

	objectMap[areax][areaz] = objectMap[srcAreax][srcAreaz];
      }
      break;

      case 0x20 : // texture
      {
	unsigned int num, size;

	fread (&num, sizeof (num), 1, scn);
	fread (&size, sizeof (size), 1, scn);

	if ((texturePtr[num] = farmalloc(size)) != NULL)
	  fread (texturePtr[num], sizeof (char), size, scn);
	else
	  fseek (scn, size, SEEK_CUR);
      }
      break;

      case 0x40 : // start point
      {
	int x, y, z, a, b, c, areax, areaz;

	fread (&areax, sizeof (areax), 1, scn);
	fread (&areaz, sizeof (areaz), 1, scn);
	fread (&x, sizeof (x), 1, scn);
	fread (&y, sizeof (y), 1, scn);
	fread (&z, sizeof (z), 1, scn);
	fread (&a, sizeof (a), 1, scn);
	fread (&b, sizeof (b), 1, scn);
	fread (&c, sizeof (c), 1, scn);

	if (objects < CHOPPERS)
	{
	  mobileMove[objects].homeArea.x = areax;
	  mobileMove[objects].homeArea.z = areaz;
	  mobileMove[objects].homePos.x = x;
	  mobileMove[objects].homePos.y = y;
	  mobileMove[objects].homePos.z = z;
	  mobileMove[objects].homeAngle.x = a;
	  mobileMove[objects].homeAngle.y = b;
	  mobileMove[objects].homeAngle.z = c;
	  mobileMove[objects].number = objects;
	  resetMobile (&mobileMove[objects]);
	  objects++;
	}
      }
      break;

      case 0x80 : // view point
      {
	int x, y, z, areax, areaz;

	fread (&areax, sizeof (areax), 1, scn);
	fread (&areaz, sizeof (areaz), 1, scn);
	fread (&x, sizeof (x), 1, scn);
	fread (&y, sizeof (y), 1, scn);
	fread (&z, sizeof (z), 1, scn);

	if (viewPoints < VIEWPOINTS)
	{
	  viewPoint[viewPoints].areaPos.x = areax;
	  viewPoint[viewPoints].areaPos.z = areaz;
	  viewPoint[viewPoints].position.x = x;
	  viewPoint[viewPoints].position.y = y;
	  viewPoint[viewPoints].position.z = z;
	  viewPoint[viewPoints].number = -1;
	  viewPoints++;
	}
      }
      break;

      case 0xD0 : // color palette
      {
	if ((mainPalette = farmalloc (768)) != NULL)
	  fread (mainPalette, sizeof (char), 768, scn);
	else
	  fseek (scn, 768, SEEK_CUR);
      }
      break;

      case 0xE0 : // title picture 320 x 200
      {
	unsigned int timeout;
	unsigned char *picture;
	unsigned char *palette;

	fread (&timeout, sizeof (timeout), 1, scn);

	if ((palette = farmalloc (768)) != NULL)
	  fread (palette, sizeof (char), 768, scn);
	else
	  fseek (scn, 768, SEEK_CUR);

	if ((picture = farmalloc (64000)) != NULL)
	  fread (picture, sizeof (char), 64000, scn);
	else
	  fseek (scn, 64000, SEEK_CUR);

	putImage (0, 0, picture, 320, 200);
	initColorPalette (palette, 0, 255);
	flipScreen ();
	fadePalette (100);
	timeoutWait (timeout);
	fadePalette (-50);
	farfree (picture);
	farfree (palette);
      }
      break;

      case 0xE1 : // menu picture 320 x 200
      {
	unsigned int number;

	fread (&number, sizeof (number), 1, scn);

	if ((menuPalette = farmalloc (768)) != NULL)
	  fread (menuPalette, sizeof (char), 768, scn);
	else
	  fseek (scn, 768, SEEK_CUR);

	if ((menuPicture = farmalloc (64000)) != NULL)
	  fread (menuPicture, sizeof (char), 64000, scn);
	else
	  fseek (scn, 64000, SEEK_CUR);

      }
      break;

      case 0xF0 : // setup : complexity
      {
	complexity = 0;
	fread (&complexity, 1, 1, scn);
      }
      break;

      case 0xF1 : // setup : view mode
      {
	viewMode = 0;
	fread (&viewMode, 1, 1, scn);
      }
      break;

      case 0xF2 : // ground color
      {
	groundColor = 0;
	fread (&groundColor, 1, 1, scn);
      }
      break;

      case 0xFF : // end of database
      {
	done = TRUE;
      }
      break;
    }
  }

  fclose (scn);

  return NO_ERROR;
}


int initDatabase (char *fileName)
{
  unsigned int mapSize = sizeof (int **) * AREAMAPSIZE * AREAMAPSIZE;
  unsigned int x, z;

  objectMap = farmalloc (mapSize);

  if (objectMap == NULL)
    return MEMORY_ERROR;

  for (x = 0; x < AREAMAPSIZE; x++)
    for (z = 0; z < AREAMAPSIZE; z++)
      objectMap[x][z] = NULL;

  return readDatabase (fileName);
}
