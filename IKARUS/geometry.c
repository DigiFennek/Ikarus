;****************************************************************************
;* PROJEKT HELIKOPTER - SIMULATOR
;****************************************************************************
;*
;*  - Transformation und Rotation
;*  - Perspektive
;*  - Intersection
;*  - Sinus und Cosinus
;*
;****************************************************************************
asm
{
		DOSSEG
		.MODEL COMPACT

		PUBLIC C transform
		PUBLIC C setAngle
		PUBLIC C setPosition
		PUBLIC C setOrder
		PUBLIC C perspective
		PUBLIC C testVisible
		PUBLIC C intersect
		PUBLIC C intSinMul
		PUBLIC C intCosMul
		PUBLIC C intSin
		PUBLIC C intCos
		PUBLIC C intArcSin
		PUBLIC C intArcCos
		PUBLIC C intRoot
		PUBLIC C longScalarProd

POINT		STRUC
x		DW	?
y		DW	?
z		DW	?
POINT		ENDS

		.DATA?

sin		POINT	<0,0,0>
cos		POINT	<0,0,0>
pos		POINT	<0,0,0>

order		DB	'xyzt'

		.Code

;----------------------------------------------------------------------------
; void setAngle (int alpha, int beta, int gamma)
;----------------------------------------------------------------------------
;
setAngle	PROC
;
ARG		alpha		: WORD, \
		beta		: WORD, \
		gamma		: WORD
;
		push	bp
		mov	bp,sp

;		sin.x = intSin (alpha);

		mov	ax,[alpha]
		push	ax
		call	intSin
		add	sp,2
		mov	[sin.x],ax

;		sin.y = intSin (beta);

		mov	ax,[beta]
		push	ax
		call	intSin
		add	sp,2
		mov	[sin.y],ax

;		sin.z = intSin (gamma);

		mov	ax,[gamma]
		push	ax
		call	intSin
		add	sp,2
		mov	[sin.z],ax

;		cos.x = intCos (alpha);

		mov	ax,[alpha]
		push	ax
		call	intCos
		add	sp,2
		mov	[cos.x],ax

;		cos.y = intCos (beta);

		mov	ax,[beta]
		push	ax
		call	intCos
		add	sp,2
		mov	[cos.y],ax

;		cos.z = intCos (gamma);

		mov	ax,[gamma]
		push	ax
		call	intCos
		add	sp,2
		mov	[cos.z],ax

		pop	bp
		ret

setAngle	ENDP


;----------------------------------------------------------------------------
; void setPosition (int xpos, int ypos, int zpos)
;----------------------------------------------------------------------------
;
setPosition	PROC
;
ARG		xpos		: WORD, \
		ypos		: WORD, \
		zpos		: WORD
;
		push	bp
		mov	bp,sp

;		pos.x = xpos;

		mov	ax,[xpos]
		mov	[pos.x],ax

;		pos.y = ypos;

		mov	ax,[ypos]
		mov	[pos.y],ax

;		pos.z = zpos;

		mov	ax,[zpos]
		mov	[pos.z],ax

		pop	bp
		ret

setPosition	ENDP


;----------------------------------------------------------------------------
; void setOrder (char far *newOrder)
;----------------------------------------------------------------------------
;
setOrder	PROC
;
ARG		newOrder	: DWORD
;
		push	bp
		mov	bp,sp
		push	si
		push	ds

		mov	ax,@DATA
		mov	ds,ax

;		for (i = 0; i < 4; i++)

		sub	bx,bx
setOrderLoop:
		cmp	bx,4
		je	setOrderEnd

;		{

;		  order[i] = newOrder[i];

		les	si,[newOrder]
		mov	al,es:[si+bx]
		mov	ds:[bx+order],al

;		}

		inc	bx
		jmp	setOrderLoop
setOrderEnd:
		pop	ds
		pop	si
		pop	bp
		ret

setOrder	ENDP


;----------------------------------------------------------------------------
; void transform (int polySize, struct POINT far *poly)
;----------------------------------------------------------------------------
;
transform	PROC
;
ARG		polySize	: WORD, \
		poly		: DWORD
;
LOCAL		orderPos	: WORD = AUTO_SIZE
;
		push	bp
		mov	bp,sp
		sub	sp,AUTO_SIZE
		push	di
		les	di,[poly]
		push	ds

		mov	ax,@DATA
		mov	ds,ax

;		while (polySize--)
transLoop:
		cmp	[polySize],0
		jnz	transPoly

		pop	ds
		pop	di
		mov	sp,bp
		pop	bp
		ret
transPoly:
		dec	[polySize]

;		{
;		  for (orderPos = 0; orderPos < 4; orderPos++)

		mov	[orderPos],0
transOrder:
		mov	bx,[orderPos]
		cmp	bx,4
		jne	transT
		jmp	transNext

;		  {
;		    if (order[orderPos] = 't')
transT:
		cmp	ds:[bx+order],'t'
		jne	transX

;		    {
;		      poly->x += pos.x;

		mov	ax,ds:[pos.x]
		add	es:[di.x],ax

;		      poly->y += pos.y;

		mov	ax,ds:[pos.y]
		add	es:[di.y],ax

;		      poly->z += pos.z;

		mov	ax,ds:[pos.z]
		add	es:[di.z],ax

;		    }

		jmp	transOrderNext

;		    else if (order[orderPos] = 'x'
transX:
		cmp	ds:[bx+order],'x'
		jne	transY

;		    {
;		      temp = (poly->y * Cos.x - poly->z * Sin.x) >> 16;

		mov	ax,es:[di.y]
		imul	Cos.x
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.z]
		imul	Sin.x
		sub	cx,ax
		sbb	bx,dx
		sal	bx,1
		push	bx

;		      poly->z = (polyIn->y * Sin.x + polyIn->z * Cos.x) >> 16;

		mov	ax,es:[di.y]
		imul	Sin.x
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.z]
		imul	Cos.x
		add	cx,ax
		adc	bx,dx
		sal	bx,1
		mov	es:[di.z],bx

;		      poly->y = temp;

		pop	ax
		mov	es:[di.y],ax

;		    }

		jmp	transOrderNext

;		    else if (order[orderPos] = 'y'
transY:
		cmp	ds:[bx+order],'y'
		jne	transZ

;		    {
;		      temp = (poly->x * Cos.y + poly->z * Sin.y) >> 16;

		mov	ax,es:[di.x]
		imul	Cos.y
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.z]
		imul	Sin.y
		add	cx,ax
		adc	bx,dx
		sal	bx,1
		push	bx

;		      poly->z = (poly->z * Cos.y - poly->x * Sin.y) >> 16;

		mov	ax,es:[di.z]
		imul	Cos.y
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.x]
		imul	Sin.y
		sub	cx,ax
		sbb	bx,dx
		sal	bx,1
		mov	es:[di.z],bx

;		      poly->x = temp;

		pop	ax
		mov	es:[di.x],ax

;		    }

		jmp	transOrderNext

;		    else if (order[orderPos] = 'z'
transZ:
		cmp	ds:[bx+order],'z'
		jne	transNext

;		    {
;		      temp = (poly->x * Cos.z - poly->y * Sin.z) >> 16;

		mov	ax,es:[di.x]
		imul	Cos.z
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.y]
		imul	Sin.z
		sub	cx,ax
		sbb	bx,dx
		sal	bx,1
		push	bx

;		      poly->y = (poly->x * Sin.z + poly->y * Cos.z) >> 16;

		mov	ax,es:[di.x]
		imul	Sin.z
		mov	cx,ax
		mov	bx,dx
		mov	ax,es:[di.y]
		imul	Cos.z
		add	cx,ax
		adc	bx,dx
		sal	bx,1
		mov	es:[di.y],bx

;		      poly->x = temp;

		pop	ax
		mov	es:[di.x],ax

;		    }

		jmp	transOrderNext

;		    else
;		      break;
transOrderNext:
		inc	[orderPos]
		jmp	transOrder

;		  }
transNext:
;		  poly++;


		add	di,6

;		}
		jmp	transLoop

transform	ENDP


;----------------------------------------------------------------------------
; void perspective (int polySize, struct POINT far *poly)
;----------------------------------------------------------------------------
;
perspective	PROC
;
ARG		polySize	: WORD, \
		poly		: DWORD
;
view		EQU	150
;
		push	bp
		mov	bp,sp
		push	si
		les	si,[poly]
		push	di
		mov	di,[polySize]

;		while (size-- > 0)
perspLoop:
		cmp	di,0
		jnz	perspPoly

		pop	di
		pop	si
		pop	bp
		ret
perspPoly:
		dec	di

;		{
;		  if ((temp = poly->z + view) != 0)

		mov	cx,es:[si.z]
		add	cx,view
		jz	perspIf

;		  {
;		    poly->x = (view * poly->x) / temp;

		mov	ax,es:[si.x]
		mov	dx,view
		imul	dx
		idiv	cx
		mov	es:[si.x],ax

;		    y = (view * y) / temp;

		mov	ax,es:[si.y]
		mov	dx,view
		imul	dx
		idiv	cx
		mov	es:[si.y],ax

;		  }
perspIf:
;		  poly->x = 160 + poly->x;

		mov	ax,160
		add	es:[si.x],ax

;		  poly->y = 100 - poly->y;

		mov	ax,100
		sub	ax,es:[si.y]
		mov	es:[si.y],ax

;		  poly++;

		add	si,6

;		}
		jmp	perspLoop

		ENDP


