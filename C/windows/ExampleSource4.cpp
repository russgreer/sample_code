
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:38:01 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:38:01 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK LayoutPwDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {

                      }
                      return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{

	 	 case IDOK: 
                  {
                   char TmpStr[ 255 ];

                   memset( TmpStr, 0, 255 );

                   GetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDT), TmpStr,
                                 GetWindowTextLength(GetDlgItem(hDlg, IDC_PASSWORD_EDT) ) + 1);



                   if(! TmpStr[ 0 ])
                     {
                      OK_Msg("INFO:","Please enter a password."); 
                      break;
                     }

                   strcpy( gLayoutPW, TmpStr );

	               EndDialog(hDlg, TRUE);
                  }
                  break;

         case IDCANCEL:
	              EndDialog(hDlg, TRUE);
                  break;
        }

     break; 
   }

  return FALSE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:40:21 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:40:21 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void SaveLayout( char *Filename, char *Password )
{ 
  DWORD NumItems, Len = 0;
  CRYPT_ITEM *CryptItem = gCryptCDItemRoot;
  FILE *out = NULL;
  char TmpStr[ 255 ];

  memset( TmpStr, 0, 255 );

  if( ! CryptItem )     
    {
     OK_Msg("INFO:","Nothing in the CryptCD layout to save.");
     return;
    } 


  if(!( out = fopen( Filename, "wb" )))
    {
     OK_Msg("Error: (452345)","Cannot create layout file. Please try again.");
     return;
    }  


  NumItems = 0;
  CryptItem = gCryptCDItemRoot;
  while( CryptItem ) // count items in list...
  {
   NumItems++;
   CryptItem = CryptItem ->Next;
  }


  gLayoutHeader.Version  = LAYOUTFILE_VER;
  gLayoutHeader.NumItems = NumItems;

  // write out the header... ( never encrypted )
  fwrite( &gLayoutHeader, 1, sizeof( LAYOUT_HEADER ), out );


  if( gLayoutHeader.Flags & FLAG_LAYOUTPASSWORD )
    {
     memset( TmpStr, 0, 255 );
     strcpy( TmpStr, (char *) EVAL_STRING );    
     Len = strlen( TmpStr );
     fwrite( TmpStr, 1, Len, out ); // NOTE! DOES NOT NULL TERMINATE!
                                    // MUST READ INTO STRLEN+1 for NULL!
                                    // IN LOAD_LAYOUT()!
    }


  if( gLayoutHeader.Flags & FLAG_LAYOUTSAVEALL )
    {
     char TmpPassword[ 255 ];

     memcpy( TmpPassword, gCfg.Password, 255 );

     memset( gCfg.Password, ':', 255 ); // Put in junk ( NOT real password )...
  
     // Write out CFG struct ( NOT including var string EULA ptr! )
     fwrite( &gCfg, 1, sizeof(CFG) - sizeof( char * ), out );
 
     strcpy( gCfg.Password, TmpPassword );

     // Dump the EULA ( if one )
     if( gCfg.EulaText )
       {
         Len = strlen( gCfg.EulaText );
       }
      else
       {
         Len = 0;
       }

     fwrite( &Len, 1, sizeof(DWORD), out );

     if( gCfg.EulaText )
         fwrite( &gCfg.EulaText[0], 1, Len, out );
    }


  CryptItem = gCryptCDItemRoot; // reset to 1st node... 
  while( CryptItem )
  {

   fwrite( &CryptItem ->ID,         1, sizeof(DWORD), out );
   fwrite( &CryptItem ->ParentID,   1, sizeof(DWORD), out );
   fwrite( &CryptItem ->InfoType,   1, sizeof(DWORD), out );
   fwrite( &CryptItem ->Size,       1, sizeof(DWORD), out );
   fwrite( &CryptItem ->Attributes, 1, sizeof(DWORD), out );
   fwrite( &CryptItem ->Flags,      1, sizeof(DWORD), out );
   fwrite( &CryptItem ->CaseNumber, 1, sizeof(DWORD), out );

   Len = ( DWORD ) strlen( CryptItem ->ItemName );
   fwrite( &Len, 1, sizeof( DWORD ), out );
   fwrite( CryptItem ->ItemName, 1, Len, out ); // NOTE! DOES NOT NULL TERMINATE!
                                                // MUST READ INTO STRLEN+1 for NULL!
                                                // IN LOAD_LAYOUT()!

   if( CryptItem ->ItemPath )
     {
      Len = ( DWORD ) strlen( CryptItem ->ItemPath );
      fwrite( &Len, 1, sizeof( DWORD ), out );
      fwrite( CryptItem ->ItemPath, 1, Len, out ); // NOT NULL terminated...
     }
    else
     {
      Len = 0L;
      fwrite( &Len, 1, sizeof( DWORD ), out );
     }

   // save item note ( if one )
   if( CryptItem ->ItemNotes )
     {
      Len = ( DWORD ) strlen( CryptItem ->ItemNotes );
      fwrite( &Len, 1, sizeof( DWORD ), out );
      fwrite( CryptItem ->ItemNotes, 1, Len, out ); // NOT NULL terminated...
     }
    else
     {
      Len = 0L;
      fwrite( &Len, 1, sizeof( DWORD ), out );
     }

   // save item suspect name ( if one )
   if( CryptItem ->SuspectName )
     {
      Len = ( DWORD ) strlen( CryptItem ->SuspectName );
      fwrite( &Len, 1, sizeof( DWORD ), out );
      fwrite( CryptItem ->SuspectName, 1, Len, out ); // NOT NULL terminated...
     }
    else
     {
      Len = 0L;
      fwrite( &Len, 1, sizeof( DWORD ), out );
     }

   CryptItem = CryptItem ->Next;
  }


  if( out ) fclose( out );


  if( gLayoutHeader.Flags & FLAG_LAYOUTPASSWORD )
      EncryptLayoutFile( Filename, Password );   
   
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:41:22 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:41:22 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *GetDirName( char *Title )
{
  BROWSEINFO bi;
  TCHAR szDir[MAX_PATH];
  LPITEMIDLIST pidl;
  LPMALLOC pmalloc;
  static char WorkStr[1024];


  if(SUCCEEDED(SHGetMalloc(&pmalloc))) 
    {
      ZeroMemory(&bi,sizeof(bi));
      bi.hwndOwner = NULL;
      bi.pszDisplayName = 0;
      bi.pidlRoot = 0;
      bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT |
                   BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT ;
      bi.lpfn = BrowseCallbackProc;
      bi.lpszTitle = Title; 

      CoInitialize( NULL );

      if(pidl = SHBrowseForFolder(&bi))
        {
          if(SHGetPathFromIDList(pidl,szDir)) 
            {
             strcpy( WorkStr, szDir );
            }
                         
             pmalloc->Free(pmalloc);
             pmalloc->Release();
        }

    }   // end if SUCCEEDED...
  else 
   return NULL;

  
  return WorkStr;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:41:30 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:41:30 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
  TCHAR szDir[MAX_PATH];

  switch(uMsg) 
  {
   case BFFM_INITIALIZED: 
           
            if(GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR), szDir)) 
               SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
              
               break;
            

   case BFFM_SELCHANGED: // show path in dialog box...
            
              if(SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) 
                 SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
               
               break;
            
   default:   break;

  } // end switch...

 return 0;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:44:30 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:44:30 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
	/*	Win32 doesn't seem to have these functions. 
	**	Therefore implement inline versions of these functions here.
	*/
	
	__inline long int 
	lrint (double flt)
	{	int intgr;

		_asm
		{	fld flt
			fistp intgr
			} ;
			
		return intgr ;
	} 
	
	__inline long int 
	lrintf (float flt)
	{	int intgr;

		_asm
		{	fld flt
			fistp intgr
			} ;
			
		return intgr ;
	}




