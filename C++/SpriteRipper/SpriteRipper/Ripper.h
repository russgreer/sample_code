#pragma once

#include < olectl.h >
#include "Sprite.h"
#include "App.h"

extern App* gApp;

#define HIMETRIC_INCH 2540
#define MAP_LOGHIM_TO_PIX(x,ppli) ( ((ppli)*(x) + HIMETRIC_INCH/2) / HIMETRIC_INCH ) 


class Ripper
{

public:
	Ripper(void);
	void LoadBMP(wchar_t* fileName);
	BOOL RipImageToSprites( HBITMAP hBM, BITMAP bm );
	BOOL IsBoxColor( unsigned char *Pixels, BOOL *hotSpot );
	int CountBoxWidth( unsigned char *Buffer, unsigned long x, unsigned long y, int *hotSpotX );
	int CountBoxHeight( unsigned char *Buffer, unsigned long x, unsigned long y, unsigned long width, int *hotSpotY );
    BOOL ClipOutSpriteFrame( unsigned char *FromPtr, unsigned long BoxWidth, unsigned long BoxHeight, unsigned long BufferWidth, int hotSpotX, int hotSpotY );
};

