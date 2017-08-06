
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:15:34 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:15:34 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK CreatedDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {
 
                        SetWindowText( GetDlgItem( hDlg, IDC_CREATEDIR_STATIC ), gCfg.CreateCDDir );
                        SetWindowText( GetDlgItem( hDlg, IDC_BURN_DIR_STATIC ), gCfg.CreateCDDir );

                       }
                       return TRUE;


	case WM_COMMAND:

		switch( GET_WM_COMMAND_ID( wParam, lParam ) )
		{
         case IDC_PREVIEW_BTN:
                              RunCD();
                              break;
#if !defined(HOME_VER)

         case IDC_COPYIT_BTN:
                            DialogBox( ghInst, MAKEINTRESOURCE( IDD_COPYTOO_DLG ),
                                      NULL,  CopyTooDlgProc);
                             break;
#endif

         case IDC_BURNCD_DVD_BTN:
                                 // Put up burn dialog box...
                                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_NEWBURN_DLG ),
                                      NULL,  NewBurnDlgProc );

                                 break;
         case IDCANCEL:
         case IDOK: // "Done >>"
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
 * Date:          12/17/2014  ( 5:16:19 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:16:19 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Run the generated CD on disk...
void RunCD( void )
{
  char TmpStr[ 255 ];
 
  sprintf(TmpStr, "%s\\%s", gCfg.CreateCDDir, gCfg.CustViewerFilename );
  
  ShellExecute( NULL, "open", TmpStr, NULL, gCfg.CreateCDDir, SW_NORMAL );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:17:04 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:17:04 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK CopyTooDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {
                        char TmpStr[ 255 ];

                        memset( TmpStr, 0, 255 );

                        // Get and parse through the drives...
                        if(! GetLogicalDriveStrings( 255, TmpStr ) ) // c:/<null>d:/<null><null>
                          {
                           OK_Msg("Error: 52", "Could not get drive strings..." );
                           return FALSE; 
                          } 
  

                         if( ! TmpStr[ 0 ] )
                           {
                            OK_Msg("Error: 53", "drives strings NULL..." );
                            return FALSE; 
                           } 
 

                         char *p = TmpStr; // convert NULL's so we can use strtok()...
                         while( 1 )
                              {
                               if( (! *p) && (! *(p+1)))
                                   break;

                               if( ! *p ) *p = '*';

                               p++;
                              } 



                         // Add the drives...
                         p = strtok( TmpStr, "*" );
                         int i = 0;
                         while( p ) 
                              {
                               // While there are tokens in "string"... 
                               p[ strlen( p ) - 1 ] = '\0';
                               
                               SendMessage( GetDlgItem( hDlg, IDC_DRIVES_COMBO ), CB_INSERTSTRING, 0, (LPARAM) p);

                               p = strtok( NULL, "*" ); // Get next token: 
                               i++;
                              }

                        SendMessage( GetDlgItem( hDlg, IDC_DRIVES_COMBO ), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );                     
                       }
                       return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
                   EndDialog(hDlg, TRUE);
                   break;

         case IDOK:
                   {
                    // Get the selected drive...
                    char TmpStr[ 255 ];

                    memset( TmpStr, 0, 255 );

                    int Index = SendMessage( GetDlgItem( hDlg, IDC_DRIVES_COMBO ), CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0 );                     
                    SendMessage( GetDlgItem( hDlg, IDC_DRIVES_COMBO ), (UINT) CB_GETLBTEXT, (WPARAM) Index, (LPARAM) TmpStr );  

                    strcat( TmpStr, "\\" ); 
                    XCopy( gCfg.CreateCDDir,  TmpStr );                   

                    EndDialog(hDlg, TRUE);
                   }
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
 * Date:          12/17/2014  ( 5:22:08 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:22:08 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
////////////////
void SplashWindow( void )
{
  MSG msg;
  WNDCLASSEX wc;
 
  wc.cbSize = sizeof( wc );
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = ( HBRUSH ) GetStockObject( BLACK_BRUSH );
  wc.hCursor = ( HCURSOR ) LoadCursor( ghInst, MAKEINTRESOURCE( IDC_HAND_CUR ) );//IDC_ARROW );
  wc.hIcon = ( HICON ) LoadIcon( ghInst, MAKEINTRESOURCE( IDI_ICON1) );//NULL, IDI_APPLICATION );
  wc.hIconSm = wc.hIcon;
  wc.hInstance = ghInst;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = SplashWndProc;
  wc.lpszClassName = "SplashWindow";
  wc.lpszMenuName = NULL;

  if(!(RegisterClassEx( &wc )))
    {
     OK_Msg("Error: 25", "Cannot register splash window class." );
     return; 
    } 

  if(! (ghSplashWnd = CreateWindowEx( WS_EX_APPWINDOW,
                             "SplashWindow", NULL,
                              WS_POPUP,
                              (gScreenX/2)-(394/2), (gScreenY/2)-(261/2),
                              411, 272,
                              NULL, // Parent
                              (HMENU) NULL, // Menu 
                              ghInst, NULL )))
    {
     OK_Msg("Error: 26", "Cannot create splash window ( 2 )." );
     return; 
    } 


  UpdateWindow( ghSplashWnd );
  ShowWindow( ghSplashWnd, SW_SHOW );

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
 * Date:          12/17/2014  ( 5:22:13 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:22:13 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
LRESULT CALLBACK SplashWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  static HBITMAP hBM;

  switch( msg )
  {
   case WM_CREATE:
                  hBM = LoadBitmap( ghInst, MAKEINTRESOURCE( IDB_CCDSPLASH_BMP )  );
                  SetTimer( hWnd, 1, SPLASHSCREEN_DELAY, NULL );
                  return 0;
  case WM_PAINT:
                {
                 HDC hDC;
                 PAINTSTRUCT ps; 
                 HFONT oldFont;

                 hDC = BeginPaint( hWnd, &ps );

                 DrawBitmap( hDC, hBM, 0, 0 );      
   
                 SetBkMode(hDC, TRANSPARENT ); 
                 SetTextColor( hDC, RGB( 255,255,255 ));              

 
                 oldFont = (HFONT) SelectObject (hDC, CreateFont(
                                10,  // nHeight
                                0, 0, 0, FW_BOLD,
                                FALSE,  // bItalic
                                FALSE,  // bUnderline
                                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                                DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                                "Arial")); 

                 DeleteObject( SelectObject( hDC, oldFont ) ); 

                 EndPaint( hWnd, &ps );
                }  
                return 0;

   case WM_TIMER:
   case WM_LBUTTONDOWN:
   case WM_KEYDOWN:
   case WM_CLOSE:
                   KillTimer( hWnd, 1 );
                   DeleteObject( hBM ); 
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
 * Date:          12/17/2014  ( 9:10:47 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:10:47 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*
  
  Gets the last node in the users list...

*/
USER *GetLastUserNode( void )
{
  USER *User = NULL;

  User = gUserListHead;

  if(! User )
    {
     // No list ( or items ) yet...
     return NULL;
    }

  while( User ->Next )
  {
   User = User ->Next;
  }

  return User;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:11:35 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:11:35 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
////
/*
   NOTE: Name and password have already been allocated and set in calling function...
*/
BOOL AddUser( USER *UserData ) 
{
  static DWORD ID = 1;
  USER *NewUser = NULL;

  if(!(NewUser = (USER *) CustAllocateMem( sizeof(USER)+100, "AddUser()", __LINE__ )) )
    { 
     OK_Msg("Error: (3423bcw)", "Cannot add user.");
     return FALSE;
    }
  
  memset( NewUser, 0, sizeof( USER ) + 50);  // zero new mem...

  NewUser ->UserPermissions = UserData ->UserPermissions;
  NewUser ->UserFlags       = UserData ->UserFlags;

  // attach ptrs to mem already filled in...
  NewUser ->UserName = UserData ->UserName;
  NewUser ->Password = UserData ->Password;
  NewUser ->UserNote = UserData ->UserNote;

  memcpy( &(NewUser ->ExpireDate), &(UserData ->ExpireDate), sizeof( SYSTEMTIME ) );

  NewUser ->Last = GetLastUserNode(); 
  NewUser ->Next = NULL;

  NewUser ->User_ID = ++ID;

  // Hook up to previous item...
  if( NewUser ->Last ) // if not ROOT node
      NewUser ->Last ->Next = NewUser;
    else
      gUserListHead = NewUser;
      

  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:12:39 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:12:39 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void RemoveUser( USER *User )
{
  USER *BeforeItem = User ->Last,
       *AfterItem  = User ->Next;

  if( BeforeItem )
      BeforeItem ->Next = AfterItem;
    else
      gUserListHead = AfterItem; // make root
  
  if( AfterItem )
      AfterItem ->Last = BeforeItem;
    
  if( User ->UserName )
      CustFreeAllocatedMem( User ->UserName, "RemoveUser()", __LINE__ );

  if( User ->Password )
      CustFreeAllocatedMem( User ->Password, "RemoveUser()", __LINE__  );

  if( User ->UserNote ) 
      CustFreeAllocatedMem( User ->UserNote, "RemoveUser()", __LINE__  );
  
  // remove all the user ID's from the crypt nodes...
  CRYPT_ITEM *TmpCryptItem = gCryptCDItemRoot;
  while( TmpCryptItem )
       { 

        USER_ID_LIST *TmpUserId = TmpCryptItem ->ID_List_Head;
        while( TmpUserId )
             {
              if( TmpUserId ->User_ID == User ->User_ID )
                  RemoveUserIDNode( TmpCryptItem, TmpUserId );

              TmpUserId = TmpUserId ->Next;
             }

        TmpCryptItem =TmpCryptItem ->Next;
       }

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:13:20 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:13:20 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void RemoveUserIDNode( CRYPT_ITEM *CryptItem, USER_ID_LIST *TmpUserId )
{
  USER_ID_LIST *BeforeItem = TmpUserId ->Last,
               *AfterItem  = TmpUserId ->Next;

  if( BeforeItem )
      BeforeItem ->Next = AfterItem;
    else
      CryptItem ->ID_List_Head = AfterItem; // make root
  
  if( AfterItem )
      AfterItem ->Last = BeforeItem;
    
  if( TmpUserId ) 
      CustFreeAllocatedMem( TmpUserId, "RemoveUserIDNode()", __LINE__  );

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:14:03 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:14:03 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ClearUserList( void )
{
  USER  *TmpNode = NULL;

  if(! gUserListHead) return; // No list

  while( gUserListHead )
  {
   TmpNode = gUserListHead; 

   gUserListHead = gUserListHead ->Next;

   if( TmpNode ->UserName )
       CustFreeAllocatedMem( TmpNode ->UserName, "ClearUserList()", __LINE__ );

   if( TmpNode ->Password )
       CustFreeAllocatedMem( TmpNode ->Password, "ClearUserList()", __LINE__ );

   if( TmpNode ->UserNote )
       CustFreeAllocatedMem( TmpNode ->UserNote, "ClearUserList()", __LINE__ );


   CustFreeAllocatedMem( TmpNode, "ClearUserLinkList()", __LINE__ );
  }

  gUserListHead = NULL;

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:14:46 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:14:46 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *GetUserNameFromID( DWORD User_ID )
{
  static char *User_Name = NULL;
  USER *TmpNode = gUserListHead;

  while( TmpNode )
  {
   if( TmpNode ->User_ID == User_ID )
       return TmpNode ->UserName; 

   TmpNode = TmpNode ->Next;
  }
  

  return "User name NOT found.";
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:15:22 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:15:22 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*
  re-sets the users dropdown combo box...
*/
void ResetShowUsers( HWND hWndCombo )
{
  
  // reset the combo...
  SendMessage( hWndCombo, CB_RESETCONTENT , (WPARAM) 0, (LPARAM) 0 );

  if( ! ( gUserListHead ) ) 
    {
      SendMessage( hWndCombo, CB_INSERTSTRING, 0, (LPARAM) "- No current users setup -" );
      SendMessage( hWndCombo, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );
      return;
    }

  // display users in the combo box...
  // loop through all the user structs displaying the names...
  int i = 0;
  USER *TmpNode = gUserListHead;
  while( TmpNode )
	   {
	    SendMessage( hWndCombo, CB_INSERTSTRING, 0, (LPARAM) TmpNode ->UserName );
        TmpNode = TmpNode ->Next;
        i++;
       }

  SendMessage( hWndCombo, CB_SETCURSEL, (WPARAM) 0, (LPARAM) i );
              
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:16:40 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:16:40 PM )
 *                                       
 * BUGS:          BROKEN, not showing users attached to a crypt item!                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ResetShowAssignedUsers( CRYPT_ITEM *CryptItem, HWND hWndCombo )
{
  
  // reset the combo...
  SendMessage( hWndCombo, CB_RESETCONTENT , (WPARAM) 0, (LPARAM) 0 );

  if( ! ( CryptItem ->ID_List_Head ) ) 
    {
      SendMessage( hWndCombo, CB_INSERTSTRING, 0, (LPARAM) "- No users assigned to this item -" );
      SendMessage( hWndCombo, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );
      return;
    }

  // display users in the combo box...
  // loop through all the user structs displaying the names...
  int i = 0;
  USER_ID_LIST *TmpNode = CryptItem ->ID_List_Head;
  while( TmpNode )
	   {
	    SendMessage( hWndCombo, CB_INSERTSTRING, 0, (LPARAM) GetUserNameFromID( TmpNode ->User_ID ) );
        TmpNode = TmpNode ->Next;
        i++;
       }

  SendMessage( hWndCombo, CB_SETCURSEL, (WPARAM) 0, (LPARAM) i );
              
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:17:34 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:17:34 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*
  TODO:
*/
BOOL AddUserIDToCryptItem( CRYPT_ITEM *CryptItem, DWORD User_ID )
{
  USER_ID_LIST *TmpListID = CryptItem ->ID_List_Head;

  // goto the end of the list...
  if(! TmpListID ) // creating ROOT node?
    {
      // Add a User_ID to the UserID list ( in this Crypt Item struct )...
      if(! ( TmpListID = (USER_ID_LIST *) CustAllocateMem( sizeof(USER_ID_LIST) + 10,  "AddUserIDToCryptItem()", __LINE__ ) ) )
        {
         OK_Msg( "ERROR:", "Cannot add ROOT user ID to item user ID list." );
         return FALSE;
        }

      TmpListID ->Next = NULL;
      TmpListID ->Last = NULL; 
      TmpListID ->User_ID = User_ID;

      CryptItem ->ID_List_Head = TmpListID; 
    }
   else // adding a node to the end...
    {

      while( TmpListID ->Next )
           {
            TmpListID = TmpListID ->Next;
           }

      // Add a User_ID to the UserID list ( in this Crypt Item struct )...
      if(! ( TmpListID ->Next = (USER_ID_LIST *) CustAllocateMem( sizeof(USER_ID_LIST) + 10,  "AddUserIDToCryptItem()", __LINE__ ) ) )
        {
         OK_Msg( "ERROR:", "Cannot add user ID to item user ID list." );
         return FALSE;
        }

      TmpListID ->Next ->Last = TmpListID;

      TmpListID = TmpListID ->Next;  

      TmpListID ->User_ID = User_ID;
      TmpListID ->Next = NULL;
   }


  CryptItem ->NumItemUsers++;

  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:18:48 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:18:48 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
HIMAGELIST GetSystemImageList( BOOL fSmall )
{
   HIMAGELIST  himl;
   SHFILEINFO  sfi;

   himl = ( HIMAGELIST ) SHGetFileInfo( TEXT("C:\\"), 0, &sfi,
      sizeof( SHFILEINFO ), SHGFI_SYSICONINDEX |
         (fSmall ? SHGFI_SMALLICON : SHGFI_LARGEICON) );

 /*
   Do a version check first because you only need to use this code on
   Windows NT version 4.0.
 */ 
   OSVERSIONINFO vi;
   vi.dwOSVersionInfoSize = sizeof( vi );
   GetVersionEx( &vi );
   if( VER_PLATFORM_WIN32_WINDOWS == vi.dwPlatformId )
       return himl;

   /*
   You need to create a temporary, empty .lnk file that you can use to
   pass to IShellIconOverlay::GetOverlayIndex. You could just enumerate
   down from the Start Menu folder to find an existing .lnk file, but
   there is a very remote chance that you will not find one. By creating
   your own, you know this code will always work.
   */ 
   HRESULT           hr;
   IShellFolder      *psfDesktop = NULL;
   IShellFolder      *psfTempDir = NULL;
   IMalloc           *pMalloc = NULL;
   LPITEMIDLIST      pidlTempDir = NULL;
   LPITEMIDLIST      pidlTempFile = NULL;
   TCHAR             szTempDir[MAX_PATH];
   TCHAR             szTempFile[MAX_PATH] = TEXT("");
   TCHAR             szFile[MAX_PATH];
   HANDLE            hFile;
   int               i;
   OLECHAR           szOle[MAX_PATH];
   DWORD             dwAttributes;
   DWORD             dwEaten;
   IShellIconOverlay *psio = NULL;
   int               nIndex;

   // Get the desktop folder.
   hr = SHGetDesktopFolder(&psfDesktop);
   if(FAILED(hr))
      goto exit;

   // Get the shell's allocator.
   hr = SHGetMalloc(&pMalloc);
   if(FAILED(hr))
      goto exit;

   // Get the TEMP directory.
   if(!GetTempPath(MAX_PATH, szTempDir))
      {
      /*
      There might not be a TEMP directory. If this is the case, use the
      Windows directory.
      */ 
      if(!GetWindowsDirectory(szTempDir, MAX_PATH))
         {
         hr = E_FAIL;
         goto exit;
         }
      }

   // Create a temporary .lnk file.
   if(szTempDir[lstrlen(szTempDir) - 1] != '\\')
      lstrcat(szTempDir, TEXT("\\"));
   for(i = 0, hFile = INVALID_HANDLE_VALUE;
      INVALID_HANDLE_VALUE == hFile;
      i++)
      {
      lstrcpy(szTempFile, szTempDir);
      wsprintf(szFile, TEXT("temp%d.lnk"), i);
      lstrcat(szTempFile, szFile);

      hFile = CreateFile(  szTempFile,
                           GENERIC_WRITE,
                           0,
                           NULL,
                           CREATE_NEW,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);

      // Do not try this more than 100 times.
      if(i > 100)
         {
         hr = E_FAIL;
         goto exit;
         }
      }

   // Close the file you just created.
   CloseHandle(hFile);
   hFile = INVALID_HANDLE_VALUE;

   // Get the PIDL for the directory.
   LocalToWideChar(szOle, szTempDir, MAX_PATH);
   hr = psfDesktop->ParseDisplayName(  NULL,
                                       NULL,
                                       szOle,
                                       &dwEaten,
                                       &pidlTempDir,
                                       &dwAttributes);
   if(FAILED(hr))
      goto exit;

   // Get the IShellFolder for the TEMP directory.
   hr = psfDesktop->BindToObject(   pidlTempDir,
                                    NULL,
                                    IID_IShellFolder,
                                    (LPVOID*)&psfTempDir);
   if(FAILED(hr))
      goto exit;

   /*
   Get the IShellIconOverlay interface for this folder. If this fails,
   it could indicate that you are running on a pre-Internet Explorer 4.0
   shell, which doesn't support this interface. If this is the case, the
   overlay icons are already in the system image list.
   */ 
   hr = psfTempDir->QueryInterface(IID_IShellIconOverlay, (LPVOID*)&psio);
   if(FAILED(hr))
      goto exit;

   // Get the PIDL for the temporary .lnk file.
   LocalToWideChar(szOle, szFile, MAX_PATH);
   hr = psfTempDir->ParseDisplayName(  NULL,
                                       NULL,
                                       szOle,
                                       &dwEaten,
                                       &pidlTempFile,
                                       &dwAttributes);
   if(FAILED(hr))
      goto exit;

   /*
   Get the overlay icon for the .lnk file. This causes the shell
   to put all of the standard overlay icons into your copy of the system
   image list.
   */ 
   hr = psio->GetOverlayIndex(pidlTempFile, &nIndex);

   exit:
   // Delete the temporary file.
   DeleteFile(szTempFile);

   if(psio)
      psio->Release();

   if(INVALID_HANDLE_VALUE != hFile)
      CloseHandle(hFile);

   if(psfTempDir)
      psfTempDir->Release();

   if(pMalloc)
      {
      if(pidlTempFile)
         pMalloc->Free(pidlTempFile);

      if(pidlTempDir)
         pMalloc->Free(pidlTempDir);

      pMalloc->Release();
      }

   if(psfDesktop)
      psfDesktop->Release();

   return himl;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:19:59 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:19:59 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL BitmapToIconInfoEx(HDC hdcRef, HBITMAP hBmpSrc, ICONINFO* pii, COLORREF crTransparent)
{
  COLORREF crBkgnd, crText;
  BITMAP   bm;
  HDC      hdc1, hdc2;

    GetObject(hBmpSrc, sizeof(bm), &bm);

    hdc1          = CreateCompatibleDC(hdcRef);
    hdc2          = CreateCompatibleDC(hdcRef);
    pii->fIcon    = TRUE;
    pii->xHotspot = 0;
    pii->yHotspot = 0;
    pii->hbmMask  = (HBITMAP)SelectObject(hdc1, CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL));
    pii->hbmColor = (HBITMAP)SelectObject(hdc2, CopyImage(hBmpSrc, IMAGE_BITMAP, 
                                       bm.bmWidth, bm.bmHeight, 0));
 
    crBkgnd       = SetBkColor(hdc2, crTransparent);

    BitBlt(hdc1, 0, 0, bm.bmWidth, bm.bmHeight, hdc2, 0, 0, SRCCOPY);

    SetBkColor(hdc2, RGB(0x00, 0x00, 0x00));
    crText = SetTextColor(hdc2, RGB(0xFF, 0xFF, 0xFF));

    BitBlt(hdc2, 0, 0, bm.bmWidth, bm.bmHeight, hdc1, 0, 0, SRCAND);

    SetTextColor(hdc2, crText);
    SetBkColor(hdc2, crBkgnd);

    pii->hbmColor = (HBITMAP)SelectObject(hdc2, pii->hbmColor);
    pii->hbmMask  = (HBITMAP)SelectObject(hdc1, pii->hbmMask);

    DeleteDC(hdc2);
    DeleteDC(hdc1);

  return(TRUE);
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:20:36 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:20:36 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
HICON BitmapToIconEx(HDC hdcRef, HBITMAP hBitmap, COLORREF crTransparent)
{
  ICONINFO ii;
  HICON    hIcon;

    if(!BitmapToIconInfoEx(hdcRef, hBitmap, &ii, crTransparent))
        return(NULL);

    hIcon = CreateIconIndirect(&ii);

    DeleteObject((HGDIOBJ)ii.hbmMask);
    DeleteObject((HGDIOBJ)ii.hbmColor);

  return(hIcon);
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:22:01 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:22:01 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void GetBitmapSize( SIZE *Size, HBITMAP hBM )
{

  BITMAP bm;

  GetObject(hBM, sizeof(BITMAP),(LPVOID) &bm); 

  Size ->cx = bm.bmWidth;
  Size ->cy = bm.bmHeight;

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:22:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:22:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Trans color is annoying pink (255,0,255)
// bgColor is background color to blit onto...
HBITMAP MakeFastTrans( HBITMAP hBM, DWORD bgColor )
{
  BITMAP bm;
  HBITMAP hNewBitmap;
  RECT r;

  HDC hdcScreen = CreateDC("DISPLAY", (LPCSTR) NULL, 
                           (LPCSTR) NULL, (CONST DEVMODE *) NULL); 

  HDC hDCMem = CreateCompatibleDC(hdcScreen); 

  SelectObject(hDCMem, hBM);
  SetMapMode(hDCMem, GetMapMode(hdcScreen));

  GetObject(hBM, sizeof(BITMAP),(LPVOID) &bm); 


  hNewBitmap = CreateCompatibleBitmap( hdcScreen, bm.bmWidth, bm.bmHeight );  
  SelectObject(hDCMem, hNewBitmap);

  r.left = 0;
  r.top = 0;
  r.right = bm.bmWidth;   
  r.bottom = bm.bmHeight;   
  FillRect( hDCMem, &r, CreateSolidBrush( bgColor ) );

  DrawTransparentBitmap( hDCMem, hBM, 0,0, RGB(255,0,255) );

  DeleteDC(hDCMem);

  return hNewBitmap;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:23:29 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:23:29 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
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



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:26:00 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:26:00 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD GetColor( void )
{
  static CHOOSECOLOR cc;
  static COLORREF Cust[ 16 ];
  DWORD res = 0;

  cc.lStructSize = sizeof( CHOOSECOLOR );
  cc.hwndOwner = NULL;
  cc.hInstance = NULL;
  cc.rgbResult = res;
  cc.lpCustColors = Cust;
  cc.Flags = CC_RGBINIT | CC_FULLOPEN;
  cc.lCustData = 0L;
  cc.lpfnHook  = NULL;
  cc.lpTemplateName = NULL;

  ChooseColor( &cc );

  return cc.rgbResult;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:28:03 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:28:03 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// char *TypeDesc, char *Type
char *GetImageFileName(void)
{

 static OPENFILENAME ofn;
 static char Filter[255];
 static char TmpStr[1024];
 char TestStr[ 255 ];

 memset(Filter,0,255);

 strcpy(Filter, "Image files (*.jpg,*.bmp)");
 strcpy(&Filter[strlen(Filter)+1],"*.BMP;*.JPG");

 memset(TmpStr,0,1024);

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = NULL; // hWnd;
 ofn.hInstance = ghInst;
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
 ofn.lpstrDefExt = "*";
 ofn.lCustData = 0L;
 ofn.lpfnHook = NULL;
 ofn.lpTemplateName = NULL;
 
 GetOpenFileName(&ofn);

 strcpy( TestStr, &TmpStr[ strlen( TmpStr ) - 3] );

 if( (! _stricmp( TestStr, "jpg")) ||  (! _stricmp( TestStr, "bmp") ))
    return ((char *) TmpStr);
     

 OK_Msg("Wrong file type:","You must load either a .JPG or .BMP file.");

 // didn't load anything ( cancel ) or 
 // not correct image type...
  strcpy( TmpStr, " " );
  return ((char *) TmpStr);; 
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:28:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:28:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void DrawBitmapStretched(HDC hDC, HBITMAP hBM, int x, int y, int w, int h)
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
  
  SetStretchBltMode( hDC, COLORONCOLOR );

  StretchBlt( hDC,
              x, y, // destination upper-left corner
              w, h, // width, Height of destination rectangle
              hdcMem, 
              ptOrg.x, ptOrg.y, // source upper-left corner..  
              ptSize.x, ptSize.y, // width/Height of source rectangle
              SRCCOPY ); 

  DeleteDC(hdcMem);

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:30:12 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:30:12 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/* MUST HAVE THESE...
#include < olectl.h >

#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)
*/
HBITMAP LoadJPG( char *Filename, DWORD *JpgWidth, DWORD *JpgHeight )
{
  HBITMAP hBM;
  HDC hdcScreen;
  HDC hDCMem;
  BITMAP bm;
  OLE_XSIZE_HIMETRIC Width;
  OLE_YSIZE_HIMETRIC Height;
  IPicture *Picture = NULL;
  WCHAR WideFileName[_MAX_PATH];
 
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Filename, -1,
                      WideFileName, _MAX_PATH);
 
  if(FAILED(OleLoadPicturePath( WideFileName, 0, 0, 0, IID_IPicture, (void**)&Picture)))
    { 
     MessageBox(NULL, "OleLoad FAILED!", "ERROR:", MB_OK );
     return NULL;
    }


  hdcScreen = CreateDC("DISPLAY", (LPCSTR) NULL, (LPCSTR) NULL, (CONST DEVMODE *) NULL); 
  hDCMem = CreateCompatibleDC( hdcScreen ); 

  Picture ->get_Width(  &Width  );
  Picture ->get_Height( &Height );

  bm.bmWidth  = MAP_LOGHIM_TO_PIX( Width,  GetDeviceCaps(hdcScreen, LOGPIXELSX));
  bm.bmHeight = MAP_LOGHIM_TO_PIX( Height, GetDeviceCaps(hdcScreen, LOGPIXELSY));

  hBM = CreateCompatibleBitmap( hdcScreen, bm.bmWidth, bm.bmHeight );
  SelectObject( hDCMem, hBM );

  Picture ->Render(hDCMem, 0, 0, bm.bmWidth, bm.bmHeight,  0, Height, Width, -Height, NULL);

  DeleteDC(hDCMem);

  *JpgWidth  = bm.bmWidth;
  *JpgHeight = bm.bmHeight;

  return ((HBITMAP) hBM ); 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:33:57 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:33:57 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
CRYPT_ITEM *GetCryptItem( DWORD ID )
{
  CRYPT_ITEM *CryptItem = NULL;

  CryptItem = gCryptCDItemRoot;

  if(! CryptItem )
    {
     // No list ( or items ) yet...
     return NULL;
    }

  while( CryptItem )
  {

   if( ID == CryptItem ->ID )
      return CryptItem;

   CryptItem = CryptItem ->Next;
  }

  return NULL;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:34:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:34:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *LoadLayoutFileName(void)
{

 static OPENFILENAME ofn;
 static char Filter[255];
 static char TmpStr[1024];

 memset(Filter,0,255);

 strcpy(Filter, "CryptCD Layout Files (*.ccd)");
 strcpy(&Filter[strlen(Filter)+1],"*.ccd");
  
 memset(TmpStr,0,1024);

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = NULL; // hWnd;
 ofn.hInstance = ghInst;
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
 ofn.lpstrDefExt = "*";
 ofn.lCustData = 0L;
 ofn.lpfnHook = NULL;
 ofn.lpTemplateName = NULL;
 
 GetOpenFileName(&ofn);

 return ((char *) TmpStr);
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:35:26 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:35:26 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void EncryptLayoutFile( char *Filename, char *Password )
{
  FILE *fp = NULL;
  DWORD Len = 0, FileSize = 0;
  char *Buff = NULL;

  if( ! (fp = fopen(Filename, "rb+")))
    {
     // Just fail... and dont encrypt the file...
     OK_Msg( "Error: (65847)", "Cannot encrypt Layout file. Please try and save it again." );
     return;
    }

  fseek( fp, 0L, SEEK_END );
  FileSize = ftell( fp ); 
  fseek( fp, 0L,  SEEK_SET );

  if( ! (Buff = (char *) CustAllocateMem( FileSize + 100, "EncryptLayoutFile()", __LINE__ ) ))
    {
     // Just fail... and dont encrypt the file...
     OK_Msg( "Error: (65848)", "Cannot encrypt Layout file ( Allocation ). Please try and save it again." );
     return;
    }


  fread( Buff, 1, FileSize, fp ); // read in the whole file...

  // encrypt everything after the compare string ( eval_string... )
  Encrypt( Password,
           (BYTE *) &Buff[ sizeof( LAYOUT_HEADER ) ],
           FileSize - sizeof( LAYOUT_HEADER ) );

  // set back to top of file...
  fseek( fp, 0, SEEK_SET ); 

  // write out the whole file again...
  fwrite( Buff, 1, FileSize, fp );
  
  if( fp ) fclose( fp );

  CustFreeAllocatedMem( Buff,  "EncryptLayoutFile()", __LINE__ );

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:36:21 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:36:21 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *SaveLayoutFileName(void)
{

 static OPENFILENAME ofn;
 static char Filter[255];
 static char TmpStr[1024];

 memset(Filter,0,255);

 strcpy(Filter, "CryptCD Layout Files (*.ccd)");
 strcpy(&Filter[strlen(Filter)+1],"*.ccd");
  
 memset(TmpStr,0,1024);

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = NULL; // hWnd;
 ofn.hInstance = ghInst;
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
 ofn.lpstrDefExt = "*";
 ofn.lCustData = 0L;
 ofn.lpfnHook = NULL;
 ofn.lpTemplateName = NULL;
 
 GetSaveFileName(&ofn);

 return ((char *) TmpStr);
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 9:37:18 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 9:37:18 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void LoadLayout( void )
{
  TVITEM tvi;
  TVINSERTSTRUCT tvins;
  HTREEITEM NewItem, hParentItem;  
  DWORD Len = 0, NumItems = 0;
  FILE *in = NULL;
  char *p = NULL;
  char Password[ 255 ];
  CRYPT_ITEM *CryptItem = NULL;
  DWORD FileSize = 0, w = 0, h = 0;;
  DWORD WhereInFile = 0;
  char ItemName[ 255 ];
  char ItemPath[ 255 ];
  SHFILEINFO shfi;
  char *Buff = NULL;


  p = LoadLayoutFileName();


  if(! p[ 0 ] )
    {
     OK_Msg("INFO:", "Layout not loaded.");
     return;
    }

  
  if(! (in = fopen(p, "rb")))
    {
     OK_Msg("ERROR: (993453)", "Cannot open layout file. Please try again.");
     return;
    }


  // Clear linked list...
  ClearCDTreeList();  

  // Clear Treeview...
  TreeView_DeleteAllItems( ghCryptCDWnd );

  fseek( in,  0L, SEEK_END );
  FileSize = ftell( in );
  fseek( in,  0L, SEEK_SET );


  if(! (Buff = (char *) CustAllocateMem( FileSize + 100,  "LoadLayout()", __LINE__ ) ))
    {
     fclose( in );
     OK_Msg("ERROR: (993453)", "Cannot read layout file ( allocation ). Please try again.");
     return;
    }


  fread( Buff, 1, FileSize, in );

  if( in ) fclose( in );

  // Load in header ( layout file )
  WhereInFile = 0;
  memcpy( &gLayoutHeader, &Buff[ 0 ], sizeof(LAYOUT_HEADER ) );
  WhereInFile += sizeof( LAYOUT_HEADER );


  // Load in password ( if flag set )
  if( gLayoutHeader.Flags & FLAG_LAYOUTPASSWORD )
    {
     memset( Password, 0, 255 );
     memset( gLayoutPW, 0, 255 );

     DialogBox( ghInst, MAKEINTRESOURCE( IDD_LAYOUTPW_DLG ),
                        NULL, LayoutPwDlgProc);

     // Get password from user...
     Encrypt( gLayoutPW, (BYTE *) &Buff[WhereInFile], FileSize - WhereInFile );

     Len = strlen(  (char *) EVAL_STRING );
     memcpy( Password, &Buff[WhereInFile], Len );
     WhereInFile += Len;

     // check password...
     if( strcmp(Password, (char *) EVAL_STRING) )
       {
        CustFreeAllocatedMem( Buff,  "LoadLayout()", __LINE__ );
        OK_Msg("Error:","Password incorrect, please try again.\r\n\r\n( Make sure your CAPS LOCK key is not on )");
        return;
       }

    } // end if password...


  // Load in CFG info ( if flag set )
  if( gLayoutHeader.Flags & FLAG_LAYOUTSAVEALL )
    {

     // Read in CFG struct ( including var string EULA! )
     memcpy( &gCfg, &Buff[WhereInFile], (sizeof(CFG) - sizeof( char * )) );
     WhereInFile += (sizeof(CFG) - sizeof( char * ));

     // wipe out any bogus "junk" password that may have been saved
     memset( gCfg.Password, 0, 255 );
 
     memcpy( &Len, &Buff[WhereInFile], sizeof( DWORD )); 
     WhereInFile += sizeof( DWORD );



     if( Len )
       {
        if( gCfg.EulaText ) // if were loading over existing layout...
            CustFreeAllocatedMem( gCfg.EulaText,  "LoadLayout()", __LINE__ );

        if( p = (char *) CustAllocateMem( Len+10,  "LoadLayout()", __LINE__ ) )
          {
           memset( p, 0, Len+9 );

           memcpy( p, &Buff[WhereInFile], Len );

           gCfg.EulaText = p;

           WhereInFile += Len;
          }
         else // failed... just skipit...
          WhereInFile += Len;
    
       }
      else // if ( EULA not in there --- NULL cfg ptr )
       gCfg.EulaText = NULL;



      // Setup any decoy text/image things...
      if( gCfg.Options & OPT_BGIMAGE )
        { 
         if( toupper( gCfg.BgFileName[ strlen( gCfg.BgFileName ) - 1 ] ) == 'P' )
           { 
            BITMAP bm;

            gBgImage = (HBITMAP) LoadImage( NULL, gCfg.BgFileName, IMAGE_BITMAP,
                                         0, 0, LR_LOADFROMFILE );

            GetObject( gBgImage, sizeof(BITMAP),(LPVOID) &bm );
            w = bm.bmWidth;
            h = bm.bmHeight;
           } // Get bitmap info...
          else
           { 
            gBgImage = LoadJPG( gCfg.BgFileName, &w, &h );
           }

         gWidth  = w;
         gHeight = h;
        }
     

       // Load in any logo image...
       if( gCfg.Options & OPT_LOGOIMAGE )
         {
           if( toupper( gCfg.LogoFileName[ strlen( gCfg.LogoFileName ) - 1 ] ) == 'P' )
             { 
              BITMAP bm;

              gLogoImage = (HBITMAP) LoadImage( NULL, gCfg.LogoFileName, IMAGE_BITMAP,
                                                0, 0, LR_LOADFROMFILE );

              GetObject( gLogoImage, sizeof(BITMAP),(LPVOID) &bm );
              w = bm.bmWidth;
              h = bm.bmHeight;
             } // Get bitmap info...
            else
             { 
              gLogoImage = LoadJPG( gCfg.LogoFileName, &w, &h );
             }

           gLogoWidth = w;
           gLogoHeight = h;
         } 
                   
    } // end ALL cfg...


  // Load in and set all CryptItems...
  CRYPT_ITEM FileItem; // ONLY use 1st 6 members!
  NumItems = gLayoutHeader.NumItems;
  while( NumItems )
  {
   memset( ItemName, 0, 255 );

   // Read in item DWORD info...
   memcpy( &FileItem, &Buff[ WhereInFile ], sizeof( DWORD ) * 7);
   WhereInFile += ( sizeof( DWORD ) * 7);

   // Read in Length of Item name...
   memcpy( &Len, &Buff[WhereInFile], sizeof(DWORD ) );
   WhereInFile += sizeof( DWORD );

   if( Len > 254) Len = 254;

   memcpy( ItemName, &Buff[WhereInFile], Len );
   WhereInFile += Len;

   // Read in Item path Length...
   memcpy( &Len, &Buff[WhereInFile], sizeof(DWORD ) );
   WhereInFile += sizeof( DWORD );

   if( Len )
     {
      if( Len > 254) Len = 254;
      memset( ItemPath, 0, 255 ); 

      memcpy( ItemPath, &Buff[WhereInFile], Len );
      WhereInFile += Len;
     }
 
   // Read in Item Notes Length...
   memcpy( &Len, &Buff[WhereInFile], sizeof(DWORD ) );
   WhereInFile += sizeof( DWORD );
   char *ItemNotePtr = NULL;

   if( Len )
     {
      // allocate new mem for it and fill it up...
       if( ItemNotePtr = (char *) CustAllocateMem( Len+10,  "LoadLayout()", __LINE__ ) )
         {
          memset( ItemNotePtr, 0, Len + 5 ); 

          memcpy( ItemNotePtr, &Buff[WhereInFile], Len );
          WhereInFile += Len;
         }
     }


   // Read in Item suspect name Length...
   memcpy( &Len, &Buff[WhereInFile], sizeof(DWORD ) );
   WhereInFile += sizeof( DWORD );
   char *ItemSuspectPtr = NULL;

   if( Len )
     {
      // allocate new mem for it and fill it up...
       if( ItemSuspectPtr = (char *) CustAllocateMem( Len+10,  "LoadLayout()", __LINE__ ) )
         {
          memset( ItemSuspectPtr, 0, Len + 5 ); 

          memcpy( ItemSuspectPtr, &Buff[WhereInFile], Len );
          WhereInFile += Len;
         }
     }

 


   tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE |  TVIF_STATE | TVIF_PARAM;

   if( FileItem.InfoType == FLAG_TREEROOT )  
     {
      tvi.iImage = giLockIcon;
      tvi.iSelectedImage = giLockIcon;

      tvi.state     = TVIS_EXPANDED;
      tvi.stateMask = TVIS_EXPANDED;   
     }
   else if( FileItem.InfoType == ID_DRIVES_TREEVIEW )  
     {
      tvi.iImage = giKeyFolderClosed;
      tvi.iSelectedImage = giKeyFolderOpen;
      tvi.state     = 0; 
      tvi.stateMask = 0; 
     }
    else
     {
      memset(&shfi,0,sizeof(shfi)); 
      SHGetFileInfo(ItemName, FILE_ATTRIBUTE_NORMAL, 
                    &shfi, sizeof(shfi), 
                    SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES ); 

        int i = ImageList_AddIcon( gTreeCDImgs, shfi.hIcon );
         if( i == -1 ) 
           {

           // add a default icon image instead.
           if( ImageList_AddIcon( gTreeCDImgs, ghDunnoIcon ) == -1 )
             {
              OK_Msg("Error: 663533", "Cannot add treeview icon." ); 
              return;
             }

           }

      tvi.iImage = i; 
      tvi.iSelectedImage = i;  
     }

   tvi.pszText   = ItemName;
   tvi.cchTextMax = strlen( ItemName ) + 1;
   tvi.lParam = (LPARAM) AddCryptItem( FileItem.ParentID,
                             FileItem.InfoType,
                             FileItem.Size,
                             FileItem.Attributes,
                             FileItem.Flags, // ( enc type etc )
                             ItemName, ItemPath ); // item text ( name )... 


   if(!(CryptItem = (CRYPT_ITEM *) tvi.lParam))
        OK_Msg("Error: 100", "cannot get ptr to CryptItem."); 

   // Add the note if one ( WARNING: this MUST go AFTER AddCryptItem(), ItemNotes is NULLed in that function! )
   if( ItemNotePtr )
       CryptItem ->ItemNotes = ItemNotePtr;

   if( ItemSuspectPtr )
       CryptItem ->SuspectName = ItemSuspectPtr;

   CryptItem ->CaseNumber = FileItem.CaseNumber;


   if( CryptItem )
     {
      CryptItem ->ID = FileItem.ID; // re-set to file ID...

      // find parentID in CryptItem List...
      if( ! (hParentItem = GetCrypthItem( FileItem.ParentID )))
        {
         gCryptCDItemRoot = CryptItem;  
         hParentItem = TVI_ROOT;
        }

      tvins.item = tvi;
      tvins.hParent = hParentItem; 

      if( !(NewItem = (HTREEITEM) SendMessage(ghCryptCDWnd, TVM_INSERTITEM, 0, (LPARAM) (LPTVINSERTSTRUCT) &tvins)))
        {
           OK_Msg("Error: 100", "cannot add item...( load layout )"); 
         //return FALSE;
        }


      if( CryptItem ->Flags & FLAG_ENCRYPTED )
         TreeView_SetCheckState( ghCryptCDWnd, NewItem, TRUE );

      UpdateStatusBar();

      CryptItem ->hItem = NewItem;
     }

  
   NumItems--;
  }  

  CustFreeAllocatedMem( Buff,  "LoadLayout()", __LINE__ );

}