void BlitBgImage( HDC hDC )
{
  BITMAP bm;
  int w, h;

  GetObject( gBgImage, sizeof(BITMAP),(LPVOID) &bm);
  w = bm.bmWidth;
  h = bm.bmHeight;

  if( gCfg.Options & OPT_BG4TILE )
    {
     w = (gScreenX/2); // 1/4 "screen" size...
     h = (gScreenY/2);

     // tile this image 4 times...    
     // top left...
     DrawBitmapStretched( hDC, gBgImage, 0, 0, w, h ); 
     // top right...
     DrawBitmapStretched( hDC, gBgImage, (gScreenX/2), 0, w, h ); 

     // bottom left...
     DrawBitmapStretched( hDC, gBgImage, 0, (gScreenY - h), w, h ); 

     // bottom right...
     DrawBitmapStretched( hDC, gBgImage, (gScreenX/2), (gScreenY - h), w, h ); 
    }
  else if( gCfg.Options & OPT_BGTILE )
    {
     int TileX = (int) lrintf( (float)( gScreenX / w ) ) + 1;   // 1280 / 267  ( 4.7 )  5
     int TileY = (int) lrintf( (float)( gScreenY / h ) ) + 1;   // 1024 / 200  ( 5.12 ) 6
 

     for( int j = 0; j < TileY; j++)
        {
         for( int i = 0; i < TileX; i++ )
              DrawBitmapStretched( hDC, gBgImage, i*w, j*h, w, h );

        }
 
    }
   else if( gCfg.Options & OPT_BGTOPLEFT )
          { // scale this image to the top left of the preview screen...
            DrawBitmapStretched( hDC, gBgImage, 0, 0, w, h ); 
          }     
   else if( gCfg.Options & OPT_BGTOPCENTER )
          { // scale this image to the top left of the preview screen...
            DrawBitmapStretched( hDC, gBgImage, (gScreenX/2)-(w/2), 0, w, h ); 
          }           
   else if( gCfg.Options & OPT_BGBOTTOMCENTER )
          { // scale this image to the top left of the preview screen...
            DrawBitmapStretched( hDC, gBgImage,  (gScreenX/2)-(w/2), (gScreenY - h ), w, h ); 
          }      
   else if( gCfg.Options & OPT_BGBOTTOMLEFT )
          { // scale this image to the top left of the preview screen...
            DrawBitmapStretched( hDC, gBgImage, 0, (gScreenY - h ), w, h); 
          }         
   else if( gCfg.Options & OPT_BGCENTER )
          { // scale this image to the top left of the preview screen...
            DrawBitmapStretched( hDC, gBgImage, (gScreenX/2)-(w/2), (gScreenY/2)-(h/2), w, h ); 
          }              
         else // Default... stretch it.
            DrawBitmapStretched( hDC, gBgImage, 0, 0, gScreenX, gScreenY ); 
 
             
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:45:11 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:45:11 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void BlitLogoImage( HDC hDC )
{
  BITMAP bm;
  int w, h;

  GetObject( gLogoImage, sizeof(BITMAP),(LPVOID) &bm);
  w = bm.bmWidth;
  h = bm.bmHeight;
  
        
        if( gCfg.Options & OPT_LOGOTOPLEFT )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage, 0 ,0, gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, 0, 0 );
          }     
   else if( gCfg.Options & OPT_LOGOTOPCENTER )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage,(short)(gScreenX/2)-(w/2) ,0, gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, (gScreenX/2)-(w/2), 0 );
          }           
   else if( gCfg.Options & OPT_LOGOBOTTOMCENTER )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage, (short)(gScreenX/2)-(w/2), (short)(gScreenY - h), gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, (gScreenX/2)-(w/2), (gScreenY - h) );
          }      
   else if( gCfg.Options & OPT_LOGOBOTTOMLEFT )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage, 0, (short)(gScreenY - h), gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, 0, (gScreenY - h) );
          }         
   else if( gCfg.Options & OPT_LOGOCENTER )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage, (short)(gScreenX/2)-(w/2), (short)(gScreenY/2)-(h/2), gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage,  (gScreenX/2)-(w/2),  ((gScreenY/2)-(h/2)) );
          }
   else if( gCfg.Options & OPT_LOGOCUSTOM )
          { // scale this image to the top left of the preview screen...

           if( gCfg.Options & OPT_LOGOTRANSPARENT )
              DrawTransparentBitmap( hDC, gLogoImage, (short)gCfg.LogoXPos, (short)gCfg.LogoYPos, gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, gCfg.LogoXPos, gCfg.LogoYPos );
          }           
         else 
          {
           if( gCfg.Options & OPT_LOGOTRANSPARENT )
             DrawTransparentBitmap( hDC, gLogoImage, 0, 0, gCfg.LogoTransColor );
             else
              DrawBitmap( hDC, gLogoImage, 0, 0 );

          }
  
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:45:45 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:45:45 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void BlitBgColor( HDC hDC )
{
  RECT r;
  HBRUSH Old, hBRBg = CreateSolidBrush( gCfg.BgColor );
  HPEN OldPen, hPen = CreatePen( PS_SOLID, 1, gCfg.BgColor );

  Old = (HBRUSH) SelectObject( hDC, hBRBg );
  OldPen = (HPEN) SelectObject( hDC, hPen );

  r.left = 0;
  r.top  = 0;
  r.right  = gScreenX; // even aspect ratio of standard display types...
  r.bottom = gScreenY;

  Rectangle( hDC, r.left, r.top, r.right, r.bottom );

  DeleteObject( (HPEN) SelectObject( hDC, OldPen ) );
  DeleteObject( (HBRUSH) SelectObject( hDC, Old ) );
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:46:31 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:46:31 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DumpHexString( char *NormString, BYTE *String, DWORD StringLength, FILE *out )
{
  DWORD Len = 0;

  fprintf( out, "\r\n\r\n// HEX string:\r\n%s[%ld] = { ", NormString, StringLength );

  for( Len = 0; Len < ( StringLength - 1 ); Len++ )
     {
      fprintf( out, "0x%.2x,", String[ Len ] ); 
     }
  
  fprintf( out, "0x%.2x };\r\n", String[ Len ] ); 


}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:47:15 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:47:15 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
HDC CreateBackBufferDC( HDC hBlitTooDC, int w, int h )
{
  HDC BackBufferDC = CreateCompatibleDC( hBlitTooDC );
  HBITMAP hBM = CreateCompatibleBitmap( hBlitTooDC, w, h );

  SelectObject( BackBufferDC, hBM );
  SetMapMode(BackBufferDC, GetMapMode(  hBlitTooDC ) );

  DeleteObject( hBM );

  return BackBufferDC;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:47:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:47:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void BlitBackBuffer( HDC BkBuffDC, int x, int y, int BuffW, int BuffH, HDC hTooDC )
{

  SetStretchBltMode( hTooDC, COLORONCOLOR );
  StretchBlt( hTooDC, // dest
              PREVIEW_X, PREVIEW_Y, // dest x,y pos.
              PREVIEW_W, PREVIEW_H, // dest w,y of rect...
              BkBuffDC, // from
              0, 0,     // from x,y pos.
              gScreenX, gScreenY, // from w, h
              SRCCOPY );
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:48:43 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:48:43 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DrawPreviewAll( HDC hDC )
{
  HDC BkBuffDC = CreateBackBufferDC( hDC, gScreenX, gScreenY );

  // Draw back ground color...
  BlitBgColor(  BkBuffDC );
                 
  // Draw a background image if one...
  if( gBgImage ) BlitBgImage( BkBuffDC );

  // Draw logo next ( if one )
  if( gLogoImage ) BlitLogoImage( BkBuffDC );

  if( gCfg.Options & OPT_USINGTEXT )  
      BlitDisplayText( BkBuffDC  );

  // Blit back buffer to actual screen...
  BlitBackBuffer( BkBuffDC, // buffer to blit from
                  PREVIEW_X, PREVIEW_Y, // pos to blit too...
                  PREVIEW_W, PREVIEW_H, // size to scale too
                  hDC ); // HDC to blit too...

  DeleteDC( BkBuffDC );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:49:07 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:49:07 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ShowPreviewWindow( void )
{
  MSG msg;
  WNDCLASSEX wc;

  UnregisterClass( "PreviewWindow", ghInst );

  wc.cbSize = sizeof( wc );
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
  wc.hCursor = ( HCURSOR ) LoadCursor( ghInst, MAKEINTRESOURCE( IDC_HAND_CUR ) );//IDC_ARROW );
  wc.hIcon = ( HICON ) LoadIcon( ghInst, MAKEINTRESOURCE( IDI_ICON1) );//NULL, IDI_APPLICATION );
  wc.hIconSm = wc.hIcon;
  wc.hInstance = ghInst;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = PreviewWndProc;
  wc.lpszClassName = "PreviewWindow";
  wc.lpszMenuName = NULL;

  if(!(RegisterClassEx( &wc )))
    {
     OK_Msg("Error: 25", "Cannot register preview window class." );
     return; 
    } 

  if(! (ghPreviewWnd = CreateWindowEx( WS_EX_APPWINDOW, "PreviewWindow", "Preview window",
                              WS_POPUP,
                              0, 0,
                              gScreenX,
                              gScreenY,
                              NULL, // Parent
                              (HMENU) NULL, // Menu 
                              ghInst, NULL )))
    {
     OK_Msg("Error: 26", "Cannot create preview window ( 2 )." );
     return; 
    } 


  UpdateWindow( ghPreviewWnd );
  ShowWindow( ghPreviewWnd, SW_SHOW );

  while( GetMessage( &msg, NULL, 0, 0 ) )
  {
   TranslateMessage( &msg );
   DispatchMessage( &msg );
  }


}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:49:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:49:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void BlitDisplayText( HDC hDC )
{
  int Line = 0, Lines = 0;
  char TmpStr[ 1024 ];
  int Len = 0;
  char *p = NULL, *t = NULL;
  HFONT oldFont;
  int x, y, shadow_x, shadow_y;
  TEXTMETRIC tm;
  int ScreenX = GetSystemMetrics( SM_CXSCREEN );
  int ScreenY = GetSystemMetrics( SM_CYSCREEN );

  SetBkMode( hDC, TRANSPARENT );
  SetTextColor( hDC, gCfg.TextColor );
  SetBkColor( hDC, gCfg.BgColor );

  // Set the font size...
  oldFont = (HFONT) SelectObject (hDC, gCfg.hFont );


  x = gCfg.TextXPos;
  y = gCfg.TextYPos;
  shadow_x = gCfg.FontShadowXOffset;
  shadow_y = gCfg.FontShadowYOffset;

  // figure out where to start drawing this text...
  GetTextMetrics( hDC, &tm );
  
  // count how many lines of text are in this... 
  // save the lonest line length too...
  Len = 0;
  int Longest = 0;
  p = gCfg.DisplayText;
  while( *p )
  {
   if( *p == '\r' )
     {
      Lines++;
      if( Len > Longest )
          Longest = Len;
      Len = 0; 
     }

   Len++;
   p++; 
  }

 Len = Longest;

        if( gCfg.MoreOptions & OPT_TEXTTOPLEFT )  
          { // Top left of screen
            x = 0;
            y = 0;                        
          }
   else if( gCfg.MoreOptions & OPT_TEXTTOPCENTER )  
          { // Center of screen
           // find center
           x = (( ScreenX / 2 ) - (( Len * tm.tmAveCharWidth )/2));  
           y = 0;  
          }
   else if( gCfg.MoreOptions & OPT_TEXTBOTTOMLEFT )  
          { // Bottom left of screen
           x = 0;  
           y = (int)((float)ScreenY - ( (float)Lines * ((float)tm.tmHeight + ( (float)tm.tmHeight * .1 ))));    
          }
   else if( gCfg.MoreOptions & OPT_TEXTBOTTOMCENTER )  
          { // Bottom  center of screen
           x = (( ScreenX / 2 ) - (( Len * tm.tmAveCharWidth )/2));  
           y = (int)((float)ScreenY - ((float) Lines * (tm.tmHeight + ( (float)tm.tmHeight * .1 )) ));                          
          }
   else if( gCfg.MoreOptions & OPT_TEXTCENTER )  
          { // Bottom center of screen
           x = (( ScreenX / 2 ) - (( Len * tm.tmAveCharWidth )/2));  
           y = (int)(( ScreenY / 2 ) - (( Lines * (tm.tmHeight + ( (float)tm.tmHeight * .1 )) )/2));                         
          }
         else  
          {
           // ( its custom... x,y are set already )
           // just drop through...
          }



                    


  if( ! ( p = gCfg.DisplayText ))
          return; // point to users text ( with new line characters... )

  memset( TmpStr, 0, 1024 );
  t = TmpStr;
  while( *p ) // while users text characters
  {
   
   if( (*p != '\r') && (*p != '\n') )
     {
      *t++ = *p++; // copy characters...
     }
    else // end of this string ( new line )  
     {
      Len = strlen( TmpStr );

      if( Len )
        { 

         if( gCfg.Options & OPT_DROPSHADOW ) // FontShadowOffset
           { 
             SetTextColor( hDC, gCfg.ShadowColor );
             TextOut( hDC,   x + shadow_x,
                           ( y + Line ) + shadow_y, TmpStr, Len );

             SetTextColor( hDC, gCfg.TextColor );
           }

         TextOut( hDC, x, y + Line, TmpStr, Len );

         Line += (int)( (float)tm.tmHeight + ( (float)tm.tmHeight * .1 ) );
        }         
  
      memset( TmpStr, 0, 1024 );
      t = TmpStr;
      p++; // skip the '/r/n'...
     }

  }

 
 SelectObject( hDC, oldFont ); 

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:50:29 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:50:29 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
LRESULT CALLBACK PreviewWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
   case WM_PAINT:
                {
                 HDC hDC;
                 PAINTSTRUCT ps;
                 char TmpStr[ 255 ];
                 int Len = 0;

                 hDC = BeginPaint( hWnd, &ps );

                 DrawFullPreviewAll( hDC ); 
                 SetTextColor( hDC, gCfg.TextColor );
                 SetBkColor( hDC, gCfg.BgColor );
                 sprintf( TmpStr, "[ Hit any key or click the mouse to go back ]" );
                 Len = strlen( TmpStr );
                 TextOut( hDC, (gScreenX/2) - (Len * 3), (gScreenY - 50), TmpStr, Len );

                 EndPaint( hWnd, &ps );
                }  
                return 0;

   case WM_LBUTTONDOWN:
   case WM_KEYDOWN:
                   SetFocus( gPrevWnd );
                   DestroyWindow( hWnd );
                   PostQuitMessage( 0 );
                   return 0;
  }

  return DefWindowProc( hWnd, msg, wParam, lParam );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:51:44 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:51:44 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DrawFullPreviewAll( HDC hDC )
{
 
  // Draw back ground color...
  BlitBgColor(  hDC );
                 
  // Draw a background image if one...
  if( gBgImage ) BlitBgImage( hDC );

  // Draw logo next ( if one )
  if( gLogoImage ) BlitLogoImage( hDC );

  if( gCfg.Options & OPT_USINGTEXT )  BlitDisplayText( hDC );

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:52:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:52:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL SetupStatusBar( void )
{
  gNumFolders = 0;
  gNumFiles = 0;
  gTotalBytes = 0;

  UpdateStatusBar();

  return TRUE;              
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:53:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:53:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DrawLogoBitmapStretched(HDC hDC, HBITMAP hBM, int x, int y, int w, int h)
{
  BITMAP bm;
  HDC hdcMem;
  POINT ptSize, ptOrg;

  hdcMem = CreateCompatibleDC(hDC);
  SelectObject(hdcMem, hBM);
  SetMapMode(hdcMem, GetMapMode(hDC));

  GetObject(hBM, sizeof(BITMAP),(LPVOID) &bm);

  ptSize.x = bm.bmWidth;
  ptSize.y = bm.bmHeight;

  DPtoLP(hDC, &ptSize, 1);

  ptOrg.x = 0;
  ptOrg.y = 0;

  DPtoLP(hdcMem, &ptOrg, 1);

  DrawTransparentBitmap( hDC, hBM, x, y, gCfg.LogoTransColor );
 

  DeleteDC(hdcMem);

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:54:57 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:54:57 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// change gTotalBytes then call this...
void UpdateProgress( void )
{
  DWORD Mb = 0;

  Mb = (DWORD) ((gTotalBytes / 1024) / 1024);

  SendMessage( ghMainProgWnd, PBM_SETPOS, (WPARAM) Mb, (LPARAM) 0); 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:55:01 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:55:01 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ResetProgress( void )
{
/*
  SendMessage( ghMainProgWnd, PBM_SETRANGE, 0, MAKELPARAM( 0, gCfg.MaxMB ));  
  SendMessage( ghMainProgWnd, PBM_SETSTEP,  (WPARAM) 1, 0); 
  SendMessage( ghMainProgWnd, PBM_DELTAPOS, (WPARAM) 0, 0); 
  SendMessage( ghMainProgWnd, PBM_SETPOS,   (WPARAM) 0, 0); 
*/
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:56:02 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:56:02 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void CreateCD( void )
{
  // MAKE USER SET A PASSWORD ( if not already set )...
  // and if they have the option enabled..
  if( gCfg.Options & OPT_PASSPROTECT )
    {

     if( ! gCfg.Password[ 0 ] )
       {

        DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETPASSWORD_DLG ),
                   NULL,  SetPWDlgProc);

        if( ! gCfg.Password[ 0 ] ) //  still didn't set it...
          {
           OK_Msg("INFO:",  "Password still was not set. Please either set it or disable it under \"Settings\"");
           return;
          }

       } 

    }

   
 // Show burn setup...
 DialogBox( ghInst, MAKEINTRESOURCE( IDD_BURNREPORT_DLG ),
            NULL,  BurnReportDlgProc);

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:56:59 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:56:59 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/// sets individual user password...
BOOL CALLBACK SetUserPWDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static char Password[ 255 ], ConfirmPassword[ 255 ];

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 


           return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDOK: 
                  memset( Password, 0, 255 );
                  memset( ConfirmPassword, 0, 255 );


                  GetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDT), Password,
                                GetWindowTextLength(GetDlgItem(hDlg, IDC_PASSWORD_EDT) ) + 1);

                  GetWindowText(GetDlgItem(hDlg, IDC_CONFIRMPASSWORD_EDT), ConfirmPassword,
                                GetWindowTextLength(GetDlgItem(hDlg, IDC_CONFIRMPASSWORD_EDT) ) + 1);

                  if( ! Password[ 0 ] ) 
                    { 
                     OK_Msg("Info:", "Please enter a password in the \"Password edit box\"");
                     break;
                    }

                  if( ! ConfirmPassword[ 0 ] ) 
                    { 
                     OK_Msg("Info:", "Please confirm your password in the \"Confirm edit box\"");
                     break;
                    }

                  if( strcmp( Password, ConfirmPassword) ) 
                    { 
                     OK_Msg("Info:", "Password and confirm password did not match, please try again.");
                     break;
                    }



                  // add or with pos of letter ( so the password wornt work with the letter in any order... )
                  strcat( Password, XOR_PW_FIX );   

                  // add or with pos of letter ( so the password wornt work with the letter in any order... )
                  
                  strcpy( gTmpPassword, Password );

                  EndDialog(hDlg, TRUE);
                  break; 

         case IDCANCEL: 
                  EndDialog(hDlg, TRUE);
                  break;
        }

     break; 
   }

  return FALSE;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:01:16 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:01:16 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DecryptStaticGlobals( void )
{

  Encrypt( STATIC_DECRYPT_PW, REG_CODES_URL, 31 );
  Encrypt( STATIC_DECRYPT_PW, REG_CODE_SCRIPT, 13 );
  Encrypt( STATIC_DECRYPT_PW, REG_STARTUPCODE_SCRIPT, 16 );
  Encrypt( STATIC_DECRYPT_PW, LOCAL_REG_CODE_FILE, 11 );
  Encrypt( STATIC_DECRYPT_PW, ONLINE_STATIC_REG_PW, 21 );
  Encrypt( STATIC_DECRYPT_PW, EVAL_STRING, 18 );
  Encrypt( STATIC_DECRYPT_PW, VIEWER_PASSWORD, 20 );
  Encrypt( STATIC_DECRYPT_PW, CRYPTCD_WEBSITE, 22 );
  Encrypt( STATIC_DECRYPT_PW, DEFAULT_CREATEDIR, 35 );
  Encrypt( STATIC_DECRYPT_PW, VIEWERFILETOADD, 11 );
  Encrypt( STATIC_DECRYPT_PW, NORM_CHARS, 68 );
  Encrypt( STATIC_DECRYPT_PW, CRYPT_CHARS, 68 );
 

}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:01:55 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:01:55 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// shows all the files in the file list ( in debug box )
void DbShowFileList( void )
{
  char Buff[ 8192 ];
  LIST_ITEM  *FileList = gpListItem; 


  memset( Buff, 0, 8192 );

  int i = 0;
  while( FileList )
  {
   sprintf( &Buff[ strlen( Buff ) ], "%ld - %s\r\n", i, FileList ->FileName );

   FileList = FileList ->Next;
   i++;
  }

  OK_Msg("", "%s", Buff);

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:03:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:03:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DB_ShowList( void )
{
  LIST_ITEM  *pList = NULL;
  char TmpStr[ 1024 ];
  
  pList = gpListItem;

  if( ! pList ) 
    {
     OK_Msg("DEBUG!", "No Linked Listview items!");
     return;
    }

  while( pList )
  {
   sprintf( TmpStr, "ID: %ld\r\nPath: %s\r\nFileName: %s\r\nFileSize: %ld", 
            pList ->ID, pList->FilePath, pList ->FileName, pList ->FileSize );

   OK_Msg("DEBUG!", "%s", TmpStr );

   pList = pList ->Next;
  }

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:04:28 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:04:28 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ShutDown( void )
{


  if( gCfg.Options & OPT_AUTOSAVE )
      SaveSettings();                       
     else
      {
        // they don't want settings saved, if the .cfg file is there delete it!
        char *p = GetAppDirectory();
  
        char Filename[ 255 ];
        sprintf( Filename, "%s\\%s", p, SETTINGS_FILENAME );

        if( FileExists( Filename ) )
            DeleteFile( Filename );

      }


  Sleep( 1500 ); // XP bug... have to let the system catch up or it may crash ): ...Windows is LAME!!!


  if( ghCustIcon )
      DestroyIcon( ghCustIcon );

  if( gBgImage ) 
      DeleteObject( gBgImage );

  if( gLogoImage ) 
      DeleteObject( gLogoImage );

  if( gCCD4MainBmp ) // NEW 4.0
      DeleteObject( gCCD4MainBmp );

  if( gCCDCarMainBmp ) // NEW 4.0
      DeleteObject( gCCDCarMainBmp );

  if( gCreateBtnBmp ) // NEW 4.0
      DeleteObject( gCreateBtnBmp );

  if( gCustAboutImageBmp ) // NEW 4.0
      DeleteObject( gCustAboutImageBmp );

 
  ImageList_Destroy( gListImgs );	
  ImageList_Destroy( gDriveTreeImgs );	
  ImageList_Destroy( gTreeCDImgs );
  ListView_DeleteAllItems( ghFilesWnd );	

  ClearFilesLinkList();
  ClearCDTreeList(); 
  ClearUserList();

  if( gCfg.EulaText )
      CustFreeAllocatedMem( &gCfg.EulaText[0], "ShutDown()", __LINE__ );

  SilentDBMsg( "ShutDown()", "\r\n\r\n***Allocations=%ld - AllocatedBytes=%ld - Alloc Failures: %ld\r\nDeallocations:%ld - De-Alloc Bytes:%ld - De-alloc Failures: %ld\r\n\r\n",
                             gMemManager.TotalAllocations,
                             gMemManager.TotalAllocatedBytes,
                             gMemManager.TotalAllocationFailures,
                             gMemManager.TotalDeAllocations,
                             gMemManager.TotalDeAllocatedBytes,
                             gMemManager.TotalDeAllocationFailures );


  PostQuitMessage( 0 );
  exit( 0 ); 
     
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:05:25 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:05:25 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL InitApp( void )
{
  BOOL loaded = false;
  SIZE Size;

  gScreenX = GetSystemMetrics(SM_CXSCREEN);
  gScreenY = GetSystemMetrics(SM_CYSCREEN);
  
  gAppW = gScreenX;  // defaults... 
  gAppH = gScreenY;


  char *p = GetAppDirectory();
  
  char Filename[ 255 ];
  sprintf( Filename, "%s\\%s", p, SETTINGS_FILENAME );

  if( FileExists( Filename ) )
    {
      if( LoadSettings( Filename ) ) // they have settings saved, load 'um up!
          loaded = true;
    }


  memset( gCustAboutImageFile, 0, 255 );

  if( !(ghCustIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_ICON2 ))))
    {
     OK_Msg("Error: 123a", "Cannot load icon x477" );
     return FALSE;
    }

  if( !(ghDunnoIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_DUNNO ))))
    {
     OK_Msg("Error: 123", "Cannot load icon 0" );
     return FALSE;
    }


  if( !(ghCDRootIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_LOCKED_ICON ))))
    {
     OK_Msg("Error: 123", "Cannot load icon 1" );
     return FALSE;
    }

  if( !(ghLockedFolderOpenIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_LOCKFOLDER_OPEN ))))
    {
     OK_Msg("Error: 31823", "Cannot load icon 2" );
     return FALSE;
    }

  if( !(ghLockedFolderIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_LOCKFOLDER ))))
    {
     OK_Msg("Error: 33436", "Cannot load icon 3" );
     return FALSE;
    }

  if( !(ghCryptFileIcon = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_LOCKEDFILE_ICON ))))
    {
     OK_Msg("Error: 1255", "Cannot load icon 44" );
     return FALSE;
    }


  if( !(ghComputerIcon = LoadIcon( ghInst, MAKEINTRESOURCE(IDI_COMPUTER_ICON))))
    {
     OK_Msg("Error: 12", "Cannot load icon 5" );
     return FALSE;
    }

  if( !(ghDriveIcon  = LoadIcon( ghInst, MAKEINTRESOURCE(IDI_DRIVE))))
    {
     OK_Msg("Error: 13", "Cannot load icon 6" );
     return FALSE;
    }

  if( !(ghFolderOpen = LoadIcon( ghInst, MAKEINTRESOURCE(IDI_FOLDER_OPEN))))
    {     
     OK_Msg("Error: 14", "Cannot load icon 7" );
     return FALSE;
    }

  if( !(ghFolderIcon = LoadIcon( ghInst, MAKEINTRESOURCE(IDI_FOLDER))))
    { 
     OK_Msg("Error: 15", "Cannot load icon 8" );
     return FALSE;
    }

  if( !(ghBackFolder = LoadIcon( ghInst, MAKEINTRESOURCE(IDI_BACKFOLDER))))
    {
     OK_Msg("Error: 16", "Cannot load icon 9" );
     return FALSE;
    }

  if( !(ghBackToRoot = LoadIcon( ghInst, MAKEINTRESOURCE( IDI_BACKTOROOT))))
    {
     OK_Msg("Error: 17", "Cannot load icon 10" );
     return FALSE;
    }

  if(!(gSizeVertCursor  = (HCURSOR) LoadCursor( ghInst, MAKEINTRESOURCE( IDC_VERT_CURSOR ))))
    {
     OK_Msg("Error: 18", "Cannot load cursor 11" );
     return FALSE;
    }

  if(!(gSizeHorizCursor = (HCURSOR) LoadCursor( ghInst, MAKEINTRESOURCE( IDC_HORIZ_CURSOR ))))
    {
     OK_Msg("Error: 19", "Cannot load cursor 12" );
     return FALSE;
    }

  if(!(gArrowCursor = LoadCursor(NULL, MAKEINTRESOURCE( IDC_ARROW ) )))
    {
     OK_Msg("Error: 20", "Cannot load icon 13" );
     return FALSE;
    }

 
  if(!(gHandCursor = LoadCursor(ghInst, MAKEINTRESOURCE( IDC_HAND_CUR ) )))
    {
     OK_Msg("Error: 2240", "Cannot load icon 163" );
     return FALSE;
    }


  if(!(gCCD4MainBmp = LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_CCD4MAIN_BMP))))
    {
     OK_Msg("Error: 25vc", "Cannot load bitmap 13" );
     return FALSE;
    }


  if(!(gCreateBtnBmp = LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_CREATEBTN_BMP))))
    {
     OK_Msg("Error: 25vcb", "Cannot load bitmap 13r" );
     return FALSE;
    }

