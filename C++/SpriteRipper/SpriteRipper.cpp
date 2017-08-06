// SpriteRipper.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpriteRipper.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SPRITERIPPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRITERIPPER));

	// main Application class to control everything...
	gApp = new App();

	gApp ->SetWindowWidth( 400 );
	gApp ->SetWindowHeight( 400 );

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

 

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITERIPPER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)CreatePatternBrush((HBITMAP) LoadImage(GetModuleHandle(0),
                                      MAKEINTRESOURCE(IDB_BITMAP1),
                                     IMAGE_BITMAP,0,0,
                                     LR_CREATEDIBSECTION));//(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SPRITERIPPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 400, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_FILE_LOADBMPTORIP:
			 {
			  RECT r;
              wchar_t *fileName = GetFileName();

			  // Rip BMP File to memory...
			  Ripper *Rip = new Ripper();
			  Rip ->LoadBMP(fileName);
			  delete Rip;

			  gApp ->NextSprite();  // show initial sprite...
			  		
			  GetClientRect( hWnd, &r);

			  gApp ->SetWindowHeight( r.bottom );
			  gApp ->SetWindowWidth( r.right );
			  
			  // update the window
			  InvalidateRect(hWnd, NULL, TRUE);
			  UpdateWindow(hWnd);
			 }
			 break;
	 
		case ID_NAVIGATION_NEXT:
			 gApp ->NextSprite();

			 // update the window
			 InvalidateRect(hWnd, NULL, TRUE);
			 UpdateWindow(hWnd);
			 break;
	 		 
		case ID_NAVIGATION_LAST:
			 gApp ->LastSprite();

			 // update the window
			 InvalidateRect(hWnd, NULL, TRUE);
			 UpdateWindow(hWnd);
			 break;

		case ID_FILE_SAVESPRITES: 
			 {
               Sprite *sprite = sprite_list;

			   // Getbase name...
			   LPWSTR baseName;
			   WCHAR fileName[255];
			   baseName = (LPWSTR) getSaveSpriteName();
			   baseName[lstrlen(baseName)-4] = L'\0';  // strip off .spr from saave dialog.
 
			   int i = 1;
			   while( sprite )
			   {			
				wsprintf(fileName, L"%s%04ld.spr", baseName, i++);

 			    gApp ->SaveSprite((char *)fileName, sprite );

			    sprite = sprite ->NextSprite;
			   }

	           wsprintf(msg, L"Sprite(s) written to file(s).");
               MessageBox(NULL,msg,TEXT("Info..."), MB_OK);

			 }
			 break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
    case WM_KEYDOWN:
        switch(wParam)
        {
		case VK_RIGHT:
			 gApp ->NextSprite();

			 // update the window
			 InvalidateRect(hWnd, NULL, TRUE);
			 UpdateWindow(hWnd);
			 break;
	  
		case VK_LEFT:
			 gApp ->LastSprite();

			 // update the window
			 InvalidateRect(hWnd, NULL, TRUE);
			 UpdateWindow(hWnd);
			 break;
	    }
	break;

	case WM_LBUTTONUP:  // check buttons...
		{
	   	 POINT p;
         if(GetCursorPos(&p))
           {
            //cursor position now in p.x and p.y
            if(ScreenToClient(hWnd, &p))
			  {
				  // DEBUG (for now)... use for when a button is clicked on later 
		          wsprintf(msg, L"mx=%ld  my=%ld", p.x, p.y);
		          MessageBox(NULL, msg,TEXT("Info..."), MB_OK);
			  }
		  }
		}
		break;

	case WM_SIZE:
		         // get new size...
		         if( gApp)
				 {
    		      gApp ->SetWindowWidth( LOWORD(lParam) );
		          gApp ->SetWindowHeight( HIWORD(lParam) );
				 }
		         break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetBkMode(hdc, TRANSPARENT);

		if( gApp )
		{
			if(gApp ->GetCurrentSprite())
			{
		      int x = (gApp ->GetWindowWidth()/2)-(gApp ->GetCurrentSprite() ->GetImage() ->getWidth()/2);
			  int y = (gApp->GetWindowHeight()/2)-(gApp ->GetCurrentSprite() ->GetImage() ->getHeight()/2);
         		 	 
			  DrawTransparentBitmap(hdc, gApp ->GetCurrentSprite() ->GetBitmap(), x, y, gApp ->GetCurrentSprite() ->GetTransparentColor() );

			  wsprintf(msg, L"Frame number: %ld", gApp ->GetCurrentSprite() ->GetFrameNumber() ); 
			  TextOut(hdc, x, y+gApp ->GetCurrentSprite() ->GetImage() ->getHeight()+10, msg, lstrlen(msg));	
			}
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		
		  if(gApp) 
			 delete gApp;

		  if(sprite_list)
		  {
            Sprite *workSprite = sprite_list;
		    while(workSprite)
			{
			    Sprite *TempSprite = workSprite;
				workSprite = workSprite ->NextSprite;

				if( TempSprite )
					delete TempSprite;
			}
		  
		  }

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



wchar_t *GetFileName(void)
{
 static OPENFILENAME ofn;
 static wchar_t Filter[255];
 static wchar_t TmpStr[1024];
 wchar_t TestStr[ 255 ];

 memset(Filter,0,255);
  
 lstrcpy(Filter, L"Image files (*.bmp)");
 lstrcpy(&Filter[lstrlen(Filter)+1],L"*.BMP;");

 memset(TmpStr,0,1024);

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = NULL; // hWnd;
 ofn.hInstance = hInst;
 ofn.lpstrFilter = Filter;
 ofn.lpstrCustomFilter = NULL;
 ofn.nMaxCustFilter = 0;
 ofn.nFilterIndex = 0;
 ofn.lpstrFile = TmpStr;   // array to fill with info...
 ofn.nMaxFile = _MAX_PATH;
 ofn.lpstrFileTitle = NULL;
 ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
 ofn.lpstrInitialDir = NULL;
 ofn.lpstrTitle = NULL;
 ofn.Flags = OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS;
 ofn.nFileOffset = 0;
 ofn.nFileExtension = 0;
 ofn.lpstrDefExt = L"*";
 ofn.lCustData = 0L;
 ofn.lpfnHook = NULL;
 ofn.lpTemplateName = NULL;
 
 GetOpenFileName(&ofn);

 lstrcpy( TestStr, &TmpStr[ lstrlen( TmpStr ) - 3] );
 
 if( (! lstrcmpi( TestStr, L"bmp") ))
    return ((wchar_t *) TmpStr);
     

 MessageBox(NULL,TEXT("Wrong file type: You must load a .BMP file."), TEXT("ERROR:123"), MB_OK);

 // didn't load anything ( cancel ) or 
 // not correct image type...
  lstrcpy( TmpStr, L" " );
  return ((wchar_t *) TmpStr);; 
}



char *getSaveSpriteName(void)
{

 static OPENFILENAME ofn;
 static wchar_t Filter[255];
 static wchar_t TmpStr[1024];

 memset(Filter,0,255);

 lstrcpy(Filter, L"Sprite File (*.spr)");
 lstrcpy(&Filter[lstrlen(Filter)+1],L"*.spr");
  
 memset(TmpStr,0,1024);

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = NULL; // hWnd;
 ofn.hInstance = hInst;
 ofn.lpstrFilter = Filter;
 ofn.lpstrCustomFilter = NULL;
 ofn.nMaxCustFilter = 0;
 ofn.nFilterIndex = 0;
 ofn.lpstrFile = TmpStr;   // array to fill with info...
 ofn.nMaxFile = _MAX_PATH;
 ofn.lpstrFileTitle = NULL;
 ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
 ofn.lpstrInitialDir = NULL;
 ofn.lpstrTitle = NULL;
 ofn.Flags = OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS;
 ofn.nFileOffset = 0;
 ofn.nFileExtension = 0;
 ofn.lpstrDefExt = L"*";
 ofn.lCustData = 0L;
 ofn.lpfnHook = NULL;
 ofn.lpTemplateName = NULL;
 
 GetSaveFileName(&ofn);

 return ((char *) TmpStr);
}





void DrawBitmap(HDC hDC, HBITMAP hBM, int x, int y)
{
  BITMAP bm;
  HDC hdcMem;
  POINT ptSize, ptOrg;

  hdcMem = CreateCompatibleDC(hDC);
  SelectObject(hdcMem, hBM);
  SetMapMode(hdcMem, GetMapMode(hDC));

  GetObject( hBM, sizeof(BITMAP),(LPVOID) &bm );

  ptSize.x = bm.bmWidth;
  ptSize.y = bm.bmHeight;

  DPtoLP(hDC, &ptSize, 1);

  ptOrg.x = 0;
  ptOrg.y = 0;

  DPtoLP(hdcMem, &ptOrg, 1);
  
  BitBlt(hDC, x, y, ptSize.x, ptSize.y, 
         hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);

  DeleteDC(hdcMem);
}


void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
                           short yStart, COLORREF cTransparentColor)
{
   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
   bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
   bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack,   bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem,    bmMemOld));
   DeleteObject(SelectObject(hdcSave,   bmSaveOld));

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
}