#include "stdafx.h"
#include "App.h"


App::App(void)
{
   CurrentSprite = NULL;
}


int App::GetWindowWidth(void)
{
    return windowWidth;	
}


void App::SetWindowWidth(int width)
{
    windowWidth = width;
}


int App::GetWindowHeight(void)
{
    return windowHeight;	
}

void App::SetWindowHeight(int height)
{
	windowHeight = height;
}



Sprite *App::GetCurrentSprite(void)
{
	return CurrentSprite;	
}

void App::NextSprite(void)
{
	if( ! CurrentSprite )
	  {
		CurrentSprite = sprite_list;
		return;
	  }

	if( CurrentSprite ->NextSprite )
		CurrentSprite = CurrentSprite ->NextSprite;
}


void App::LastSprite(void)
{
	if( ! CurrentSprite )
	  {
		CurrentSprite = sprite_list;
		return;
	  }

	if( CurrentSprite ->LastSprite )
		CurrentSprite = CurrentSprite ->LastSprite;
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