#if defined(FORENSIC_VER)

  if(!(gCCDCarMainBmp = LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_CARMAIN_BMP))))
    {
     OK_Msg("Error: 25vcb", "Cannot load bitmap 13b" );
     return FALSE;
    }

#endif


#if defined(PRO_VER) || (ENTERPRISE_VER) || (HOME_VER)
  if(!(gCCDCarMainBmp = LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_TOPLEFT_BMP))))
    {
     OK_Msg("Error: 25vcb", "Cannot load bitmap 13b" );
     return FALSE;
    }
#endif




  if(!(gMB_CDBmp = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_CD_MB)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 21", "Cannot load bitmap 14" );
     return FALSE;
    }

  if(!(gCryptTab = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_CDTITLE)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 71", "Cannot load bitmap 15" );
     return FALSE;
    }

  if(!(gFolderTitlebar = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_SOURCEFOLDER_TITLE)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 711", "Cannot load bitmap 151" );
     return FALSE;
    }

  if(!(gFilesTitlebar = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_SOURCEFILES_TITLE)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 715", "Cannot load bitmap 155" );
     return FALSE;
    }

  if(!(gEndTitlebar = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_TITLEBAR_END)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 712", "Cannot load bitmap 152" );
     return FALSE;
    }

  if(!(gRegLockPic = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_REGISTERPIC)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }


