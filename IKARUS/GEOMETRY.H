//***************************************************************************
//* PROJEKT HELIKOPTER - SIMULATOR
//***************************************************************************
//*
//*  - Definitionen fuer GEOMETRY.ASM
//*
//***************************************************************************

extern	void	setAngle	(int alpha, int beta, int gamma);
extern	void	setPosition	(int xpos, int ypos, int zpos);
extern	void	setOrder	(char far *newOrder);
extern	void	transform	(int polySize, struct POINT far *poly);
extern	void	perspective	(int polySize, struct POINT far *poly);
extern	int	testVisible	(struct POINT far *poly);
extern	int	intersect	(int a1, int b1, int a2, int b2, int i);
extern	int	intSinMul	(int angle, int v);
extern	int	intCosMul	(int angle, int v);
extern	int	intSin		(int angle);
extern	int	intCos		(int angle);
extern	int	intArcSin	(int sinAngle);
extern	int	intArcCos	(int sinAngle);
extern	int	intRoot		(long radikand);
extern	long    longScalarProd  (struct POINT far *p1, struct POINT far *p2);

