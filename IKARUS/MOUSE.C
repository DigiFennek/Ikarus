//***************************************************************************
//* IKARUS HELIKOPTER-SIMULATOR
//***************************************************************************
//*
//*  - Mouse driver
//*
//***************************************************************************

#include <dos.h>

int mousePointerOn;

int initMouse (void)
{
  union REGS reg;
  mousePointerOn = 0;
  reg.x.ax = 0x0000;
  int86 (0x33, &reg, &reg);
  return (reg.x.ax == 65535);
}


void getMousePos (int *x, int *y, int *key)
{
  union REGS reg;
  reg.x.ax = 0x0003;
  int86 (0x33, &reg, &reg);
  *key = reg.x.bx;
  *x   = reg.x.cx;
  *y   = reg.x.dx;
}


void setMousePos (int x, int y)
{
  union REGS reg;
  reg.x.cx = x;
  reg.x.dx = y;
  reg.x.ax = 0x0004;
  int86 (0x33, &reg, &reg);
}


void setMousePointer (int on)
{
  union REGS reg;

  if (on != mousePointerOn)
  {
    if (on)
      reg.x.ax = 0x0001;
    else
      reg.x.ax = 0x0002;

    int86 (0x33, &reg, &reg);

    mousePointerOn = on;
  }
}