// anims...
  if(!(gCopyAnim[0] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM1)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[1] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM2)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[2] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM3)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[3] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM4)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[4] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM5)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[5] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM6)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }
  if(!(gCopyAnim[6] = MakeFastTrans( LoadBitmap( ghInst, MAKEINTRESOURCE(IDB_COPYANIM7)), GetSysColor( COLOR_BTNFACE ) )))
    {
     OK_Msg("Error: 91", "Cannot load bitmap 18" );
     return FALSE;
    }



  GetBitmapSize( &Size, gMB_CDBmp );
  MbCDPic.ID = ID_MB_CD_BMP; 
  MbCDPic.x = ( gAppW - 98 );
  MbCDPic.y = ( gAppH / 2) - 31;
  MbCDPic.w = Size.cx;
  MbCDPic.h = Size.cy;

  GetBitmapSize( &Size, gCryptTab );
  CryptTabPic.ID = ID_CRYPT_TAB_BMP; 
  CryptTabPic.x = 0;
  CryptTabPic.y = 0;
  CryptTabPic.w = Size.cx;
  CryptTabPic.h = Size.cy;


  if( ! loaded ) // Set standard default options...
    {
      memset( &gCfg, 0, sizeof( CFG ) );

      gCfg.BgColor   = RGB( 0,0,255 );
      gCfg.TextColor = RGB( 255, 255, 255 );
      gCfg.FontSize  = 12; // Default...

      gCfg.NumReps = 7;
  
      
      gCfg.Options |= OPT_CLEARRECENT;
      gCfg.Options |= OPT_PASSPROTECT;
      gCfg.Options |= OPT_AUTOBOOT;
      gCfg.Options |= OPT_AUTOSAVE;

      strcpy( gCfg.CustViewerFilename, "setup.exe");
      strcpy( gCfg.CustDatExt, "dat");
      strcpy( gCfg.CustDatName, "data");
   }
  
  GetModuleFileName(NULL, (char *) gThisFilePath, 255);
  StripFilenameFromPath( gThisFilePath);
 
  sprintf( gCfg.CreateCDDir, "%s", GetBurnPath() );
 
  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:06:22 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:06:22 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

  display, add or edit users...

