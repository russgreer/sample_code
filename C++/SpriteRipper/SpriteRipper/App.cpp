#include "stdafx.h"
#include "App.h"


App::App(void)
{
   this ->CurrentSprite = 0;
}


int App::GetNumSprites(void) {
	return this ->numSprites;
}


void App::SetNumSprites(int num) {
	this ->numSprites = num;
}


int App::GetWindowWidth(void)
{
    return this ->windowWidth;	
}


void App::SetWindowWidth(int width)
{
    this ->windowWidth = width;
}


int App::GetWindowHeight(void)
{
    return this ->windowHeight;	
}


void App::SetWindowHeight(int height)
{
	this ->windowHeight = height;
}


void App::AddSprite(std::shared_ptr<Sprite> newSprite) {
	this ->SpriteList.push_back(newSprite);
}


Sprite* App::GetSprite(int frameNumber) {
		
	for (auto it = this->SpriteList.begin(); it != this->SpriteList.end(); ++it) {
	
		if (it->get()->GetFrameNumber() == frameNumber)
			return it->get();
	}

	return NULL;
}


Sprite* App::GetCurrentSprite(void)
{
	return std::next(this ->SpriteList.begin(), this ->CurrentSprite) ->get();	
}


void App::NextSprite(void)
{
	if(this ->CurrentSprite+1 >= this ->numSprites)
		return;
	  
	this ->CurrentSprite++;
}


void App::LastSprite(void)
{ 
	if(this ->CurrentSprite != 0)
	  this->CurrentSprite--;
}


// save to individual file.
BOOL App::SaveSprite(char* fileName, Sprite *sprite)
{
	FILE *fp = NULL;

	if( ! fileName || ! sprite )
		return FALSE;
		
	// Open file
	HANDLE fh = CreateFile( (LPCWSTR)fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL, NULL);

	// Get sprite info...
	BYTE bytesPerPixel = (BYTE) sprite ->GetImage() ->getBytesPerPixel();
    short width = (short) sprite ->GetImage() ->getWidth();
    short height = (short)sprite ->GetImage() ->getHeight();
	long  transparentColor = (long) sprite ->GetTransparentColor();
	DWORD BytesWritten = 0;

	// write sprite header
	WriteFile(fh, &bytesPerPixel,    sizeof(BYTE),  &BytesWritten, NULL);
	WriteFile(fh, &width,            sizeof(short), &BytesWritten, NULL);
	WriteFile(fh, &height,           sizeof(short), &BytesWritten, NULL);
	WriteFile(fh, &transparentColor, sizeof(long),  &BytesWritten, NULL);

	// write sprite data
	WriteFile(fh, sprite ->GetImage() ->getImageData(), 
		     ( width * height )*bytesPerPixel,
		      &BytesWritten, NULL);

	// Close file
	CloseHandle(fh);

	return TRUE;
}



App::~App(void)
{

}