;----------------------------------------------------------------------------
; int testVisible (struct POINT far *poly)
;----------------------------------------------------------------------------
;
testVisible	PROC
;
ARG		poly		: DWORD
;
LOCAL		Vx,Vy,Vz	: WORD, \
		Wx,Wy,Wz	: WORD, \
		Xx,Xy,Xz	: WORD = AUTO_SIZE
;
		push	bp
		mov	bp,sp
		sub	sp,AUTO_SIZE
		push	si
		les	si,[poly]
		push	ds

		mov	ax,@DATA
		mov	ds,ax

;	        long Vx = poly[1].x - poly[0].x;

		mov	ax,es:[si.x+6]
		sub	ax,es:[si.x]
		mov	word ptr [Vx],ax

;		long Vy = poly[1].y - poly[0].y;

		mov	ax,es:[si.y+6]
		sub	ax,es:[si.y]
		mov	word ptr [Vy],ax

;		long Vz = poly[1].z - poly[0].z;

		mov	ax,es:[si.z+6]
		sub	ax,es:[si.z]
		mov	word ptr [Vz],ax

;		long Wx = poly[2].x - poly[0].x;

		mov	ax,es:[si.x+12]
		sub	ax,es:[si.x]
		mov	word ptr [Wx],ax

;		long Wy = poly[2].y - poly[0].y;

		mov	ax,es:[si.y+12]
		sub	ax,es:[si.y]
		mov	word ptr [Wy],ax

;		long Wz = poly[2].z - poly[0].z;

		mov	ax,es:[si.z+12]
		sub	ax,es:[si.z]
		mov	word ptr [Wz],ax

;		dlong Xx = (Vy * Wz - Vz * Wy) >> 8;

		mov	ax,word ptr [Vy]
		imul	word ptr [Wz]
		mov	bx,ax
		mov	cx,dx
		mov	ax,word ptr [Vz]
		imul	word ptr [Wy]
		sub	bx,ax
		sbb	cx,dx
		mov	byte ptr [Xx+0],bh
		mov	byte ptr [Xx+1],cl

;		dlong Xy = (Vz * Wx - Vx * Wz) >> 8;

		mov	ax,word ptr [Vz]
		imul	word ptr [Wx]
		mov	bx,ax
		mov	cx,dx
		mov	ax,word ptr [Vx]
		imul	word ptr [Wz]
		sub	bx,ax
		sbb	cx,dx
		mov	byte ptr [Xy+0],bh
		mov	byte ptr [Xy+1],cl

;		dlong Xz = (Vx * Wy - Vy * Wx) >> 8;

		mov	ax,word ptr [Vx]
		imul	word ptr [Wy]
		mov	bx,ax
		mov	cx,dx
		mov	ax,word ptr [Vy]
		imul	word ptr [Wx]
		sub	bx,ax
		sbb	cx,dx
		mov	byte ptr [Xz+0],bh
		mov	byte ptr [Xz+1],cl

;		dlong Sx = poly[0].x + pos.x;
;		dlong Sy = poly[0].y + pos.y;
;		dlong Sz = poly[0].z + pos.z;
;
;		dlong q = Sx * Cx + Sy * Cy + Sz * Cz;

		mov	ax,es:[si.x]
		add	ax,ds:[pos.x]
		imul	word ptr [Xx]
		mov	bx,ax
		mov	cx,dx
		mov	ax,es:[si.y]
		add	ax,ds:[pos.y]
		imul	word ptr [Xy]
		add	bx,ax
		adc	cx,dx
		mov	ax,es:[si.z]
		add	ax,ds:[pos.z]
		imul	word ptr [Xz]
		add	bx,ax
		adc	cx,dx

;		return q < 0;

		cmp	cx,0
		jl	testVisibleRetTrue
		mov	ax,0
		jmp	testVisibleRet

testVisibleRetTrue:
		mov	ax,1

testVisibleRet:
		pop	ds
		pop	si
		mov	sp,bp
		pop	bp
		ret

testVisible	ENDP


;----------------------------------------------------------------------------
; int intersect (int a1, int b1, int a2, int b2, int i)
;----------------------------------------------------------------------------
;
intersect	PROC
;
ARG		a1		: WORD, \
		b1		: WORD, \
		a2		: WORD, \
		b2		: WORD, \
		i		: WORD
;
		push	bp
		mov	bp,sp

;		if (a2 == a1)

		mov	cx,[a2]
		sub	cx,[a1]
		jnz	intersectIf

;		  return b1;

		mov	ax,[b1]
		jmp	intersectRet

;		else
intersectIf:
;		  return ((b2 - b1) * (i - a1)) / (a2 - a1) + b1;

		mov	ax,[b2]
		sub	ax,[b1]
		mov	bx,[i]
		sub	bx,[a1]
		imul	bx
		idiv	cx
		add	ax,[b1]

intersectRet:
		pop	bp
		ret

		ENDP


;----------------------------------------------------------------------------
; int intSinMul (int angle, int v)
;----------------------------------------------------------------------------
;
intSinMul	PROC
;
ARG		angle		: WORD, \
		v		: WORD
;
		push	bp
		mov	bp,sp

;		return	(v * intSin (angle)) >> 16

		mov	ax,[angle]
		push	ax
		call	intSin
		add	sp,2
		imul	[v]
		sal	dx,1
		mov	ax,dx

		pop	bp
		ret

intSinMul	ENDP


;----------------------------------------------------------------------------
; int intCosMul (int angle, int v)
;----------------------------------------------------------------------------
;
intCosMul	PROC
;
ARG		angle		: WORD, \
		v		: WORD
;
		push	bp
		mov	bp,sp

;		return	(v * intCos (angle)) >> 16

		mov	ax,[angle]
		push	ax
		call	intCos
		add	sp,2
		imul	[v]
		sal	dx,1
		mov	ax,dx

		pop	bp
		ret

intCosMul	ENDP


;----------------------------------------------------------------------------
; int intSin (int angle)
;----------------------------------------------------------------------------
;
intSin		PROC
;
ARG		angle		: WORD
;
		push	bp
		mov	bp,sp

;		if (angle < 0)

		mov	bx,[angle]
		cmp	bx,0
		jnl	intSinIf1

;		  angle += 3600;

		add	bx,3600

;		return sinusTable[angle];
intSinIf1:
		shl	bx,1
		mov	ax,cs:[bx+word ptr sinusTable]

		pop	bp
		ret

intSin		ENDP


;----------------------------------------------------------------------------
; int intCos (int angle)
;----------------------------------------------------------------------------
;
intCos		PROC
;
ARG		angle		: WORD
;
		push	bp
		mov	bp,sp

;		if (angle < 0)

		mov	bx,[angle]
		cmp	bx,0
		jnl	intCosIf1

;		  angle += 3600;

		add	bx,3600

;		if (angle >= 2700)
intCosIf1:
		cmp	bx,2700
		jl	intCosIf2

;		  return sinusTable[angle - 2700];

		sub	bx,2700
		jmp	intCosRet

;		else
;		  return sinusTable[angle + 900];

intCosIf2:
		add	bx,900
intCosRet:
		shl	bx,1
		mov	ax,cs:[bx+word ptr sinusTable]

		pop	bp
		ret

intCos		ENDP


;----------------------------------------------------------------------------
; int intArcSin (int sinAngle)
;----------------------------------------------------------------------------
;
intArcSin	PROC
;
ARG		sinAngle	: WORD
;
		push	bp
		mov	bp,sp
		push	si

;		angle = 1800;

		mov	bx,1800

;		approximator = 512;

		mov	cx,512

;		while (approximator != 0)
intArcSinLoop:
		cmp	cx,0
		je	intArcSinRet

;		{
;		  if (sinusTable[angle] <= sinAngle)

		mov	si,bx
		shl	si,1
		cmp	cs:[si+word ptr sinusTable],ax
		jg	intArcSinElse1

;		    if (sinusTable[angle-1] > sinAngle)
;		      break;

		cmp	cs:[si+word ptr sinusTable-2],ax
		jg	intArcSinRet

;		    else
;		      angle = angle - approximator;

		sub	bx,cx
		jmp	intArcSinIf1

;		  else
intArcSinElse1:
;		    angle = angle + approximator;

		add	bx,cx
intArcSinIf1:
;		  approximator = approximator / 2;

		shr	cx,1

;		}
		jmp	intArcSinLoop
intArcSinRet:
;		angle = 1800 - angle;

		mov	ax,1800
		sub	ax,bx

;		if (angle > 900)

		cmp	ax,900
		jle	intArcSinElse2

;		  angle = 900;

		mov	ax,900
		jmp	intArcSinIf2

intArcSinElse2:

;		else if (angle < -900)

		cmp	ax,-900
		jge	intArcSinIf2

;		  angle = -900;

		mov	ax,-900
intArcSinIf2:

;		return angle;

		pop	si
		pop	bp
		ret

intArcSin	ENDP