*/
BOOL CALLBACK AdminUsersDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {                     

                        // if no users, just put no current users in combo box...
                        if( ! ( gUserListHead ) ) 
                          {
                           SendMessage( GetDlgItem( hDlg,IDC_USER_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "- No current users setup -" );
                           SendMessage( GetDlgItem( hDlg, IDC_USER_COMBO ), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );
                           break;
                          }


                        // reset user list...
                        ResetShowUsers( GetDlgItem( hDlg, IDC_USER_COMBO ) );

                       }
                       return TRUE;

    case WM_PAINT:
                 {
                  PAINTSTRUCT ps;
                  HDC hDC = BeginPaint( hDlg,&ps );
                  EndPaint(hDlg, &ps );
                 }
                 return TRUE;



	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
 
         case IDC_EDITUSER_BTN:
                              {
                               char Name[ 255 ];

                               memset( Name, 0, 255 );

                               // get the user ID to edit...
                               int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_USER_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                               SendMessage(  GetDlgItem( hDlg, IDC_USER_COMBO ), 
                                                           (UINT) CB_GETLBTEXT,
                                                           (WPARAM) lResult, (LPARAM) Name );  
   
                              if(! gUserListHead)                                 
                                {
                                  OK_Msg("INFO", "No users to edit. Add a user first." );
                                  break;
                                 }

                               if(! Name[0])
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }

                               gTmpEditUser = NULL;
                               USER  *TmpNode = gUserListHead;           
                               BOOL Found = FALSE;
                               while( TmpNode )
                                    {
 
                                     if( ! strcmp( Name, TmpNode ->UserName ) )
                                       {
                                        Found = TRUE;
                                        gTmpEditUser = TmpNode;
                                        break;
                                       }  

                                     TmpNode = TmpNode ->Next;
                                    }                                

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }


                               ShowWindow( hDlg, SW_HIDE );
                               DialogBox( ghInst, MAKEINTRESOURCE( IDD_EDITUSER_DLG ),
                                          NULL, EditUserDlgProc);
                               ShowWindow( hDlg, SW_SHOW );

                               // reset user list...
                               ResetShowUsers( GetDlgItem( hDlg, IDC_USER_COMBO ) );
                              }
                              break;

         case IDC_ADDUSER_BTN:
                               gTmpEditUser = NULL;
                               ShowWindow( hDlg, SW_HIDE );
                               DialogBox( ghInst, MAKEINTRESOURCE( IDD_ADDUSER_DLG ),
                                          NULL, AddUserDlgProc);
                               ShowWindow( hDlg, SW_SHOW );

                               // reset user list...
                               ResetShowUsers( GetDlgItem( hDlg, IDC_USER_COMBO ) );
                               break;


         // changed their mind...
         case IDCANCEL:
                   EndDialog(hDlg, TRUE);
                   break;

         }

     break; 
   }

  return FALSE;
}