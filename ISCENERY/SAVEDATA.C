//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*
//*
//***************************************************************************

#include <alloc.h>
#include <mem.h>
#include <stdio.h>
#include "ikarus2.h"
#include "geometry.h"

FILE *scn;

int objectCount = 0;
int objectxArea = 0;
int objectzArea = 0;

char areaUsed[AREAMAPSIZE][AREAMAPSIZE];

int getObjectSize (int *object)
{
  int objectSize = 0;

  while (*object)
  {
    int polySize = *object;

    objectSize += 6;
    object += 6;

    object += polySize * 3;
    objectSize += polySize * 3;
  }

  objectSize++;

  return (objectSize);
}


void opcode (int opc)
{
  fputc  ('#', scn);
  fputc  (opc, scn);
}


void viewPoint (int x, int y, int z)
{
  opcode (0x80);
  fwrite (&objectxArea, sizeof (objectxArea), 1, scn);
  fwrite (&objectzArea, sizeof (objectzArea), 1, scn);
  fwrite (&x, sizeof (x), 1, scn);
  fwrite (&y, sizeof (y), 1, scn);
  fwrite (&z, sizeof (z), 1, scn);
}


void startPoint (int x, int y, int z, int xAngle, int yAngle, int zAngle)
{
  opcode (0x40);
  fwrite (&objectxArea, sizeof (objectxArea), 1, scn);
  fwrite (&objectzArea, sizeof (objectzArea), 1, scn);
  fwrite (&x, sizeof (x), 1, scn);
  fwrite (&y, sizeof (y), 1, scn);
  fwrite (&z, sizeof (z), 1, scn);
  fwrite (&xAngle, sizeof (xAngle), 1, scn);
  fwrite (&yAngle, sizeof (yAngle), 1, scn);
  fwrite (&zAngle, sizeof (zAngle), 1, scn);
}


void assignArea (int xArea, int zArea, int count)
{
  if ((!areaUsed[xArea][zArea]) && count)
  {
    areaUsed[xArea][zArea] = TRUE;
    opcode (0x01);
    fwrite (&xArea, sizeof (xArea), 1, scn);
    fwrite (&zArea, sizeof (zArea), 1, scn);
    fwrite (&count, sizeof (count), 1, scn);

    objectCount = count;
    objectxArea = xArea;
    objectzArea = zArea;
  }
  else
    objectCount = 0;
}


void transformObject (int *objectIn, int *objectOut, int x, int y, int z,
		      int xAngle, int yAngle, int zAngle)
{
  setPosition (x, y, z);
  setAngle (xAngle, yAngle, zAngle);
  setOrder ("xyzt");

  while (*objectIn)
  {
    int polySize = *objectIn++;
    *objectOut++ = polySize;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;
    *objectOut++ = *objectIn++;

    memcpy (objectOut, objectIn, polySize * sizeof (struct POINT));

    transform (polySize, (struct POINT *)objectOut);

    objectIn += (3 * polySize);
    objectOut += (3 * polySize);
  }

  *objectOut = 0;
}


void placeObject (int *object, int x, int y, int z,
		  int xAngle, int yAngle, int zAngle, int complexity)
{
  if (objectCount--)
  {
    int objectSize = getObjectSize (object);
    int *objectTrans;

    if ((objectTrans = farmalloc (objectSize * sizeof (int))) != NULL)
    {
      transformObject (object, objectTrans, x, y, z,
	xAngle, yAngle, zAngle);

      objectTrans[1] = complexity;

      opcode (0x02);
      fwrite (&objectxArea, sizeof (objectxArea), 1, scn);
      fwrite (&objectzArea, sizeof (objectzArea), 1, scn);
      fwrite (&objectCount, sizeof (objectCount), 1, scn);
      fwrite (&objectSize, sizeof (objectSize), 1, scn);
      fwrite (objectTrans, sizeof (int), objectSize, scn);

      farfree (objectTrans);
    }
  }
}


void dynamicObject (int *object, int type)
{
  *object++ = 1;
  *object++ = 0;
  *object++ = type;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
  *object++ = 0;
}


void copyArea (int srcxArea, int srczArea, int xArea, int zArea)
{
  if (!areaUsed[xArea][zArea])
  {
    areaUsed[xArea][zArea] = TRUE;
    opcode (0x03);
    fwrite (&srcxArea, sizeof (srcxArea), 1, scn);
    fwrite (&srczArea, sizeof (srczArea), 1, scn);
    fwrite (&xArea, sizeof (xArea), 1, scn);
    fwrite (&zArea, sizeof (zArea), 1, scn);
  }

  objectxArea = xArea;
  objectzArea = zArea;
}


void setGroundColor (char color)
{
  opcode (0xF2);
  fwrite (&color, sizeof (color), 1, scn);
}


void setComplexity (int complexity)
{
  opcode (0xF0);
  fwrite (&complexity, 1, 1, scn);
}


void setViewMode (int viewMode)
{
  opcode (0xF1);
  fwrite (&viewMode, 1, 1, scn);
}


void saveBMP (char *fileName)
{
  unsigned int size = 320 * 200;
  int i, y;
  unsigned char rgb[3], *image;
  FILE *bmp;

  // Bitmapfile oeffnen
  bmp = fopen (fileName,"rb");

  // an den Anfang der Farbpalette
  fseek (bmp, 54, SEEK_SET);

  // Farbpalette kopieren
  for (i = 0; i < 256; i++)
  {
    rgb[0] = fgetc (bmp); /* blue */
    rgb[1] = fgetc (bmp); /* green */
    rgb[2] = fgetc (bmp); /* red */
    fgetc (bmp);
    fwrite (&rgb, sizeof (rgb), 1, scn);
  }

  // Bild kopieren
  if ((image = farmalloc (size)) != NULL)
  {
    for (y = 199; y >= 0; y--)
      fread (&image[y * 320], 320, sizeof (char), bmp);
    fwrite (image, size, sizeof (char), scn);
    farfree (image);
  }
  else
    fwrite (image, size, sizeof (char), scn);
}


