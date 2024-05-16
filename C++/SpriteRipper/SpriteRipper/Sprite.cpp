#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite( unsigned char *imageData, int width, int height, int hotSpotX, int hotSpotY )
{
  HDC hdcScreen = CreateDC(L"DISPLAY", (LPCWSTR) NULL, (LPCWSTR) NULL, (CONST DEVMODE *) NULL); 
  HDC hDCMem = CreateCompatibleDC( hdcScreen ); 

  hBM = CreateCompatibleBitmap( hdcScreen, width, height );

  SetBitmapBits( hBM, (width * height ) * 4, imageData );

  image = new Image(imageData, width, height, 4 );

}



int Sprite::GetFrameNumber(void)
{
	return frameNumber;	
}


void Sprite::SetFrameNumber(int frameNum)
{
  frameNumber = frameNum;
}


int Sprite::GetDelay(void)
{
	return delay;
}


void Sprite::SetDelay(int frameDelay)
{
  delay = frameDelay;
}


long Sprite::GetTransparentColor(void)
{
	return transparentColor;
}


void Sprite::SetTransparentColor(long transColor)
{
	transparentColor = transColor;
}


HBITMAP Sprite::GetBitmap(void)
{
  return hBM;
}


Image *Sprite::GetImage(void)
{
  return image;
}


Sprite::~Sprite(void)
{
	if( hBM )
		DeleteObject(hBM);
	
	if( image )
		delete image;

}
