#pragma once

#include "Image.h"

class Sprite
{
private:
	int  frameNumber;
	int  delay;
	long transparentColor;
	int  hotSpotX;
	int  hotSpotY;
	HBITMAP  hBM;
	Image *image;    // image class
public:

	Sprite( unsigned char *imageData, int width, int height, int hotSpotX, int hotSpotY );
	int GetFrameNumber(void);
	void SetFrameNumber(int frameNum);
	int  GetDelay(void);
	void SetDelay(int frameDelay);
	long GetTransparentColor(void);
	void SetTransparentColor(long transColor);
	HBITMAP GetBitmap(void);
	Image *GetImage(void);
//	HBITMAP BitmapToHBM(BITMAP bm);
	~Sprite(void);


};


extern Sprite *sprite_list;