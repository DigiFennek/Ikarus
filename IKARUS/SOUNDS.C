//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Sound Driver
//*
//***************************************************************************

#include "ikarus2.h"
#include "geometry.h"
#include <stdlib.h>
#include <dos.h>

void adLib_setRegister (int addr, int data)
{
  int d;

  outportb (0x388, addr);
  for (d = 1; d <= 6; d++)
    inportb (0x388);
  outportb (0x389, data);
  for (d = 1; d <= 35; d++)
    inportb (0x388);
}


void adLib_setADSR (int channel, int a, int d, int s, int r)
{
  adLib_setRegister (0x60 + channel, (a << 4) + d);
  adLib_setRegister (0x80 + channel, (s << 4) + r);
}


void adLib_setFrequency (int channel, int frequency)
{
  if (frequency)
  {
    adLib_setRegister (0xA0 + channel, frequency);
    adLib_setRegister (0xB0 + channel, 0x20 | (frequency >> 8));
  }
  else
    adLib_setRegister (0xB0 + channel, 0x00);
}


void adLib_reset (void)
{
  int i;

  for (i = 0; i <= 255; i++)
    adLib_setRegister (i, 0);
}


void adLib_initChopperSound (int ch)
{
  // register Offsets
  int a = ch * 3;
  int b = ch * 8;

  // ADSR auf Dauerton einstellen
  adLib_setADSR (0 + b, 15, 0, 0, 0);
  adLib_setADSR (1 + b, 15, 0, 0, 0);
  adLib_setADSR (2 + b, 15, 0, 0, 0);
  adLib_setADSR (3 + b, 15, 0, 0, 0);
  adLib_setADSR (4 + b, 15, 0, 0, 0);
  adLib_setADSR (5 + b, 15, 0, 0, 0);

  // Ton ausschalten
  adLib_setFrequency (0 + a, 0);
  adLib_setFrequency (1 + a, 0);
  adLib_setFrequency (2 + a, 0);
}

void adLib_muteChopperSound (int ch)
{
  // register Offsets
  int a = ch * 3;
  int b = ch * 8;

  // ADSR auf kuerzesten Anschlag stellen
  adLib_setADSR (0 + b, 15, 0, 0, 15);
  adLib_setADSR (1 + b, 15, 0, 0, 15);
  adLib_setADSR (2 + b, 15, 0, 0, 15);
  adLib_setADSR (3 + b, 15, 0, 0, 15);
  adLib_setADSR (4 + b, 15, 0, 0, 15);
  adLib_setADSR (5 + b, 15, 0, 0, 15);

  // Ton ausschalten
  adLib_setFrequency (0 + a, 0);
  adLib_setFrequency (1 + a, 0);
  adLib_setFrequency (2 + a, 0);
}

void adLib_makeChopperSound (int ch, int frequency, int volume, int outside)
{
  // register Offsets
  int a = ch * 3;
  int b = ch * 8;

  // Lautstaerke von innen
  int motorVolume = 40;
  int noiseVolume = 35;
  int rotorVolume = 50;

  // rauschen
  adLib_setRegister (0x20 + b, 0x01);
  adLib_setRegister (0xC0 + a, 0x0E);
  adLib_setFrequency (0 + a, 0x1910);
  adLib_setRegister (0x40 + b, 0);

  // turbine
  adLib_setRegister (0x21 + b, 0x00);
  adLib_setRegister (0xC1 + a, 0x00);
  adLib_setFrequency (1 + a, 6300 + frequency);
  adLib_setRegister (0x41 + b, 0);

  // rotor
  adLib_setRegister (0x22 + b, 0x01);
  adLib_setRegister (0xC2 + a, 0x08);
  adLib_setFrequency (2 + a, 100 + (frequency >> 2));
  adLib_setRegister (0x42 + b, (63 - volume) >> 1);

  // Lautstaerke
  if (outside)
  {
    // Lautstaerke von aussen
    motorVolume = (volume << 1) - 90;
    noiseVolume = (volume << 1) - 100;
    rotorVolume = volume;
  }

  if (frequency < 700)
    rotorVolume += (frequency >> 2) - 175;

  if (frequency < 50)
    motorVolume += frequency - 50;

  if (frequency < 50)
    noiseVolume += frequency - 50;

  adLib_setRegister (0x43 + b, 63 - max (0, noiseVolume));
  adLib_setRegister (0x44 + b, 63 - max (0, motorVolume));
  adLib_setRegister (0x45 + b, 63 - max (0, rotorVolume));
}

extern struct MOBILE observer;
extern int newView;

int soundType = NO_SOUND;

long lastDist[3] = {0, 0, 0};
int mute[3] = {TRUE, TRUE, TRUE};

extern int mobileCount;

void dingSound (void)
{
  if (soundType == ADLIB_SOUND)
  {
    adLib_setADSR (0, 15, 7, 0, 7);
    adLib_setADSR (3, 15, 4, 7, 7);

    adLib_setFrequency (0, 0);
    adLib_setRegister (0x20, 0x00);
    adLib_setRegister (0xC0, 0x00);
    adLib_setFrequency (0, 0x1A00);

    adLib_setRegister (0x40, 30);
    adLib_setRegister (0x43, 10);
  }
}


void muteSound ()
{
  if (soundType == ADLIB_SOUND)
  {
    adLib_muteChopperSound (0);
    adLib_muteChopperSound (1);
    adLib_muteChopperSound (2);
  }

  mute[0] = TRUE;
  mute[1] = TRUE;
  mute[2] = TRUE;
}



void initSound (int newSound)
{
  if (newSound == ADLIB_SOUND)
  {
    if (soundType != ADLIB_SOUND)
      adLib_reset ();
  }

  muteSound ();

  soundType = newSound;
}

void checkSound (struct MOBILE *ch)
{
  struct POINT source;
  long dist;
  int frequency, volume = 0;

  if ((ch->number <= (mobileCount - 1)) && (ch->power > 0))
  {
    if (ch->number != observer.number)
    {
      if ((abs (ch->areaPos.x - observer.areaPos.x) <= AREAVIEWWIDE) &&
	  (abs (ch->areaPos.z - observer.areaPos.z) <= AREAVIEWWIDE))
      {
	int doppler = 0;

	source = ch->position;

	source.x += AREAWIDE * (ch->areaPos.x - observer.areaPos.x);
	source.z += AREAWIDE * (ch->areaPos.z - observer.areaPos.z);

	dist = longScalarProd (&observer.position, &source);

	if (!newView)
	  doppler = (lastDist[ch->number] - dist) / 25000L;

	lastDist[ch->number] = dist;

	frequency = ch->power + min (doppler, 100);
	volume = 63 - min (dist / 5000000L, 63);
      }
    }

    // Wir sind im Helikopter
    else
    {
      frequency = ch->power;
      volume = 63;
    }
  }

  if ((soundType == ADLIB_SOUND) && (ch->number <= 2))
  {
    if (volume)
    {
      if (mute[ch->number])
      {
	adLib_initChopperSound (ch->number);
	mute[ch->number] = FALSE;
      }

      adLib_makeChopperSound (ch->number, frequency,
	volume, (ch->number != observer.number));
    }

    else if (!mute[ch->number])
    {
      adLib_muteChopperSound (ch->number);
      mute[ch->number] = TRUE;
    }
  }
}