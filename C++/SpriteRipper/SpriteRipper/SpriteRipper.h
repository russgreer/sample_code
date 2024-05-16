#pragma once



#include "Commdlg.h"
#include "resource.h"

// Globals (for now)
wchar_t msg[255];

#include "App.h"
#include "Ripper.h"

App* gApp = NULL;

// prototypes...
wchar_t *GetFileName(void);
char *getSaveSpriteName(void);
void DrawBitmap(HDC hDC, HBITMAP hBM, int x, int y);
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
                           short yStart, COLORREF cTransparentColor);