void colorPalette (void)
{
  unsigned char far *palette;


  if ((palette = farmalloc (3 * 256)) != NULL)
  {
    int i, k, j, c = 0;

    for (i = 0; i < 768; i++)
      palette[i] = 0;

    c = 9;

    for (i = 0; i < 5; i++)
      for (k = 0; k < 5; k++)
        for (j = 0; j < 5; j++)
        {
          palette[c+0] = i * 63;
          palette[c+1] = k * 63;
          palette[c+2] = j * 63;
          c += 3;
        }


    for (i = 0; i <= 15; i++)
    {
      palette[i * 3 + 2 + 3 * RED] = 15 + ((i+0) << 4);
      palette[i * 3 + 1 + 3 * RED] = 0;
      palette[i * 3 + 0 + 3 * RED] = 0;
      palette[i * 3 + 2 + 3 * GREEN] = 0;
      palette[i * 3 + 1 + 3 * GREEN] = 15 + ((i+0) << 4);
      palette[i * 3 + 0 + 3 * GREEN] = 0;
      palette[i * 3 + 2 + 3 * BLUE] = 0;
      palette[i * 3 + 1 + 3 * BLUE] = 0;
      palette[i * 3 + 0 + 3 * BLUE] = 15 + ((i+0) << 4);
      palette[i * 3 + 2 + 3 * YELLOW] = 15 + ((i+0) << 4);
      palette[i * 3 + 1 + 3 * YELLOW] = 15 + ((i+0) << 4);
      palette[i * 3 + 0 + 3 * YELLOW] = 0;
      palette[i * 3 + 2 + 3 * GREY] = 15 + ((i+0) << 4);
      palette[i * 3 + 1 + 3 * GREY] = 15 + ((i+0) << 4);
      palette[i * 3 + 0 + 3 * GREY] = 15 + ((i+0) << 4);
      palette[i * 3 + 2 + 3 * BROWN] = 15 + ((i+0) << 4);
      palette[i * 3 + 1 + 3 * BROWN] = 15 + (((i+0) << 5) / 5);
      palette[i * 3 + 0 + 3 * BROWN] = 0;
      palette[i * 3 + 2 + 3 * SKYBLUE] = ((i+1) + 40) * 4;
      palette[i * 3 + 1 + 3 * SKYBLUE] = ((i+1) + 40) * 4;
      palette[i * 3 + 0 + 3 * SKYBLUE] = 255;
    }

    opcode  (0xD0);
    fwrite (palette, 256 * 3, sizeof (char), scn);
    {
      int i;
      FILE *f = fopen ("bitmaps\\ikarus.pal", "wt");
      fprintf (f, "JASC-PAL\n0100\n256\n");
      for (i = 0; i <= 255; i++)
        fprintf (f, "%i %i %i\n", palette[i*3+2],
          palette[i*3+1],palette[i*3+0]);
      fclose(f);
    }
    farfree (palette);
  }

}


void titlePicture (char *bmpName, int timeout)
{
  opcode  (0xE0);
  fwrite (&timeout, sizeof (timeout), 1, scn);
  saveBMP (bmpName);
}


void menuBackground (char *bmpName, int menuNumber)
{
  opcode (0xE1);
  fwrite (&menuNumber, sizeof (menuNumber), 1, scn);
  saveBMP (bmpName);
}


void createDatabase (char *name)
{
  printf ("creating ikarus scenery %s\n", name);
  memset (&areaUsed, FALSE, sizeof (areaUsed));
  scn = fopen (name, "wb");
}


void closeDatabase (void)
{
  printf ("scenery created.\n");
  opcode (0xFF);
  fclose (scn);
}


void texture (char *name, int num)
{
  int bpp, xWide, yWide, i, offs, size;
  FILE *bmp;
  unsigned char far *image;

  bmp = fopen (name, "rb");

  fseek (bmp, 0x0A, SEEK_SET);
  fread (&offs, sizeof (offs), 1, bmp);
  fseek (bmp, 0x12, SEEK_SET);
  fread (&xWide, sizeof (xWide), 1, bmp);
  fseek (bmp, 0x16, SEEK_SET);
  fread (&yWide, sizeof (yWide), 1, bmp);
  fseek (bmp, 0x1C, SEEK_SET);
  fread (&bpp, sizeof (bpp), 1, bmp);
  fseek (bmp, offs, SEEK_SET);

  // 256-Farben bmp, max. 255 x 255 Pixel
  if ((xWide <= 255) && (yWide <= 255) && (bpp == 8) && bmp &&
      (image = farmalloc (xWide * yWide)) != NULL)
  {
    printf ("creating texture %s: %i x %i pixels\n", name, xWide, yWide);

    size = xWide * yWide + 2;
    offs = (4 - xWide) & 3;

    // Bild lesen
    for (i = (yWide - 1); i >= 0; i--)
    {
      fread (&image[i * xWide], xWide, sizeof (char), bmp);
      fseek (bmp, offs, SEEK_CUR);
    }

    // texture schreiben
    opcode (0x20);
    fwrite (&num, sizeof (num), 1, scn);
    fwrite (&size, sizeof (size), 1, scn);
    fwrite (&xWide, sizeof (char), 1, scn);
    fwrite (&yWide, sizeof (char), 1, scn);
    fwrite (image, sizeof (char), size - 2, scn);

    farfree (image);
  }
  else
    printf ("can't create texture %s\n", name);

}
