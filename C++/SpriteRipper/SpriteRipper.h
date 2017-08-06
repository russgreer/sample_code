#pragma once



#include "Commdlg.h"
#include "resource.h"

#include "App.h"
#include "Ripper.h"

// Globals (for now)
wchar_t msg[255];
App    *gApp = NULL;
Sprite *sprite_list = NULL; // list of sprites ripped


// prototypes...
wchar_t *GetFileName(void);
char *getSaveSpriteName(void);
void DrawBitmap(HDC hDC, HBITMAP hBM, int x, int y);
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
                           short yStart, COLORREF cTransparentColor);