;----------------------------------------------------------------------------
; int intArcCos (int sinAngle)
;----------------------------------------------------------------------------
;
intArcCos	PROC
;
ARG		sinAngle	: WORD
;
		push	bp
		mov	bp,sp
		push	si

;		return 900 - intArcSin (sinAngle);

		mov	ax,[sinAngle]
		push	ax
		call	intArcSin
		add	sp,2
		xchg	ax,bx
		mov	ax,900
		sub	ax,bx

		pop	si
		pop	bp
		ret

intArcCos	ENDP


;----------------------------------------------------------------------------
; int SinusTable[3600]
;----------------------------------------------------------------------------
;
sinusTable:
;
DW 00000h,00039h,00072h,000ABh,000E4h,0011Dh,00157h,00190h,001C9h,00202h
DW 0023Bh,00275h,002AEh,002E7h,00320h,00359h,00392h,003CCh,00405h,0043Eh
DW 00477h,004B0h,004E9h,00522h,0055Ch,00595h,005CEh,00607h,00640h,00679h
DW 006B2h,006ECh,00725h,0075Eh,00797h,007D0h,00809h,00842h,0087Bh,008B4h
DW 008EDh,00926h,0095Fh,00998h,009D1h,00A0Ah,00A43h,00A7Ch,00AB5h,00AEEh
DW 00B27h,00B60h,00B99h,00BD2h,00C0Bh,00C44h,00C7Dh,00CB6h,00CEFh,00D28h
DW 00D61h,00D99h,00DD2h,00E0Bh,00E44h,00E7Dh,00EB6h,00EEEh,00F27h,00F60h
DW 00F99h,00FD2h,0100Ah,01043h,0107Ch,010B4h,010EDh,01126h,0115Eh,01197h
DW 011D0h,01208h,01241h,0127Ah,012B2h,012EBh,01323h,0135Ch,01394h,013CDh
DW 01405h,0143Eh,01476h,014AFh,014E7h,01520h,01558h,01590h,015C9h,01601h
DW 01639h,01672h,016AAh,016E2h,0171Bh,01753h,0178Bh,017C3h,017FBh,01834h
DW 0186Ch,018A4h,018DCh,01914h,0194Ch,01984h,019BCh,019F4h,01A2Ch,01A64h
DW 01A9Ch,01AD4h,01B0Ch,01B44h,01B7Ch,01BB4h,01BEBh,01C23h,01C5Bh,01C93h
DW 01CCAh,01D02h,01D3Ah,01D72h,01DA9h,01DE1h,01E18h,01E50h,01E88h,01EBFh
DW 01EF7h,01F2Eh,01F65h,01F9Dh,01FD4h,0200Ch,02043h,0207Ah,020B2h,020E9h
DW 02120h,02157h,0218Fh,021C6h,021FDh,02234h,0226Bh,022A2h,022D9h,02310h
DW 02347h,0237Eh,023B5h,023ECh,02423h,0245Ah,02491h,024C7h,024FEh,02535h
DW 0256Ch,025A2h,025D9h,02610h,02646h,0267Dh,026B3h,026EAh,02720h,02757h
DW 0278Dh,027C3h,027FAh,02830h,02866h,0289Dh,028D3h,02909h,0293Fh,02975h
DW 029ABh,029E1h,02A17h,02A4Dh,02A83h,02AB9h,02AEFh,02B25h,02B5Bh,02B91h
DW 02BC6h,02BFCh,02C32h,02C68h,02C9Dh,02CD3h,02D08h,02D3Eh,02D73h,02DA9h
DW 02DDEh,02E14h,02E49h,02E7Eh,02EB3h,02EE9h,02F1Eh,02F53h,02F88h,02FBDh
DW 02FF2h,03027h,0305Ch,03091h,030C6h,030FBh,03130h,03164h,03199h,031CEh
DW 03203h,03237h,0326Ch,032A0h,032D5h,03309h,0333Eh,03372h,033A6h,033DBh
DW 0340Fh,03443h,03477h,034ACh,034E0h,03514h,03548h,0357Ch,035B0h,035E4h
DW 03617h,0364Bh,0367Fh,036B3h,036E6h,0371Ah,0374Eh,03781h,037B5h,037E8h
DW 0381Ch,0384Fh,03882h,038B6h,038E9h,0391Ch,0394Fh,03982h,039B5h,039E8h
DW 03A1Bh,03A4Eh,03A81h,03AB4h,03AE7h,03B1Ah,03B4Ch,03B7Fh,03BB2h,03BE4h
DW 03C17h,03C49h,03C7Ch,03CAEh,03CE0h,03D13h,03D45h,03D77h,03DA9h,03DDBh
DW 03E0Dh,03E3Fh,03E71h,03EA3h,03ED5h,03F07h,03F38h,03F6Ah,03F9Ch,03FCDh
DW 03FFFh,04031h,04062h,04093h,040C5h,040F6h,04127h,04158h,0418Ah,041BBh
DW 041ECh,0421Dh,0424Eh,0427Fh,042AFh,042E0h,04311h,04342h,04372h,043A3h
DW 043D3h,04404h,04434h,04465h,04495h,044C5h,044F5h,04526h,04556h,04586h
DW 045B6h,045E6h,04616h,04645h,04675h,046A5h,046D4h,04704h,04734h,04763h
DW 04793h,047C2h,047F1h,04821h,04850h,0487Fh,048AEh,048DDh,0490Ch,0493Bh
DW 0496Ah,04999h,049C7h,049F6h,04A25h,04A53h,04A82h,04AB0h,04ADFh,04B0Dh
DW 04B3Bh,04B6Ah,04B98h,04BC6h,04BF4h,04C22h,04C50h,04C7Eh,04CACh,04CD9h
DW 04D07h,04D35h,04D62h,04D90h,04DBDh,04DEBh,04E18h,04E45h,04E73h,04EA0h
DW 04ECDh,04EFAh,04F27h,04F54h,04F81h,04FADh,04FDAh,05007h,05033h,05060h
DW 0508Ch,050B9h,050E5h,05111h,0513Eh,0516Ah,05196h,051C2h,051EEh,0521Ah
DW 05246h,05271h,0529Dh,052C9h,052F4h,05320h,0534Bh,05377h,053A2h,053CDh
DW 053F9h,05424h,0544Fh,0547Ah,054A5h,054D0h,054FAh,05525h,05550h,0557Ah
DW 055A5h,055CFh,055FAh,05624h,0564Eh,05679h,056A3h,056CDh,056F7h,05721h
DW 0574Bh,05774h,0579Eh,057C8h,057F1h,0581Bh,05844h,0586Eh,05897h,058C0h
DW 058E9h,05912h,0593Ch,05964h,0598Dh,059B6h,059DFh,05A08h,05A30h,05A59h
DW 05A81h,05AAAh,05AD2h,05AFAh,05B22h,05B4Bh,05B73h,05B9Bh,05BC3h,05BEAh
DW 05C12h,05C3Ah,05C61h,05C89h,05CB0h,05CD8h,05CFFh,05D26h,05D4Eh,05D75h
DW 05D9Ch,05DC3h,05DEAh,05E10h,05E37h,05E5Eh,05E84h,05EABh,05ED1h,05EF8h
DW 05F1Eh,05F44h,05F6Bh,05F91h,05FB7h,05FDDh,06002h,06028h,0604Eh,06074h
DW 06099h,060BFh,060E4h,06109h,0612Fh,06154h,06179h,0619Eh,061C3h,061E8h
DW 0620Ch,06231h,06256h,0627Ah,0629Fh,062C3h,062E8h,0630Ch,06330h,06354h
DW 06378h,0639Ch,063C0h,063E4h,06408h,0642Bh,0644Fh,06472h,06496h,064B9h
DW 064DCh,064FFh,06523h,06546h,06568h,0658Bh,065AEh,065D1h,065F3h,06616h
DW 06638h,0665Bh,0667Dh,0669Fh,066C1h,066E3h,06705h,06727h,06749h,0676Bh
DW 0678Dh,067AEh,067D0h,067F1h,06812h,06834h,06855h,06876h,06897h,068B8h
DW 068D9h,068F9h,0691Ah,0693Bh,0695Bh,0697Ch,0699Ch,069BCh,069DCh,069FDh
DW 06A1Dh,06A3Dh,06A5Ch,06A7Ch,06A9Ch,06ABBh,06ADBh,06AFAh,06B1Ah,06B39h
DW 06B58h,06B77h,06B96h,06BB5h,06BD4h,06BF3h,06C12h,06C30h,06C4Fh,06C6Dh
DW 06C8Bh,06CAAh,06CC8h,06CE6h,06D04h,06D22h,06D40h,06D5Eh,06D7Bh,06D99h
DW 06DB6h,06DD4h,06DF1h,06E0Eh,06E2Bh,06E49h,06E65h,06E82h,06E9Fh,06EBCh
DW 06ED9h,06EF5h,06F12h,06F2Eh,06F4Ah,06F66h,06F83h,06F9Fh,06FBBh,06FD6h
DW 06FF2h,0700Eh,07029h,07045h,07060h,0707Ch,07097h,070B2h,070CDh,070E8h
DW 07103h,0711Eh,07139h,07153h,0716Eh,07188h,071A3h,071BDh,071D7h,071F1h
DW 0720Bh,07225h,0723Fh,07259h,07272h,0728Ch,072A5h,072BFh,072D8h,072F1h
DW 0730Ah,07323h,0733Ch,07355h,0736Eh,07387h,0739Fh,073B8h,073D0h,073E8h
DW 07400h,07419h,07431h,07449h,07460h,07478h,07490h,074A7h,074BFh,074D6h
DW 074EEh,07505h,0751Ch,07533h,0754Ah,07561h,07578h,0758Eh,075A5h,075BBh
DW 075D2h,075E8h,075FEh,07614h,0762Ah,07640h,07656h,0766Ch,07682h,07697h
DW 076ADh,076C2h,076D7h,076ECh,07701h,07716h,0772Bh,07740h,07755h,0776Ah
DW 0777Eh,07793h,077A7h,077BBh,077CFh,077E3h,077F7h,0780Bh,0781Fh,07833h
DW 07846h,0785Ah,0786Dh,07881h,07894h,078A7h,078BAh,078CDh,078E0h,078F3h
DW 07905h,07918h,0792Ah,0793Dh,0794Fh,07961h,07973h,07985h,07997h,079A9h
DW 079BBh,079CCh,079DEh,079EFh,07A01h,07A12h,07A23h,07A34h,07A45h,07A56h
DW 07A67h,07A77h,07A88h,07A98h,07AA9h,07AB9h,07AC9h,07AD9h,07AE9h,07AF9h
DW 07B09h,07B19h,07B28h,07B38h,07B47h,07B57h,07B66h,07B75h,07B84h,07B93h
DW 07BA2h,07BB1h,07BBFh,07BCEh,07BDCh,07BEBh,07BF9h,07C07h,07C15h,07C23h
DW 07C31h,07C3Fh,07C4Dh,07C5Ah,07C68h,07C75h,07C82h,07C90h,07C9Dh,07CAAh
DW 07CB7h,07CC4h,07CD0h,07CDDh,07CE9h,07CF6h,07D02h,07D0Eh,07D1Ah,07D27h
DW 07D32h,07D3Eh,07D4Ah,07D56h,07D61h,07D6Dh,07D78h,07D83h,07D8Eh,07D9Ah
DW 07DA4h,07DAFh,07DBAh,07DC5h,07DCFh,07DDAh,07DE4h,07DEEh,07DF9h,07E03h
DW 07E0Dh,07E17h,07E20h,07E2Ah,07E34h,07E3Dh,07E47h,07E50h,07E59h,07E62h
DW 07E6Bh,07E74h,07E7Dh,07E85h,07E8Eh,07E97h,07E9Fh,07EA7h,07EAFh,07EB8h
DW 07EC0h,07EC8h,07ECFh,07ED7h,07EDFh,07EE6h,07EEEh,07EF5h,07EFCh,07F03h
DW 07F0Ah,07F11h,07F18h,07F1Fh,07F25h,07F2Ch,07F32h,07F39h,07F3Fh,07F45h
DW 07F4Bh,07F51h,07F57h,07F5Ch,07F62h,07F68h,07F6Dh,07F72h,07F78h,07F7Dh
DW 07F82h,07F87h,07F8Ch,07F90h,07F95h,07F99h,07F9Eh,07FA2h,07FA7h,07FABh
DW 07FAFh,07FB3h,07FB6h,07FBAh,07FBEh,07FC1h,07FC5h,07FC8h,07FCBh,07FCFh
DW 07FD2h,07FD5h,07FD7h,07FDAh,07FDDh,07FDFh,07FE2h,07FE4h,07FE6h,07FE8h
DW 07FEBh,07FECh,07FEEh,07FF0h,07FF2h,07FF3h,07FF5h,07FF6h,07FF7h,07FF8h
DW 07FFAh,07FFAh,07FFBh,07FFCh,07FFDh,07FFDh,07FFEh,07FFEh,07FFEh,07FFFh
DW 07FFFh,07FFFh,07FFEh,07FFEh,07FFEh,07FFDh,07FFDh,07FFCh,07FFBh,07FFAh
DW 07FFAh,07FF8h,07FF7h,07FF6h,07FF5h,07FF3h,07FF2h,07FF0h,07FEEh,07FECh
DW 07FEBh,07FE8h,07FE6h,07FE4h,07FE2h,07FDFh,07FDDh,07FDAh,07FD7h,07FD5h
DW 07FD2h,07FCFh,07FCBh,07FC8h,07FC5h,07FC1h,07FBEh,07FBAh,07FB6h,07FB3h
DW 07FAFh,07FABh,07FA7h,07FA2h,07F9Eh,07F99h,07F95h,07F90h,07F8Ch,07F87h
DW 07F82h,07F7Dh,07F78h,07F72h,07F6Dh,07F68h,07F62h,07F5Ch,07F57h,07F51h
DW 07F4Bh,07F45h,07F3Fh,07F39h,07F32h,07F2Ch,07F25h,07F1Fh,07F18h,07F11h
DW 07F0Ah,07F03h,07EFCh,07EF5h,07EEEh,07EE6h,07EDFh,07ED7h,07ECFh,07EC8h
DW 07EC0h,07EB8h,07EAFh,07EA7h,07E9Fh,07E97h,07E8Eh,07E85h,07E7Dh,07E74h
DW 07E6Bh,07E62h,07E59h,07E50h,07E47h,07E3Dh,07E34h,07E2Ah,07E20h,07E17h
DW 07E0Dh,07E03h,07DF9h,07DEEh,07DE4h,07DDAh,07DCFh,07DC5h,07DBAh,07DAFh
DW 07DA4h,07D9Ah,07D8Eh,07D83h,07D78h,07D6Dh,07D61h,07D56h,07D4Ah,07D3Eh
DW 07D32h,07D27h,07D1Ah,07D0Eh,07D02h,07CF6h,07CE9h,07CDDh,07CD0h,07CC4h
DW 07CB7h,07CAAh,07C9Dh,07C90h,07C82h,07C75h,07C68h,07C5Ah,07C4Dh,07C3Fh
DW 07C31h,07C23h,07C15h,07C07h,07BF9h,07BEBh,07BDCh,07BCEh,07BBFh,07BB1h
DW 07BA2h,07B93h,07B84h,07B75h,07B66h,07B57h,07B47h,07B38h,07B28h,07B19h
DW 07B09h,07AF9h,07AE9h,07AD9h,07AC9h,07AB9h,07AA9h,07A98h,07A88h,07A77h
DW 07A67h,07A56h,07A45h,07A34h,07A23h,07A12h,07A01h,079EFh,079DEh,079CCh
DW 079BBh,079A9h,07997h,07985h,07973h,07961h,0794Fh,0793Dh,0792Ah,07918h
DW 07905h,078F3h,078E0h,078CDh,078BAh,078A7h,07894h,07881h,0786Dh,0785Ah
DW 07846h,07833h,0781Fh,0780Bh,077F7h,077E3h,077CFh,077BBh,077A7h,07793h
DW 0777Eh,0776Ah,07755h,07740h,0772Bh,07716h,07701h,076ECh,076D7h,076C2h
DW 076ADh,07697h,07682h,0766Ch,07656h,07640h,0762Ah,07614h,075FEh,075E8h
DW 075D2h,075BBh,075A5h,0758Eh,07578h,07561h,0754Ah,07533h,0751Ch,07505h
DW 074EEh,074D6h,074BFh,074A7h,07490h,07478h,07460h,07449h,07431h,07419h
DW 07400h,073E8h,073D0h,073B8h,0739Fh,07387h,0736Eh,07355h,0733Ch,07323h
DW 0730Ah,072F1h,072D8h,072BFh,072A5h,0728Ch,07272h,07259h,0723Fh,07225h
DW 0720Bh,071F1h,071D7h,071BDh,071A3h,07188h,0716Eh,07153h,07139h,0711Eh
DW 07103h,070E8h,070CDh,070B2h,07097h,0707Ch,07060h,07045h,07029h,0700Eh
DW 06FF2h,06FD6h,06FBBh,06F9Fh,06F83h,06F66h,06F4Ah,06F2Eh,06F12h,06EF5h
DW 06ED9h,06EBCh,06E9Fh,06E82h,06E65h,06E49h,06E2Bh,06E0Eh,06DF1h,06DD4h
DW 06DB6h,06D99h,06D7Bh,06D5Eh,06D40h,06D22h,06D04h,06CE6h,06CC8h,06CAAh
DW 06C8Bh,06C6Dh,06C4Fh,06C30h,06C12h,06BF3h,06BD4h,06BB5h,06B96h,06B77h
DW 06B58h,06B39h,06B1Ah,06AFAh,06ADBh,06ABBh,06A9Ch,06A7Ch,06A5Ch,06A3Dh
DW 06A1Dh,069FDh,069DCh,069BCh,0699Ch,0697Ch,0695Bh,0693Bh,0691Ah,068F9h
DW 068D9h,068B8h,06897h,06876h,06855h,06834h,06812h,067F1h,067D0h,067AEh
DW 0678Dh,0676Bh,06749h,06727h,06705h,066E3h,066C1h,0669Fh,0667Dh,0665Bh
DW 06638h,06616h,065F3h,065D1h,065AEh,0658Bh,06568h,06546h,06523h,064FFh
DW 064DCh,064B9h,06496h,06472h,0644Fh,0642Bh,06408h,063E4h,063C0h,0639Ch
DW 06378h,06354h,06330h,0630Ch,062E8h,062C3h,0629Fh,0627Ah,06256h,06231h
DW 0620Ch,061E8h,061C3h,0619Eh,06179h,06154h,0612Fh,06109h,060E4h,060BFh
DW 06099h,06074h,0604Eh,06028h,06002h,05FDDh,05FB7h,05F91h,05F6Bh,05F44h
DW 05F1Eh,05EF8h,05ED1h,05EABh,05E84h,05E5Eh,05E37h,05E10h,05DEAh,05DC3h
DW 05D9Ch,05D75h,05D4Eh,05D26h,05CFFh,05CD8h,05CB0h,05C89h,05C61h,05C3Ah
DW 05C12h,05BEAh,05BC3h,05B9Bh,05B73h,05B4Bh,05B22h,05AFAh,05AD2h,05AAAh
DW 05A81h,05A59h,05A30h,05A08h,059DFh,059B6h,0598Dh,05964h,0593Ch,05912h
DW 058E9h,058C0h,05897h,0586Eh,05844h,0581Bh,057F1h,057C8h,0579Eh,05774h
DW 0574Bh,05721h,056F7h,056CDh,056A3h,05679h,0564Eh,05624h,055FAh,055CFh
DW 055A5h,0557Ah,05550h,05525h,054FAh,054D0h,054A5h,0547Ah,0544Fh,05424h
DW 053F9h,053CDh,053A2h,05377h,0534Bh,05320h,052F4h,052C9h,0529Dh,05271h
DW 05246h,0521Ah,051EEh,051C2h,05196h,0516Ah,0513Eh,05111h,050E5h,050B9h
DW 0508Ch,05060h,05033h,05007h,04FDAh,04FADh,04F81h,04F54h,04F27h,04EFAh
DW 04ECDh,04EA0h,04E73h,04E45h,04E18h,04DEBh,04DBDh,04D90h,04D62h,04D35h
DW 04D07h,04CD9h,04CACh,04C7Eh,04C50h,04C22h,04BF4h,04BC6h,04B98h,04B6Ah
DW 04B3Bh,04B0Dh,04ADFh,04AB0h,04A82h,04A53h,04A25h,049F6h,049C7h,04999h
DW 0496Ah,0493Bh,0490Ch,048DDh,048AEh,0487Fh,04850h,04821h,047F1h,047C2h
DW 04793h,04763h,04734h,04704h,046D4h,046A5h,04675h,04645h,04616h,045E6h
DW 045B6h,04586h,04556h,04526h,044F5h,044C5h,04495h,04465h,04434h,04404h
DW 043D3h,043A3h,04372h,04342h,04311h,042E0h,042AFh,0427Fh,0424Eh,0421Dh
DW 041ECh,041BBh,0418Ah,04158h,04127h,040F6h,040C5h,04093h,04062h,04031h
DW 03FFFh,03FCDh,03F9Ch,03F6Ah,03F38h,03F07h,03ED5h,03EA3h,03E71h,03E3Fh
DW 03E0Dh,03DDBh,03DA9h,03D77h,03D45h,03D13h,03CE0h,03CAEh,03C7Ch,03C49h
DW 03C17h,03BE4h,03BB2h,03B7Fh,03B4Ch,03B1Ah,03AE7h,03AB4h,03A81h,03A4Eh
DW 03A1Bh,039E8h,039B5h,03982h,0394Fh,0391Ch,038E9h,038B6h,03882h,0384Fh
DW 0381Ch,037E8h,037B5h,03781h,0374Eh,0371Ah,036E6h,036B3h,0367Fh,0364Bh
DW 03617h,035E4h,035B0h,0357Ch,03548h,03514h,034E0h,034ACh,03477h,03443h
DW 0340Fh,033DBh,033A6h,03372h,0333Eh,03309h,032D5h,032A0h,0326Ch,03237h
DW 03203h,031CEh,03199h,03164h,03130h,030FBh,030C6h,03091h,0305Ch,03027h
DW 02FF2h,02FBDh,02F88h,02F53h,02F1Eh,02EE9h,02EB3h,02E7Eh,02E49h,02E14h
DW 02DDEh,02DA9h,02D73h,02D3Eh,02D08h,02CD3h,02C9Dh,02C68h,02C32h,02BFCh
DW 02BC6h,02B91h,02B5Bh,02B25h,02AEFh,02AB9h,02A83h,02A4Dh,02A17h,029E1h
DW 029ABh,02975h,0293Fh,02909h,028D3h,0289Dh,02866h,02830h,027FAh,027C3h
DW 0278Dh,02757h,02720h,026EAh,026B3h,0267Dh,02646h,02610h,025D9h,025A2h
DW 0256Ch,02535h,024FEh,024C7h,02491h,0245Ah,02423h,023ECh,023B5h,0237Eh
DW 02347h,02310h,022D9h,022A2h,0226Bh,02234h,021FDh,021C6h,0218Fh,02157h
DW 02120h,020E9h,020B2h,0207Ah,02043h,0200Ch,01FD4h,01F9Dh,01F65h,01F2Eh
DW 01EF7h,01EBFh,01E88h,01E50h,01E18h,01DE1h,01DA9h,01D72h,01D3Ah,01D02h
DW 01CCAh,01C93h,01C5Bh,01C23h,01BEBh,01BB4h,01B7Ch,01B44h,01B0Ch,01AD4h
DW 01A9Ch,01A64h,01A2Ch,019F4h,019BCh,01984h,0194Ch,01914h,018DCh,018A4h
DW 0186Ch,01834h,017FBh,017C3h,0178Bh,01753h,0171Bh,016E2h,016AAh,01672h
DW 01639h,01601h,015C9h,01590h,01558h,01520h,014E7h,014AFh,01476h,0143Eh
DW 01405h,013CDh,01394h,0135Ch,01323h,012EBh,012B2h,0127Ah,01241h,01208h
DW 011D0h,01197h,0115Eh,01126h,010EDh,010B4h,0107Ch,01043h,0100Ah,00FD2h
DW 00F99h,00F60h,00F27h,00EEEh,00EB6h,00E7Dh,00E44h,00E0Bh,00DD2h,00D99h
DW 00D61h,00D28h,00CEFh,00CB6h,00C7Dh,00C44h,00C0Bh,00BD2h,00B99h,00B60h
DW 00B27h,00AEEh,00AB5h,00A7Ch,00A43h,00A0Ah,009D1h,00998h,0095Fh,00926h
DW 008EDh,008B4h,0087Bh,00842h,00809h,007D0h,00797h,0075Eh,00725h,006ECh
DW 006B2h,00679h,00640h,00607h,005CEh,00595h,0055Ch,00522h,004E9h,004B0h
DW 00477h,0043Eh,00405h,003CCh,00392h,00359h,00320h,002E7h,002AEh,00275h
DW 0023Bh,00202h,001C9h,00190h,00157h,0011Dh,000E4h,000ABh,00072h,00039h
DW 00000h,0FFC7h,0FF8Eh,0FF55h,0FF1Ch,0FEE3h,0FEA9h,0FE70h,0FE37h,0FDFEh
DW 0FDC5h,0FD8Bh,0FD52h,0FD19h,0FCE0h,0FCA7h,0FC6Eh,0FC34h,0FBFBh,0FBC2h
DW 0FB89h,0FB50h,0FB17h,0FADEh,0FAA4h,0FA6Bh,0FA32h,0F9F9h,0F9C0h,0F987h
DW 0F94Eh,0F914h,0F8DBh,0F8A2h,0F869h,0F830h,0F7F7h,0F7BEh,0F785h,0F74Ch
DW 0F713h,0F6DAh,0F6A1h,0F668h,0F62Fh,0F5F6h,0F5BDh,0F584h,0F54Bh,0F512h
DW 0F4D9h,0F4A0h,0F467h,0F42Eh,0F3F5h,0F3BCh,0F383h,0F34Ah,0F311h,0F2D8h
DW 0F29Fh,0F267h,0F22Eh,0F1F5h,0F1BCh,0F183h,0F14Ah,0F112h,0F0D9h,0F0A0h
DW 0F067h,0F02Eh,0EFF6h,0EFBDh,0EF84h,0EF4Ch,0EF13h,0EEDAh,0EEA2h,0EE69h
DW 0EE30h,0EDF8h,0EDBFh,0ED86h,0ED4Eh,0ED15h,0ECDDh,0ECA4h,0EC6Ch,0EC33h
DW 0EBFBh,0EBC2h,0EB8Ah,0EB51h,0EB19h,0EAE0h,0EAA8h,0EA70h,0EA37h,0E9FFh
DW 0E9C7h,0E98Eh,0E956h,0E91Eh,0E8E5h,0E8ADh,0E875h,0E83Dh,0E805h,0E7CCh
DW 0E794h,0E75Ch,0E724h,0E6ECh,0E6B4h,0E67Ch,0E644h,0E60Ch,0E5D4h,0E59Ch
DW 0E564h,0E52Ch,0E4F4h,0E4BCh,0E484h,0E44Ch,0E415h,0E3DDh,0E3A5h,0E36Dh
DW 0E336h,0E2FEh,0E2C6h,0E28Eh,0E257h,0E21Fh,0E1E8h,0E1B0h,0E178h,0E141h
DW 0E109h,0E0D2h,0E09Bh,0E063h,0E02Ch,0DFF4h,0DFBDh,0DF86h,0DF4Eh,0DF17h
DW 0DEE0h,0DEA9h,0DE71h,0DE3Ah,0DE03h,0DDCCh,0DD95h,0DD5Eh,0DD27h,0DCF0h
DW 0DCB9h,0DC82h,0DC4Bh,0DC14h,0DBDDh,0DBA6h,0DB6Fh,0DB39h,0DB02h,0DACBh
DW 0DA94h,0DA5Eh,0DA27h,0D9F0h,0D9BAh,0D983h,0D94Dh,0D916h,0D8E0h,0D8A9h
DW 0D873h,0D83Dh,0D806h,0D7D0h,0D79Ah,0D763h,0D72Dh,0D6F7h,0D6C1h,0D68Bh
DW 0D655h,0D61Fh,0D5E9h,0D5B3h,0D57Dh,0D547h,0D511h,0D4DBh,0D4A5h,0D46Fh
DW 0D43Ah,0D404h,0D3CEh,0D398h,0D363h,0D32Dh,0D2F8h,0D2C2h,0D28Dh,0D257h
DW 0D222h,0D1ECh,0D1B7h,0D182h,0D14Dh,0D117h,0D0E2h,0D0ADh,0D078h,0D043h
DW 0D00Eh,0CFD9h,0CFA4h,0CF6Fh,0CF3Ah,0CF05h,0CED0h,0CE9Ch,0CE67h,0CE32h
DW 0CDFDh,0CDC9h,0CD94h,0CD60h,0CD2Bh,0CCF7h,0CCC2h,0CC8Eh,0CC5Ah,0CC25h
DW 0CBF1h,0CBBDh,0CB89h,0CB54h,0CB20h,0CAECh,0CAB8h,0CA84h,0CA50h,0CA1Ch
DW 0C9E9h,0C9B5h,0C981h,0C94Dh,0C91Ah,0C8E6h,0C8B2h,0C87Fh,0C84Bh,0C818h
DW 0C7E4h,0C7B1h,0C77Eh,0C74Ah,0C717h,0C6E4h,0C6B1h,0C67Eh,0C64Bh,0C618h
DW 0C5E5h,0C5B2h,0C57Fh,0C54Ch,0C519h,0C4E6h,0C4B4h,0C481h,0C44Eh,0C41Ch
DW 0C3E9h,0C3B7h,0C384h,0C352h,0C320h,0C2EDh,0C2BBh,0C289h,0C257h,0C225h
DW 0C1F3h,0C1C1h,0C18Fh,0C15Dh,0C12Bh,0C0F9h,0C0C8h,0C096h,0C064h,0C033h
DW 0C001h,0BFCFh,0BF9Eh,0BF6Dh,0BF3Bh,0BF0Ah,0BED9h,0BEA8h,0BE76h,0BE45h
DW 0BE14h,0BDE3h,0BDB2h,0BD81h,0BD51h,0BD20h,0BCEFh,0BCBEh,0BC8Eh,0BC5Dh
DW 0BC2Dh,0BBFCh,0BBCCh,0BB9Bh,0BB6Bh,0BB3Bh,0BB0Bh,0BADAh,0BAAAh,0BA7Ah
DW 0BA4Ah,0BA1Ah,0B9EAh,0B9BBh,0B98Bh,0B95Bh,0B92Ch,0B8FCh,0B8CCh,0B89Dh
DW 0B86Dh,0B83Eh,0B80Fh,0B7DFh,0B7B0h,0B781h,0B752h,0B723h,0B6F4h,0B6C5h
DW 0B696h,0B667h,0B639h,0B60Ah,0B5DBh,0B5ADh,0B57Eh,0B550h,0B521h,0B4F3h
DW 0B4C5h,0B496h,0B468h,0B43Ah,0B40Ch,0B3DEh,0B3B0h,0B382h,0B354h,0B327h
DW 0B2F9h,0B2CBh,0B29Eh,0B270h,0B243h,0B215h,0B1E8h,0B1BBh,0B18Dh,0B160h
DW 0B133h,0B106h,0B0D9h,0B0ACh,0B07Fh,0B053h,0B026h,0AFF9h,0AFCDh,0AFA0h
DW 0AF74h,0AF47h,0AF1Bh,0AEEFh,0AEC2h,0AE96h,0AE6Ah,0AE3Eh,0AE12h,0ADE6h
DW 0ADBAh,0AD8Fh,0AD63h,0AD37h,0AD0Ch,0ACE0h,0ACB5h,0AC89h,0AC5Eh,0AC33h
DW 0AC07h,0ABDCh,0ABB1h,0AB86h,0AB5Bh,0AB30h,0AB06h,0AADBh,0AAB0h,0AA86h
DW 0AA5Bh,0AA31h,0AA06h,0A9DCh,0A9B2h,0A987h,0A95Dh,0A933h,0A909h,0A8DFh
DW 0A8B5h,0A88Ch,0A862h,0A838h,0A80Fh,0A7E5h,0A7BCh,0A792h,0A769h,0A740h
DW 0A717h,0A6EEh,0A6C4h,0A69Ch,0A673h,0A64Ah,0A621h,0A5F8h,0A5D0h,0A5A7h
DW 0A57Fh,0A556h,0A52Eh,0A506h,0A4DEh,0A4B5h,0A48Dh,0A465h,0A43Dh,0A416h
DW 0A3EEh,0A3C6h,0A39Fh,0A377h,0A350h,0A328h,0A301h,0A2DAh,0A2B2h,0A28Bh
DW 0A264h,0A23Dh,0A216h,0A1F0h,0A1C9h,0A1A2h,0A17Ch,0A155h,0A12Fh,0A108h
DW 0A0E2h,0A0BCh,0A095h,0A06Fh,0A049h,0A023h,09FFEh,09FD8h,09FB2h,09F8Ch
DW 09F67h,09F41h,09F1Ch,09EF7h,09ED1h,09EACh,09E87h,09E62h,09E3Dh,09E18h
DW 09DF4h,09DCFh,09DAAh,09D86h,09D61h,09D3Dh,09D18h,09CF4h,09CD0h,09CACh
DW 09C88h,09C64h,09C40h,09C1Ch,09BF8h,09BD5h,09BB1h,09B8Eh,09B6Ah,09B47h
DW 09B24h,09B01h,09ADDh,09ABAh,09A98h,09A75h,09A52h,09A2Fh,09A0Dh,099EAh
DW 099C8h,099A5h,09983h,09961h,0993Fh,0991Dh,098FBh,098D9h,098B7h,09895h
DW 09873h,09852h,09830h,0980Fh,097EEh,097CCh,097ABh,0978Ah,09769h,09748h
DW 09727h,09707h,096E6h,096C5h,096A5h,09684h,09664h,09644h,09624h,09603h
DW 095E3h,095C3h,095A4h,09584h,09564h,09545h,09525h,09506h,094E6h,094C7h
DW 094A8h,09489h,0946Ah,0944Bh,0942Ch,0940Dh,093EEh,093D0h,093B1h,09393h
DW 09375h,09356h,09338h,0931Ah,092FCh,092DEh,092C0h,092A2h,09285h,09267h
DW 0924Ah,0922Ch,0920Fh,091F2h,091D5h,091B7h,0919Bh,0917Eh,09161h,09144h
DW 09127h,0910Bh,090EEh,090D2h,090B6h,0909Ah,0907Dh,09061h,09045h,0902Ah
DW 0900Eh,08FF2h,08FD7h,08FBBh,08FA0h,08F84h,08F69h,08F4Eh,08F33h,08F18h
DW 08EFDh,08EE2h,08EC7h,08EADh,08E92h,08E78h,08E5Dh,08E43h,08E29h,08E0Fh
DW 08DF5h,08DDBh,08DC1h,08DA7h,08D8Eh,08D74h,08D5Bh,08D41h,08D28h,08D0Fh
DW 08CF6h,08CDDh,08CC4h,08CABh,08C92h,08C79h,08C61h,08C48h,08C30h,08C18h
DW 08C00h,08BE7h,08BCFh,08BB7h,08BA0h,08B88h,08B70h,08B59h,08B41h,08B2Ah
DW 08B12h,08AFBh,08AE4h,08ACDh,08AB6h,08A9Fh,08A88h,08A72h,08A5Bh,08A45h
DW 08A2Eh,08A18h,08A02h,089ECh,089D6h,089C0h,089AAh,08994h,0897Eh,08969h
DW 08953h,0893Eh,08929h,08914h,088FFh,088EAh,088D5h,088C0h,088ABh,08896h
DW 08882h,0886Dh,08859h,08845h,08831h,0881Dh,08809h,087F5h,087E1h,087CDh
DW 087BAh,087A6h,08793h,0877Fh,0876Ch,08759h,08746h,08733h,08720h,0870Dh
DW 086FBh,086E8h,086D6h,086C3h,086B1h,0869Fh,0868Dh,0867Bh,08669h,08657h
DW 08645h,08634h,08622h,08611h,085FFh,085EEh,085DDh,085CCh,085BBh,085AAh
DW 08599h,08589h,08578h,08568h,08557h,08547h,08537h,08527h,08517h,08507h
DW 084F7h,084E7h,084D8h,084C8h,084B9h,084A9h,0849Ah,0848Bh,0847Ch,0846Dh
DW 0845Eh,0844Fh,08441h,08432h,08424h,08415h,08407h,083F9h,083EBh,083DDh
DW 083CFh,083C1h,083B3h,083A6h,08398h,0838Bh,0837Eh,08370h,08363h,08356h
DW 08349h,0833Ch,08330h,08323h,08317h,0830Ah,082FEh,082F2h,082E6h,082D9h
DW 082CEh,082C2h,082B6h,082AAh,0829Fh,08293h,08288h,0827Dh,08272h,08266h
DW 0825Ch,08251h,08246h,0823Bh,08231h,08226h,0821Ch,08212h,08207h,081FDh
DW 081F3h,081E9h,081E0h,081D6h,081CCh,081C3h,081B9h,081B0h,081A7h,0819Eh
DW 08195h,0818Ch,08183h,0817Bh,08172h,08169h,08161h,08159h,08151h,08148h
DW 08140h,08138h,08131h,08129h,08121h,0811Ah,08112h,0810Bh,08104h,080FDh
DW 080F6h,080EFh,080E8h,080E1h,080DBh,080D4h,080CEh,080C7h,080C1h,080BBh
DW 080B5h,080AFh,080A9h,080A4h,0809Eh,08098h,08093h,0808Eh,08088h,08083h
DW 0807Eh,08079h,08074h,08070h,0806Bh,08067h,08062h,0805Eh,08059h,08055h
DW 08051h,0804Dh,0804Ah,08046h,08042h,0803Fh,0803Bh,08038h,08035h,08031h
DW 0802Eh,0802Bh,08029h,08026h,08023h,08021h,0801Eh,0801Ch,0801Ah,08018h
DW 08015h,08014h,08012h,08010h,0800Eh,0800Dh,0800Bh,0800Ah,08009h,08008h
DW 08006h,08006h,08005h,08004h,08003h,08003h,08002h,08002h,08002h,08000h
DW 08000h,08000h,08002h,08002h,08002h,08003h,08003h,08004h,08005h,08006h
DW 08006h,08008h,08009h,0800Ah,0800Bh,0800Dh,0800Eh,08010h,08012h,08014h
DW 08015h,08018h,0801Ah,0801Ch,0801Eh,08021h,08023h,08026h,08029h,0802Bh
DW 0802Eh,08031h,08035h,08038h,0803Bh,0803Fh,08042h,08046h,0804Ah,0804Dh
DW 08051h,08055h,08059h,0805Eh,08062h,08067h,0806Bh,08070h,08074h,08079h
DW 0807Eh,08083h,08088h,0808Eh,08093h,08098h,0809Eh,080A4h,080A9h,080AFh
DW 080B5h,080BBh,080C1h,080C7h,080CEh,080D4h,080DBh,080E1h,080E8h,080EFh
DW 080F6h,080FDh,08104h,0810Bh,08112h,0811Ah,08121h,08129h,08131h,08138h
DW 08140h,08148h,08151h,08159h,08161h,08169h,08172h,0817Bh,08183h,0818Ch
DW 08195h,0819Eh,081A7h,081B0h,081B9h,081C3h,081CCh,081D6h,081E0h,081E9h
DW 081F3h,081FDh,08207h,08212h,0821Ch,08226h,08231h,0823Bh,08246h,08251h
DW 0825Ch,08266h,08272h,0827Dh,08288h,08293h,0829Fh,082AAh,082B6h,082C2h
DW 082CEh,082D9h,082E6h,082F2h,082FEh,0830Ah,08317h,08323h,08330h,0833Ch
DW 08349h,08356h,08363h,08370h,0837Eh,0838Bh,08398h,083A6h,083B3h,083C1h
DW 083CFh,083DDh,083EBh,083F9h,08407h,08415h,08424h,08432h,08441h,0844Fh
DW 0845Eh,0846Dh,0847Ch,0848Bh,0849Ah,084A9h,084B9h,084C8h,084D8h,084E7h
DW 084F7h,08507h,08517h,08527h,08537h,08547h,08557h,08568h,08578h,08589h
DW 08599h,085AAh,085BBh,085CCh,085DDh,085EEh,085FFh,08611h,08622h,08634h
DW 08645h,08657h,08669h,0867Bh,0868Dh,0869Fh,086B1h,086C3h,086D6h,086E8h
DW 086FBh,0870Dh,08720h,08733h,08746h,08759h,0876Ch,0877Fh,08793h,087A6h
DW 087BAh,087CDh,087E1h,087F5h,08809h,0881Dh,08831h,08845h,08859h,0886Dh
DW 08882h,08896h,088ABh,088C0h,088D5h,088EAh,088FFh,08914h,08929h,0893Eh
DW 08953h,08969h,0897Eh,08994h,089AAh,089C0h,089D6h,089ECh,08A02h,08A18h
DW 08A2Eh,08A45h,08A5Bh,08A72h,08A88h,08A9Fh,08AB6h,08ACDh,08AE4h,08AFBh
DW 08B12h,08B2Ah,08B41h,08B59h,08B70h,08B88h,08BA0h,08BB7h,08BCFh,08BE7h
DW 08C00h,08C18h,08C30h,08C48h,08C61h,08C79h,08C92h,08CABh,08CC4h,08CDDh
DW 08CF6h,08D0Fh,08D28h,08D41h,08D5Bh,08D74h,08D8Eh,08DA7h,08DC1h,08DDBh
DW 08DF5h,08E0Fh,08E29h,08E43h,08E5Dh,08E78h,08E92h,08EADh,08EC7h,08EE2h
DW 08EFDh,08F18h,08F33h,08F4Eh,08F69h,08F84h,08FA0h,08FBBh,08FD7h,08FF2h
DW 0900Eh,0902Ah,09045h,09061h,0907Dh,0909Ah,090B6h,090D2h,090EEh,0910Bh
DW 09127h,09144h,09161h,0917Eh,0919Bh,091B7h,091D5h,091F2h,0920Fh,0922Ch
DW 0924Ah,09267h,09285h,092A2h,092C0h,092DEh,092FCh,0931Ah,09338h,09356h
DW 09375h,09393h,093B1h,093D0h,093EEh,0940Dh,0942Ch,0944Bh,0946Ah,09489h
DW 094A8h,094C7h,094E6h,09506h,09525h,09545h,09564h,09584h,095A4h,095C3h
DW 095E3h,09603h,09624h,09644h,09664h,09684h,096A5h,096C5h,096E6h,09707h
DW 09727h,09748h,09769h,0978Ah,097ABh,097CCh,097EEh,0980Fh,09830h,09852h
DW 09873h,09895h,098B7h,098D9h,098FBh,0991Dh,0993Fh,09961h,09983h,099A5h
DW 099C8h,099EAh,09A0Dh,09A2Fh,09A52h,09A75h,09A98h,09ABAh,09ADDh,09B01h
DW 09B24h,09B47h,09B6Ah,09B8Eh,09BB1h,09BD5h,09BF8h,09C1Ch,09C40h,09C64h
DW 09C88h,09CACh,09CD0h,09CF4h,09D18h,09D3Dh,09D61h,09D86h,09DAAh,09DCFh
DW 09DF4h,09E18h,09E3Dh,09E62h,09E87h,09EACh,09ED1h,09EF7h,09F1Ch,09F41h
DW 09F67h,09F8Ch,09FB2h,09FD8h,09FFEh,0A023h,0A049h,0A06Fh,0A095h,0A0BCh
DW 0A0E2h,0A108h,0A12Fh,0A155h,0A17Ch,0A1A2h,0A1C9h,0A1F0h,0A216h,0A23Dh
DW 0A264h,0A28Bh,0A2B2h,0A2DAh,0A301h,0A328h,0A350h,0A377h,0A39Fh,0A3C6h
DW 0A3EEh,0A416h,0A43Dh,0A465h,0A48Dh,0A4B5h,0A4DEh,0A506h,0A52Eh,0A556h
DW 0A57Fh,0A5A7h,0A5D0h,0A5F8h,0A621h,0A64Ah,0A673h,0A69Ch,0A6C4h,0A6EEh
DW 0A717h,0A740h,0A769h,0A792h,0A7BCh,0A7E5h,0A80Fh,0A838h,0A862h,0A88Ch
DW 0A8B5h,0A8DFh,0A909h,0A933h,0A95Dh,0A987h,0A9B2h,0A9DCh,0AA06h,0AA31h
DW 0AA5Bh,0AA86h,0AAB0h,0AADBh,0AB06h,0AB30h,0AB5Bh,0AB86h,0ABB1h,0ABDCh
DW 0AC07h,0AC33h,0AC5Eh,0AC89h,0ACB5h,0ACE0h,0AD0Ch,0AD37h,0AD63h,0AD8Fh
DW 0ADBAh,0ADE6h,0AE12h,0AE3Eh,0AE6Ah,0AE96h,0AEC2h,0AEEFh,0AF1Bh,0AF47h
DW 0AF74h,0AFA0h,0AFCDh,0AFF9h,0B026h,0B053h,0B07Fh,0B0ACh,0B0D9h,0B106h
DW 0B133h,0B160h,0B18Dh,0B1BBh,0B1E8h,0B215h,0B243h,0B270h,0B29Eh,0B2CBh
DW 0B2F9h,0B327h,0B354h,0B382h,0B3B0h,0B3DEh,0B40Ch,0B43Ah,0B468h,0B496h
DW 0B4C5h,0B4F3h,0B521h,0B550h,0B57Eh,0B5ADh,0B5DBh,0B60Ah,0B639h,0B667h
DW 0B696h,0B6C5h,0B6F4h,0B723h,0B752h,0B781h,0B7B0h,0B7DFh,0B80Fh,0B83Eh
DW 0B86Dh,0B89Dh,0B8CCh,0B8FCh,0B92Ch,0B95Bh,0B98Bh,0B9BBh,0B9EAh,0BA1Ah
DW 0BA4Ah,0BA7Ah,0BAAAh,0BADAh,0BB0Bh,0BB3Bh,0BB6Bh,0BB9Bh,0BBCCh,0BBFCh
DW 0BC2Dh,0BC5Dh,0BC8Eh,0BCBEh,0BCEFh,0BD20h,0BD51h,0BD81h,0BDB2h,0BDE3h
DW 0BE14h,0BE45h,0BE76h,0BEA8h,0BED9h,0BF0Ah,0BF3Bh,0BF6Dh,0BF9Eh,0BFCFh
DW 0C001h,0C033h,0C064h,0C096h,0C0C8h,0C0F9h,0C12Bh,0C15Dh,0C18Fh,0C1C1h
DW 0C1F3h,0C225h,0C257h,0C289h,0C2BBh,0C2EDh,0C320h,0C352h,0C384h,0C3B7h
DW 0C3E9h,0C41Ch,0C44Eh,0C481h,0C4B4h,0C4E6h,0C519h,0C54Ch,0C57Fh,0C5B2h
DW 0C5E5h,0C618h,0C64Bh,0C67Eh,0C6B1h,0C6E4h,0C717h,0C74Ah,0C77Eh,0C7B1h
DW 0C7E4h,0C818h,0C84Bh,0C87Fh,0C8B2h,0C8E6h,0C91Ah,0C94Dh,0C981h,0C9B5h
DW 0C9E9h,0CA1Ch,0CA50h,0CA84h,0CAB8h,0CAECh,0CB20h,0CB54h,0CB89h,0CBBDh
DW 0CBF1h,0CC25h,0CC5Ah,0CC8Eh,0CCC2h,0CCF7h,0CD2Bh,0CD60h,0CD94h,0CDC9h
DW 0CDFDh,0CE32h,0CE67h,0CE9Ch,0CED0h,0CF05h,0CF3Ah,0CF6Fh,0CFA4h,0CFD9h
DW 0D00Eh,0D043h,0D078h,0D0ADh,0D0E2h,0D117h,0D14Dh,0D182h,0D1B7h,0D1ECh
DW 0D222h,0D257h,0D28Dh,0D2C2h,0D2F8h,0D32Dh,0D363h,0D398h,0D3CEh,0D404h
DW 0D43Ah,0D46Fh,0D4A5h,0D4DBh,0D511h,0D547h,0D57Dh,0D5B3h,0D5E9h,0D61Fh
DW 0D655h,0D68Bh,0D6C1h,0D6F7h,0D72Dh,0D763h,0D79Ah,0D7D0h,0D806h,0D83Dh
DW 0D873h,0D8A9h,0D8E0h,0D916h,0D94Dh,0D983h,0D9BAh,0D9F0h,0DA27h,0DA5Eh
DW 0DA94h,0DACBh,0DB02h,0DB39h,0DB6Fh,0DBA6h,0DBDDh,0DC14h,0DC4Bh,0DC82h
DW 0DCB9h,0DCF0h,0DD27h,0DD5Eh,0DD95h,0DDCCh,0DE03h,0DE3Ah,0DE71h,0DEA9h
DW 0DEE0h,0DF17h,0DF4Eh,0DF86h,0DFBDh,0DFF4h,0E02Ch,0E063h,0E09Bh,0E0D2h
DW 0E109h,0E141h,0E178h,0E1B0h,0E1E8h,0E21Fh,0E257h,0E28Eh,0E2C6h,0E2FEh
DW 0E336h,0E36Dh,0E3A5h,0E3DDh,0E415h,0E44Ch,0E484h,0E4BCh,0E4F4h,0E52Ch
DW 0E564h,0E59Ch,0E5D4h,0E60Ch,0E644h,0E67Ch,0E6B4h,0E6ECh,0E724h,0E75Ch
DW 0E794h,0E7CCh,0E805h,0E83Dh,0E875h,0E8ADh,0E8E5h,0E91Eh,0E956h,0E98Eh
DW 0E9C7h,0E9FFh,0EA37h,0EA70h,0EAA8h,0EAE0h,0EB19h,0EB51h,0EB8Ah,0EBC2h
DW 0EBFBh,0EC33h,0EC6Ch,0ECA4h,0ECDDh,0ED15h,0ED4Eh,0ED86h,0EDBFh,0EDF8h
DW 0EE30h,0EE69h,0EEA2h,0EEDAh,0EF13h,0EF4Ch,0EF84h,0EFBDh,0EFF6h,0F02Eh
DW 0F067h,0F0A0h,0F0D9h,0F112h,0F14Ah,0F183h,0F1BCh,0F1F5h,0F22Eh,0F267h
DW 0F29Fh,0F2D8h,0F311h,0F34Ah,0F383h,0F3BCh,0F3F5h,0F42Eh,0F467h,0F4A0h
DW 0F4D9h,0F512h,0F54Bh,0F584h,0F5BDh,0F5F6h,0F62Fh,0F668h,0F6A1h,0F6DAh
DW 0F713h,0F74Ch,0F785h,0F7BEh,0F7F7h,0F830h,0F869h,0F8A2h,0F8DBh,0F914h
DW 0F94Eh,0F987h,0F9C0h,0F9F9h,0FA32h,0FA6Bh,0FAA4h,0FADEh,0FB17h,0FB50h
DW 0FB89h,0FBC2h,0FBFBh,0FC34h,0FC6Eh,0FCA7h,0FCE0h,0FD19h,0FD52h,0FD8Bh
DW 0FDC5h,0FDFEh,0FE37h,0FE70h,0FEA9h,0FEE3h,0FF1Ch,0FF55h,0FF8Eh,0FFC7h

