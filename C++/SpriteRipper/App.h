#pragma once

#include "Sprite.h"

extern Sprite *sprite_list;

class App
{
private:
	int windowWidth;
	int windowHeight;
	BOOL playingSprites;
    Sprite *CurrentSprite;
public:
	App(void);
	int  GetWindowWidth(void);
	void SetWindowWidth(int width);
	int  GetWindowHeight(void);
	void SetWindowHeight(int height);
	Sprite *GetCurrentSprite(void);
	void NextSprite(void);
	void LastSprite(void);
	BOOL SaveSprite(char* fileName, Sprite *sprite);	
	~App(void);
};

