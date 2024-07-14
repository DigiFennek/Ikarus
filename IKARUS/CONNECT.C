//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Serial bus driver
//*  - Link zu anderem PC
//*
//***************************************************************************

#include <dos.h>
#include <stdlib.h>
#include "ikarus2.h"

#define BUFFERSIZE 1024

volatile int txPutPtr = 0;
volatile int rxPutPtr = 0;
volatile int txGetPtr = 0;
volatile int rxGetPtr = 0;

volatile int txBusy = 0;

volatile int addr;
volatile int irq;

volatile unsigned char rxBuffer[BUFFERSIZE];
volatile unsigned char txBuffer[BUFFERSIZE];

void interrupt (*oldHandler)(void) = NULL;


int com_getChar (unsigned char *ch)
{
  if (rxGetPtr != rxPutPtr)
  {
    if (++rxGetPtr == sizeof (rxBuffer))
      rxGetPtr = 0;

    *ch = rxBuffer[rxGetPtr];

    return 1;
  }

  return 0;
}



void com_putChar (unsigned char ch)
{
  if (++txPutPtr == sizeof (txBuffer))
    txPutPtr = 0;

  txBuffer[txPutPtr] = ch;
  outportb(addr+1,3);	      /* enable Rx and Tx interrupt */
}


void interrupt irqHandler (void)
{
  int status;

  while (!((status = inportb (addr + 2)) & 1))
  {
    if (status == 2)
    {
      if (txGetPtr != txPutPtr)
      {
	if (++txGetPtr == sizeof (txBuffer))
	  txGetPtr = 0;

	outportb (addr, txBuffer[txGetPtr]);
      }
    }

    else if (status == 4)
    {
      if (++rxPutPtr == sizeof (rxBuffer))
	  rxPutPtr = 0;

      rxBuffer[rxPutPtr] = inportb (addr);
    }
  }

  outportb (0x20, 0x20);
}

void com_flush (void)
{
  /* Pointer an den anfang der Buffer setzen */
  rxPutPtr = rxGetPtr = 0;
  txPutPtr = txGetPtr = 0;
}

void com_init (int com, int bautRate)
{
  /* alle Interrups sperren */
  disable ();

  /* Adresse und IRQ-Nummer fuer COM1 oder COM2 setzen */
  switch (com)
  {
    case 1  : addr = 0x3F8; irq = 4; break; /* COM1 */
    default : addr = 0x2F8; irq = 3; break; /* COM2 */
  }

  /* OUT2, OUT1, RTS und DTR Leitung auf high */
  outportb (addr + 4, 0x0F);

  /* Rx Buffer entleeren */
  inportb (addr);

  /* Buffer leeren */
  com_flush ();

  /* Bautrate setzen */
  outportb (addr + 3, 0x80);
  outport (addr, 115200 / bautRate);

  /* 8 bit, no parity, 1 stop */
  outportb (addr + 3, 0x03);

  /* Interrupt auf irqHandler umleiten */
  if (getvect (irq + 8) != irqHandler)
  {
    oldHandler = getvect (irq + 8);
    setvect (irq + 8, irqHandler);
  }

  /* Interruptkontroller setzen */
  outportb (0x21, inportb (0x21) & (0xFF - (1 << irq)));

  /* alle Interrups freigeben */
  enable ();

  /* Rx und Tx Interrupt freigeben */
  outportb (addr + 1, 0x03);

  /* Kuenstlicher Interrupt erzeugen */
  geninterrupt (irq + 8);
}


void com_exit (void)
{
  if (oldHandler != NULL)
  {
    outportb(0x21, inportb (0x21) | (1 << irq ));
    setvect (irq + 8, oldHandler);
  }
}


void sendWord (int w)
{
  com_putChar (w & 0x00FF);
  com_putChar (w >> 8);
}


void transmit (struct MOBILE *plane)
{
  sendWord (0xFFFF);
  sendWord (0xFFFF);
  com_putChar (plane->areaPos.x);
  com_putChar (plane->areaPos.z);
  sendWord (plane->position.x);
  sendWord (plane->position.y);
  sendWord (plane->position.z);
  sendWord (plane->angle.x);
  sendWord (plane->angle.y);
  sendWord (plane->angle.z);
  sendWord (plane->power);
  com_putChar (plane->number);
  com_putChar (plane->state);
}


extern struct MOBILE mobileMove[];
static struct MOBILE mobileTemp[2];

extern int mobileCount;

int linkPort = COM_NONE;



void makeLink (int com)
{
  if (com != COM_NONE)
  {
    if (linkPort == COM_NONE)
    {
      resetMobile (&mobileMove[0]);
      resetMobile (&mobileMove[1]);
      mobileTemp[0] = mobileMove[0];
      mobileTemp[1] = mobileMove[1];
    }

    mobileCount = 1;

    com_init (com, 4800);
  }

  else
  {
    if (linkPort != COM_NONE)
    {
      mobileMove[0] = mobileTemp[0];
      mobileMove[1] = mobileTemp[1];
    }
  }

  linkPort = com;
}


static int count = 0;

unsigned char buffer[30];

int receive (struct MOBILE *plane)
{
  unsigned char ch;

  while (com_getChar (&ch))
  {
    if (count >= 4)
    {
      buffer[count++] = ch;

      if (count >= 22)
      {
	count = 0;

	plane->areaPos.x = buffer[4];
	plane->areaPos.z = buffer[5];
	plane->position.x = buffer[6] + (buffer[7]  << 8);
	plane->position.y = buffer[8] + (buffer[9] << 8);
	plane->position.z = buffer[10] + (buffer[11] << 8);
	plane->angle.x = buffer[12] + (buffer[13] << 8);
	plane->angle.y = buffer[14] + (buffer[15] << 8);
	plane->angle.z = buffer[16] + (buffer[17] << 8);
	plane->power = buffer[18] + (buffer[19] << 8);
	plane->number = buffer[20];
	plane->state = buffer[21];

	return 1;
      }
    }
    else
    {
      if (ch == 0xFF)
	count++;
      else
	count = 0;
    }
  }
  return 0;
}


#define TIMEOUT 10;
int timeout = TIMEOUT;


void checkConnection (void)
{
  if (linkPort != COM_NONE)
  {
    if (receive (&mobileMove[1]))
    {
      if (mobileMove[0].number == mobileMove[1].number)
      {
	mobileMove[0] = mobileTemp[1];
	resetMobile (&mobileMove[0]);
      }

      transmit (&mobileMove[0]);
      timeout = TIMEOUT;
      mobileCount = 2;
    }

    if (timeout-- == 0)
    {
      makeLink (linkPort);
      timeout = TIMEOUT;
      transmit (&mobileMove[0]);
    }
  }
}