;----------------------------------------------------------------------------
; int intRoot (long radikand)
;----------------------------------------------------------------------------
;
intRoot 	PROC
;
ARG		radikand	: DWORD
;
		push	bp
		mov	bp,sp

;		approximator = 0x4000;

		mov	bx,2000h

;		root = 0x8000;

		mov	cx,4000h

;		while (approximator)
intRootLoop:
		cmp	bx,0
		jz	intRootRet

;		{
;		  potenz = root * root;

		mov	ax,cx
		mul	ax

;		  if (potenz = radikand)

		cmp	dx,[word ptr radikand+2]
		jne	intRootIf1
		cmp	ax,[word ptr radikand]
		jne	intRootIf1

;		    break;

		jmp	intRootRet
intRootIf1:
;		  if (potenz < radikand)

		sub	ax,[word ptr radikand]
		sbb	dx,[word ptr radikand+2]
		jge	intRootElse2

;		    root = root + approximator;

		add	cx,bx
		jmp	intRootIf2

intRootElse2:
;		  else
;		    root = root - approximator;

		sub	cx,bx

intRootIf2:
;		  approximator >>= 1;

		shr	bx,1

;		}
		jmp	intRootLoop
intRootRet:
;		return root;

		mov	ax,cx
		pop	bp
		ret

intRoot 	ENDP


;----------------------------------------------------------------------------
; long longScalarProd (struct POINT far *p1, struct POINT far *p2)
;----------------------------------------------------------------------------
;
longScalarProd	PROC
;
ARG		p1		: DWORD, \
		p2		: DWORD
;
		push	bp
		mov	bp,sp
		push	si
		push	di
		push	ds

;		long a = (p2.x - p1.x) * (p2.x - p1.x);
;		long b = (p2.y - p1.y) * (p2.y - p1.y);
;		long c = (p2.z - p1.z) * (p2.z - p1.z);

		les	di,[p1]
		lds	si,[p2]
		mov	ax,ds:[si.x]
		sub	ax,es:[di.x]
		imul	ax
		mov	bx,ax
		mov	cx,dx
		mov	ax,ds:[si.y]
		sub	ax,es:[di.y]
		imul	ax
		add	bx,ax
		adc	cx,dx
		mov	ax,ds:[si.z]
		sub	ax,es:[di.z]
		imul	ax
		add	bx,ax
		adc	cx,dx

;		return a + b + c;

		mov	ax,bx
		mov	dx,cx

		pop	ds
		pop	di
		pop	si
		pop	bp
		ret

longScalarProd	ENDP



		END
}
