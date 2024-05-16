#pragma once

#include <list>
#include <memory>
#include "Sprite.h"

class App
{
private:
	int  windowWidth;
	int  windowHeight;
	BOOL playingSprites;
	int  CurrentSprite;
	int  numSprites;
	std::list<std::shared_ptr<Sprite>> SpriteList;

public:
	App(void);
	int  GetWindowWidth(void);
	void SetWindowWidth(int width);
	int  GetWindowHeight(void);
	int  GetNumSprites(void);
	void AddSprite(std::shared_ptr<Sprite> newSprite);
	void SetNumSprites(int num);
	void SetWindowHeight(int height);
 	Sprite* GetCurrentSprite(void);
	Sprite* GetSprite(int frameNumber);
	void NextSprite(void);
	void LastSprite(void);
	BOOL SaveSprite(char* fileName, Sprite *sprite);	
	~App(void);
};



