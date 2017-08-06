
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:07:08 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:07:08 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Add a user to the CD/DVD...
/*

*/
BOOL CALLBACK AddUserDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       // grey out the date expire edit and the note...
                       EnableWindow( GetDlgItem( hDlg, IDC_DATETIMEPICKER1 ), FALSE );
                       EnableWindow( GetDlgItem( hDlg, IDC_NOTE1_EDT ), FALSE );

                       memset( gTmpPassword, 0, 255 );
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
         case IDC_EXPIREHELP_BTN:
                   OK_Msg( "Help", "This is to set a timeout/expire date for this user." ); 
                   break;

         case IDC_NOTEHELP_BTN:
                   OK_Msg( "Help", "This is to set a note for this user when they boot the CD/DVD." ); 
                   break;
  
         case IDC_SETPW_BTN:
                   ShowWindow( hDlg, SW_HIDE );
                   DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETUSERPW_DLG ),
                              NULL, SetUserPWDlgProc );
                   ShowWindow( hDlg, SW_SHOW );
                 
                   break;


         case IDC_STARTNOTE_CHK:

                    if( IsDlgButtonChecked( hDlg, IDC_STARTNOTE_CHK ) )
                        EnableWindow( GetDlgItem( hDlg, IDC_NOTE1_EDT ), TRUE );
                       else
                        EnableWindow( GetDlgItem( hDlg, IDC_NOTE1_EDT ), FALSE );

                   break;


         case IDC_EXPIRE_CHK:

                    if( IsDlgButtonChecked( hDlg, IDC_EXPIRE_CHK ) )
                        EnableWindow( GetDlgItem( hDlg, IDC_DATETIMEPICKER1 ), TRUE );
                       else
                        EnableWindow( GetDlgItem( hDlg, IDC_DATETIMEPICKER1 ), FALSE );

                   break;

         case IDOK:
                  {
                   USER NewUser;
                   char TmpStr[ 255 ];

#ifndef DEMO_VERSION

                   if( ! IsRegistered1( gVersion ) )
                         break;

#endif

                   memset( &NewUser, 0, sizeof( USER ) );

                   NewUser.UserName = NULL;
                   NewUser.Password = NULL;
                   NewUser.UserNote = NULL;

                   // make sure a user password has been entered...
                   if( ! gTmpPassword[ 0 ] )
                     {
                      OK_Msg("INFO:", "Please enter a password for this user."); 
                      break;
                     }

                   // Get name
                   memset( TmpStr, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_NAME_EDT ), TmpStr, GetWindowTextLength( GetDlgItem( hDlg, IDC_NAME_EDT ))+1);
                   if( ! TmpStr[ 0 ] )
                     {
                      OK_Msg("INFO:", "Please enter a name for this user."); 
                      break;
                     }

                  // add name...
                  int Len = strlen( TmpStr );
                  char *p = NULL;
                
                  if( Len )
                    {
 
                     if( ( p = (char *) CustAllocateMem( Len + 10,  "AddUserDlgProc()", __LINE__ ) ) )
                       {   
                        strcpy( p, TmpStr );
                        NewUser.UserName = p;
                       }
                
  
                    }
 
    
                   // add the password...
                  Len = strlen( gTmpPassword );
                  p = NULL;

                  if( Len )
                    {
    
                     if( ( p = (char *) CustAllocateMem( Len + 10,  "AddUserDlgProc()", __LINE__ ) ) )
                       {   
                        strcpy( p, gTmpPassword );
                        NewUser.Password = p;
                       }
                      
  
                    }
 
  
                   // get checkbox states ( for flags etc. )...
                   if( SendMessage( GetDlgItem( hDlg, IDC_EXPIRE_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_TIMEOUT;
                    else
                     NewUser.UserFlags &= ~OPT_USER_TIMEOUT;

                   if( SendMessage( GetDlgItem( hDlg, IDC_STARTNOTE_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_NOTE;
                    else
                     {
                      NewUser.UserFlags &= ~OPT_USER_NOTE;
                     } 

                   if( SendMessage( GetDlgItem( hDlg, IDC_OPEN_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_OPEN;
                    else
                     NewUser.UserFlags &= ~OPT_USER_OPEN;

                   if( SendMessage( GetDlgItem( hDlg,IDC_EXPORT_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_EXPORT;
                    else
                     NewUser.UserFlags &= ~OPT_USER_EXPORT;

                   if( SendMessage( GetDlgItem( hDlg,IDC_HIDE_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_HIDEALL;
                    else
                     NewUser.UserFlags &= ~OPT_USER_HIDEALL;

                   if( SendMessage( GetDlgItem( hDlg,IDC_NOTE_CHK ), BM_GETCHECK, 0, 0 ) )
                        NewUser.UserFlags |= OPT_USER_VIEWNOTES;
                    else
                     NewUser.UserFlags &= ~OPT_USER_VIEWNOTES;




                   // get expire date?
                   if( NewUser.UserFlags & OPT_USER_TIMEOUT ) // if expire date set...
                      DateTime_GetSystemtime(GetDlgItem( hDlg, IDC_DATETIMEPICKER1 ), &NewUser.ExpireDate );

                   // get user note ( if one )...
                   if( NewUser.UserFlags & OPT_USER_NOTE )
                     {
                      int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_NOTE1_EDT ) ) + 1;

                      if( Len )
                        {
                         // allocate this notes mem...
                       
                         if(! ( NewUser.UserNote = (char *) CustAllocateMem( Len + 100,  "AddUserDlgProc()", __LINE__ ) ) )
                           { 
                            NewUser.UserFlags &= ~OPT_USER_NOTE;
                            OK_Msg("Error:", "Could not allocate user note.");
                            break;
                           }

                        memset( NewUser.UserNote, 0, Len + 50 );

                        GetWindowText( GetDlgItem( hDlg, IDC_NOTE1_EDT ), NewUser.UserNote, Len );
                       }

                     } 
                     


                   if( ! ( AddUser( ( USER *) &NewUser ) ) )
                     {
                      OK_Msg("ERROR:", "Could not add user."); 
                      break;
                     }


                   EndDialog(hDlg, TRUE);
                  }
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



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:08:28 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:08:28 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Edit CD/DVD user...
BOOL CALLBACK EditUserDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                       // Fill in all the current user info...     
                       memset(  gTmpPassword, 0, 255 );

                       if(! gTmpEditUser )
                         {
                          OK_Msg( "Error:", "User not found." );
                          break;
                         }

                       SetWindowText( GetDlgItem( hDlg, IDC_NAME_EDT ), gTmpEditUser ->UserName );
                       
                       // check all check boxes and grey windows that are not checked...
                       if( gTmpEditUser ->UserFlags & OPT_USER_NOTE )
                         {
                           EnableWindow( GetDlgItem( hDlg, IDC_NOTE_EDT ), TRUE );
                           SendDlgItemMessage( hDlg, IDC_NOTE2_CHK, BM_SETCHECK, BST_CHECKED, 0 );
                           SetWindowText( GetDlgItem( hDlg, IDC_NOTE_EDT ), gTmpEditUser ->UserNote );
                         }
                        else
                         {
                           EnableWindow( GetDlgItem( hDlg, IDC_NOTE_EDT ), FALSE );
                           SendDlgItemMessage( hDlg, IDC_NOTE2_CHK, BM_SETCHECK, BST_UNCHECKED, 0 );
                         }



                       if( gTmpEditUser ->UserFlags & OPT_USER_TIMEOUT )
                         {
                           EnableWindow( GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), TRUE );
                           SendDlgItemMessage( hDlg, IDC_EXP_CHK, BM_SETCHECK, BST_CHECKED, 0 );
                           DateTime_SetSystemtime(GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), GDT_VALID , &(gTmpEditUser ->ExpireDate));
                         }
                        else
                         {
                           EnableWindow( GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), FALSE );
                           SendDlgItemMessage( hDlg, IDC_EXP_CHK, BM_SETCHECK, BST_UNCHECKED, 0 );
                         }



                       if( gTmpEditUser ->UserFlags & OPT_USER_OPEN )                       
                           SendDlgItemMessage( hDlg, IDC_OPEN_CHK, BM_SETCHECK, BST_CHECKED, 0 );
           
                       if( gTmpEditUser ->UserFlags & OPT_USER_HIDEALL )                       
                           SendDlgItemMessage( hDlg, IDC_HIDE_CHK, BM_SETCHECK, BST_CHECKED, 0 );
 
                       if( gTmpEditUser ->UserFlags & OPT_USER_VIEWNOTES )                       
                           SendDlgItemMessage( hDlg, IDC_NOTE_CHK, BM_SETCHECK, BST_CHECKED, 0 );
 
                       if( gTmpEditUser ->UserFlags & OPT_USER_EXPORT )                       
                           SendDlgItemMessage( hDlg, IDC_EXPORT_CHK, BM_SETCHECK, BST_CHECKED, 0 );
 
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
         case IDC_EDITEXPIREHELP_BTN:
                   OK_Msg( "Help", "This is to set a timeout/expire date for this user." ); 
                   break;

         case IDC_EDITNOTEHELP_BTN:
                   OK_Msg( "Help", "This is to set a note for this user when they boot the CD/DVD." ); 
                   break;

         case IDC_NOTE2_CHK:

                    if( IsDlgButtonChecked( hDlg, IDC_NOTE2_CHK ) )
                        EnableWindow( GetDlgItem( hDlg, IDC_NOTE_EDT ), TRUE );
                       else
                        EnableWindow( GetDlgItem( hDlg, IDC_NOTE_EDT ), FALSE );

                   break;


         case IDC_EXP_CHK:

                    if( IsDlgButtonChecked( hDlg, IDC_EXP_CHK ) )
                        EnableWindow( GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), TRUE );
                       else
                        EnableWindow( GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), FALSE );

                   break;


         case IDC_CHANGEPW_BTN:
                   ShowWindow( hDlg, SW_HIDE );
                   DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETUSERPW_DLG ),
                              NULL, SetUserPWDlgProc );
                   ShowWindow( hDlg, SW_SHOW );
                 
                   break;

         case IDC_KILLUSER_BTN:
                   {
                    // are you sure?
                    if( YesNoCancel_Msg( "Delete User", "Are you sure?" ) != IDYES )
                        break;

                    // Remove this user...
                    RemoveUser( gTmpEditUser );
                    
                    gTmpEditUser = NULL; // reset...

                    // close this dialog...
                    EndDialog(hDlg, TRUE);
                   }
                   break;

         case IDOK:  
                  { 
                   // Get name
                   char TmpStr[ 255 ];

                   memset( TmpStr, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_NAME_EDT ), TmpStr, GetWindowTextLength( GetDlgItem( hDlg, IDC_NAME_EDT ))+1);
                   if( ! TmpStr[ 0 ] )
                     {
                      OK_Msg("INFO:", "Please enter a name for this user."); 
                      break;
                     }
                    else
                     { 
                      // add name...
                      int Len = strlen( TmpStr );
                      char *p = NULL;
                
                      if( Len )
                        {

                         if( gTmpEditUser ->UserName )
                             CustFreeAllocatedMem( gTmpEditUser ->UserName, "EditUserDlgProc()", __LINE__ );

                         if( ( p = (char *) CustAllocateMem( Len + 10,  "AddUserDlgProc()", __LINE__ ) ) )
                           {    
                            strcpy( p, TmpStr );
                            gTmpEditUser ->UserName = p;
                           }
                          
  
                        }

                      } // end else...


            

                   // If the password is different get it...
                   if( gTmpPassword[0] ) // if they changed it this will be set...
                     {

                      if( (strcmp( gTmpEditUser ->Password,  gTmpPassword ) ) )
                        {
                         // free old PW and allocate and copy in the new one...
                         if( gTmpEditUser ->Password )
                             CustFreeAllocatedMem( gTmpEditUser ->Password, "EditUserDlgProc()", __LINE__ );


                         int Len = strlen( gTmpPassword );
                         char *p = NULL;

                         if( Len )
                           {
    
                            if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                              {    
                               strcpy( p, gTmpPassword );
                               gTmpEditUser ->Password = p;
                              }
                             
  
                           }
                        }
                     } // end if( pw[0] )


                   // If note is unchecked, kill and free it
                   if(!( SendMessage( GetDlgItem( hDlg, IDC_NOTE2_CHK ), BM_GETCHECK, 0, 0 ) ) )
                     {
                      // note is un-checked... remove it if it existed previously...
                      if( gTmpEditUser ->UserNote )
                        {
                          CustFreeAllocatedMem( gTmpEditUser ->UserNote, "EditUserDlgProc()", __LINE__ );
                          gTmpEditUser ->UserNote = NULL;
                        }

                      gTmpEditUser ->UserFlags &= ~OPT_USER_NOTE;
                     }
                    else
                     {
                      // note is checked... instead of checking it all again it's faster just
                      // to remove it and re-allocate it again.
                      int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_NOTE_EDT ) );

                      CustFreeAllocatedMem( gTmpEditUser ->UserNote, "EditUserDlgProc()", __LINE__ );
                      gTmpEditUser ->UserNote = NULL;

                      if( Len )
                        {
                         char *p = NULL;

                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_NOTE_EDT ), p, Len + 1 );       
                            gTmpEditUser ->UserNote = p;
                            gTmpEditUser ->UserFlags |= OPT_USER_NOTE;
                           }
                          

                        }
                       else  // nothing in the box... remove it...
                        gTmpEditUser ->UserFlags &= ~OPT_USER_NOTE;


                      } // end else...



                   // Reset any flags... ( just overwrite um with whats in there now ).
                   if( SendMessage( GetDlgItem( hDlg, IDC_EXP_CHK ), BM_GETCHECK, 0, 0 ) )
                     { 
                      gTmpEditUser ->UserFlags |= OPT_USER_TIMEOUT;
                      DateTime_GetSystemtime(GetDlgItem( hDlg, IDC_EDITDATETIMEPICKER ), &(gTmpEditUser ->ExpireDate) );
                     } 
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_TIMEOUT;

                   if( SendMessage( GetDlgItem( hDlg, IDC_NOTE2_CHK ), BM_GETCHECK, 0, 0 ) )
                      gTmpEditUser ->UserFlags |= OPT_USER_NOTE;
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_NOTE;
                      
                   if( SendMessage( GetDlgItem( hDlg, IDC_OPEN_CHK ), BM_GETCHECK, 0, 0 ) )
                      gTmpEditUser ->UserFlags |= OPT_USER_OPEN;
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_OPEN;

                   if( SendMessage( GetDlgItem( hDlg,IDC_EXPORT_CHK ), BM_GETCHECK, 0, 0 ) )
                      gTmpEditUser ->UserFlags |= OPT_USER_EXPORT;
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_EXPORT;

                   if( SendMessage( GetDlgItem( hDlg,IDC_HIDE_CHK ), BM_GETCHECK, 0, 0 ) )
                      gTmpEditUser ->UserFlags |= OPT_USER_HIDEALL;
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_HIDEALL;

                   if( SendMessage( GetDlgItem( hDlg,IDC_NOTE_CHK ), BM_GETCHECK, 0, 0 ) )
                      gTmpEditUser ->UserFlags |= OPT_USER_VIEWNOTES;
                     else
                      gTmpEditUser ->UserFlags &= ~OPT_USER_VIEWNOTES;


 

                   gTmpEditUser = NULL; // reset...
                   EndDialog(hDlg, TRUE);
                  }  
                  break;
 
 
         // changed their mind...
         case IDCANCEL:
                   gTmpEditUser = NULL; // reset...
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
 * Date:          12/17/2014  ( 10:09:17 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:09:17 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK CustAboutBoxDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {
                        // NOTE: to get here the about box flag has to be set already.

                        // Fill in the 3 text boxes if that information exists...
                        if( gCfg.AboutBoxText1[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TXT1_EDIT ), gCfg.AboutBoxText1 );
      
                        if( gCfg.AboutBoxText2[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TXT2_EDIT ), gCfg.AboutBoxText2 );
  
                        if( gCfg.AboutBoxText3[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TXT3_EDIT ), gCfg.AboutBoxText3 );
  
                       }
                       return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
                   EndDialog(hDlg, TRUE);
                   break;

         case IDC_ADDIMAGE_BTN:
                   {
                    DWORD w, h;

                    strcpy( gCustAboutImageFile, GetImageFileName() );
                    if( ! (gCustAboutImageFile[0])) 
                        break; 

                    // Get the image ( if they actually choose one...
                    if( gCustAboutImageBmp ) // in case they try and change it...
                       DeleteObject( gCustAboutImageBmp );

                    gCfg.Options |= OPT_CUSTABOUTBOXIMG;

                    if( toupper( gCustAboutImageFile[ strlen( gCustAboutImageFile ) - 1 ] ) == 'P' )
                      { 
                       BITMAP bm;

                       gCustAboutImageBmp = (HBITMAP) LoadImage( NULL, gCustAboutImageFile, IMAGE_BITMAP,
                                                      0, 0, LR_LOADFROMFILE );

                       GetObject( gCustAboutImageBmp, sizeof(BITMAP),(LPVOID) &bm );
                       w = bm.bmWidth;
                       h = bm.bmHeight;
                      } // Get bitmap info...
                     else
                      { 
                       gCustAboutImageBmp = LoadJPG( gCustAboutImageFile, &w, &h );
                      }              

                    strcpy( gCfg.AboutImageFileName, gCustAboutImageFile );                  
                   }
                   break;

         case IDC_PREVIEW_BTN:

                   memset( gCfg.AboutBoxText1, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT1_EDIT ), gCfg.AboutBoxText1, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT1_EDIT ))+1);

                   memset( gCfg.AboutBoxText2, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT2_EDIT ), gCfg.AboutBoxText2, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT2_EDIT ))+1);

                   memset( gCfg.AboutBoxText3, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT3_EDIT ), gCfg.AboutBoxText3, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT3_EDIT ))+1);


                   // Show how the dialog will look...
                   ShowWindow( hDlg, SW_HIDE );
                   DialogBox( ghInst, MAKEINTRESOURCE( IDD_ABOUTPREVIEW_DLG ),
                               NULL,  AboutPreviewDlgProc);
                   ShowWindow( hDlg, SW_SHOW );               
                   break;


         case IDOK:
                   memset( gCfg.AboutBoxText1, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT1_EDIT ), gCfg.AboutBoxText1, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT1_EDIT ))+1);

                   memset( gCfg.AboutBoxText2, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT2_EDIT ), gCfg.AboutBoxText2, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT2_EDIT ))+1);

                   memset( gCfg.AboutBoxText3, 0, 255 );
                   GetWindowText( GetDlgItem( hDlg, IDC_TXT3_EDIT ), gCfg.AboutBoxText3, GetWindowTextLength( GetDlgItem( hDlg, IDC_TXT3_EDIT ))+1);

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
 * Date:          12/17/2014  ( 10:09:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:09:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK AboutPreviewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {

                        if( gCfg.AboutBoxText1[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TEXT1_STATIC ), gCfg.AboutBoxText1 );
      
                        if( gCfg.AboutBoxText2[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TEXT2_STATIC ), gCfg.AboutBoxText2 );

                        if( gCfg.AboutBoxText3[ 0 ] )
                            SetWindowText( GetDlgItem( hDlg, IDC_TEXT3_STATIC ), gCfg.AboutBoxText3 );


                        SendMessage( hDlg, WM_SIZE, 0,0 );   
                      }
                       return TRUE;



    case WM_SIZE:
                 {
                  // if they choose a pic... adjust the size of the dialog accordingly...
                   if( gCustAboutImageBmp )
                     {
                      // Get the bitmaps size, so the dialog box can be sized...
                      BITMAP bm;

                      GetObject( gCustAboutImageBmp, sizeof(BITMAP),(LPVOID) &bm );

                      DWORD xPos = (GetSystemMetrics( SM_CXSCREEN )/2) - ((bm.bmWidth + 15)/2);
                      DWORD yPos = (GetSystemMetrics( SM_CYSCREEN )/2) - ((bm.bmHeight + 130)/2);

                      MoveWindow( hDlg, xPos, yPos, bm.bmWidth + 15, bm.bmHeight + 130, TRUE );

                      MoveWindow( GetDlgItem( hDlg, IDC_TEXT1_STATIC ), 5, bm.bmHeight + 10, bm.bmWidth + 50, 16, TRUE );
                      MoveWindow( GetDlgItem( hDlg, IDC_TEXT2_STATIC ), 5, bm.bmHeight + 26, bm.bmWidth + 50, 16, TRUE );
                      MoveWindow( GetDlgItem( hDlg, IDC_TEXT3_STATIC ), 5, bm.bmHeight + 42, bm.bmWidth + 50, 16, TRUE );

                      MoveWindow( GetDlgItem( hDlg, IDOK ), bm.bmWidth-40, (bm.bmHeight + 94) - 30, 40, 25, TRUE );
                     }

                 }
                 break;

    case WM_PAINT:
                  {
                   PAINTSTRUCT ps;
                   HDC hDC = BeginPaint( hDlg, &ps );

                   // Draw in the pic... if they choose one...
                   if( gCustAboutImageBmp )
                       DrawBitmap( hDC, gCustAboutImageBmp, 5, 5 );
                      else
                       {
                        SetTextColor( hDC, RGB( 0, 0, 100 ) );
                        SetBkMode( hDC, TRANSPARENT );
                        TextOut( hDC, 150, 20, "No picture selected.", 20 );
                       }

                   EndPaint( hDlg, &ps );
                  }
                  break;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
         case IDOK:
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
 * Date:          12/17/2014  ( 10:11:09 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:11:09 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK SetPWDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

                  if( gCfg.Password[ 0 ] ) // if there's a password...
                    {
                     if(IDNO == YesNo_Msg("Are you sure?", "Are you sure you want to change your password?" ))
                        break;
                    }

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


                  if( strlen( Password ) < 8 )
                    { 
                     OK_Msg("Info:", "Please enter a password with at least 8 characters");
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
                  
                  strcpy( gCfg.Password, Password );

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
 * Date:          12/17/2014  ( 10:12:02 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:12:02 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {
                        char TmpStr[ 255 ];

                        sprintf( TmpStr, "Build: %s %s", NowTime(), __DATE__ ); 
                        SetWindowText( GetDlgItem( hDlg, IDC_VERSION_STATIC ), TmpStr );
 
                       }
                       return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
         case IDOK:
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
 * Date:          12/17/2014  ( 10:12:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:12:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK SetTextDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static char TmpStr[ 255 ];

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 


                     if( gCfg.MoreOptions & OPT_TEXTTOPLEFT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }
                else if( gCfg.MoreOptions & OPT_TEXTTOPCENTER )  
                       { // Center of screen
                        CheckDlgButton( hDlg, IDC_RADIO2, BST_CHECKED );
                       }
                else if( gCfg.MoreOptions & OPT_TEXTBOTTOMLEFT )  
                       { // Bottom left of screen
                        CheckDlgButton( hDlg, IDC_RADIO3, BST_CHECKED );
                       }
                else if( gCfg.MoreOptions & OPT_TEXTBOTTOMCENTER )  
                       { // Top  center of screen
                        CheckDlgButton( hDlg, IDC_RADIO4, BST_CHECKED );
                       }
                else if( gCfg.MoreOptions & OPT_TEXTCENTER )  
                       { // Bottom center of screen
                        CheckDlgButton( hDlg, IDC_RADIO5, BST_CHECKED );
                       }
                      else  // just use default...
                       {
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }



                     if( gCfg.MoreOptions & OPT_TEXTCUSTOM )
                       {
                        CheckDlgButton( hDlg, IDC_RADIO6, BST_CHECKED );
                        EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), TRUE );
                        EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), TRUE );

                        sprintf( TmpStr, "%ld", gCfg.TextXPos);
                        SetWindowText( GetDlgItem( hDlg, IDC_XPOS_EDT), TmpStr );

                        sprintf( TmpStr, "%ld", gCfg.TextYPos );
                        SetWindowText( GetDlgItem( hDlg, IDC_YPOS_EDT), TmpStr );
                       }
                      else
                       {
                        EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), FALSE );
                        EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), FALSE );
                       }



#if !defined(HOME_VER)
                sprintf( TmpStr, "%ld", gCfg.FontSize );
                SetWindowText( GetDlgItem( hDlg, IDC_TEXTSIZE_EDT ), TmpStr );
#endif
                SetWindowText( GetDlgItem( hDlg, IDC_TEXT_EDT ), gCfg.DisplayText );

                if( gCfg.Options & OPT_DROPSHADOW )
                  {
                   CheckDlgButton( hDlg, IDC_DROPSHADOW_CHK, TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_XOFFSET_EDT), TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YOFFSET_EDT), TRUE );

                   sprintf( TmpStr, "%ld", gCfg.FontShadowXOffset);
                   SetWindowText( GetDlgItem( hDlg, IDC_XOFFSET_EDT), TmpStr );

                   sprintf( TmpStr, "%ld", gCfg.FontShadowYOffset );
                   SetWindowText( GetDlgItem( hDlg, IDC_YOFFSET_EDT), TmpStr );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XOFFSET_EDT), FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YOFFSET_EDT), FALSE );
                  }

           return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_SETFONT:
                          gCfg.MoreOptions |= OPT_CUSTOMFONT;
                          gCfg.hFont = GetFont( hDlg );
                          break;


         case IDC_RADIO1:
         case IDC_RADIO2:
         case IDC_RADIO3:
         case IDC_RADIO4:
         case IDC_RADIO5:
                   EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), FALSE );
                         break;

         case IDC_RADIO6:

                if( IsDlgButtonChecked( hDlg, IDC_RADIO6 ) )
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), TRUE );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), FALSE );
                  }
                  break;

         case IDC_DROPSHADOW_CHK:

                if( IsDlgButtonChecked( hDlg, IDC_DROPSHADOW_CHK ) )
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XOFFSET_EDT), TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YOFFSET_EDT), TRUE );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XOFFSET_EDT), FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YOFFSET_EDT), FALSE );
                  }

               break;

#if !defined(HOME_VER)

         case IDC_TEXTCOLOR_BTN:
                  {
                   gCfg.TextColor = GetColor();
                  }
               break;
#endif

         case IDC_SHADOWCOLOR_BTN:
                  {
                   gCfg.ShadowColor = GetColor();
                  }
              break; 

         case IDOK:
                  {
                   // Get options...
                DWORD w = 0, h = 0, Flags = 0;

                // Get image placeement/action... ( from radio buttons... )
                
                // Clear the old Image placement flags...
                Flags = OPT_TEXTTOPLEFT | OPT_TEXTTOPCENTER |
                        OPT_TEXTBOTTOMLEFT | OPT_TEXTBOTTOMCENTER |
                        OPT_TEXTCENTER;

                gCfg.MoreOptions &= ~Flags;


                     if( IsDlgButtonChecked( hDlg, IDC_RADIO1 ) )  
                       { // Top left of screen
                        gCfg.MoreOptions |= OPT_TEXTTOPLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO2 ) )  
                       { // Top center of screen
                        gCfg.MoreOptions |= OPT_TEXTTOPCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO3 ) )  
                       { // Bottom left of screen
                        gCfg.MoreOptions |= OPT_TEXTBOTTOMLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO4 ) )  
                       { // Bottom Center of screen
                        gCfg.MoreOptions |= OPT_TEXTBOTTOMCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO5 ) )  
                       { // center of screen
                        gCfg.MoreOptions |= OPT_TEXTCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO6 ) )  
                       { // center of screen
                        gCfg.MoreOptions |= OPT_TEXTCUSTOM;

                        GetWindowText(GetDlgItem(hDlg, IDC_XPOS_EDT), TmpStr,
                                      GetWindowTextLength(GetDlgItem(hDlg,IDC_XPOS_EDT) ) + 1);

                        gCfg.TextXPos = (DWORD) atoi( TmpStr );

                        GetWindowText(GetDlgItem(hDlg, IDC_YPOS_EDT), TmpStr,
                                      GetWindowTextLength(GetDlgItem(hDlg,IDC_YPOS_EDT) ) + 1);

                        gCfg.TextYPos = (DWORD) atoi( TmpStr );
                       }
                      else  // just use default...
                       {
                        gCfg.MoreOptions |= OPT_TEXTTOPLEFT;
                       }



                   GetWindowText(GetDlgItem(hDlg, IDC_TEXT_EDT ), gCfg.DisplayText,
                                GetWindowTextLength(GetDlgItem(hDlg, IDC_TEXT_EDT ) ) + 1);

                  if( gCfg.DisplayText[ 0 ] )
                    {
                      if(gCfg.DisplayText[strlen(gCfg.DisplayText) - 1] != '\n')
                         strcat( gCfg.DisplayText, "\r\n" );

                      gCfg.Options |= OPT_USINGTEXT;
                    }
                  else
                      gCfg.Options &= ~OPT_USINGTEXT;


                if( IsDlgButtonChecked( hDlg, IDC_DROPSHADOW_CHK ) )
                  {
                   gCfg.Options |= OPT_DROPSHADOW; 

                   GetWindowText(GetDlgItem(hDlg, IDC_XOFFSET_EDT), TmpStr,
                                GetWindowTextLength(GetDlgItem(hDlg,IDC_XOFFSET_EDT) ) + 1);

                   gCfg.FontShadowXOffset = (DWORD) atoi( TmpStr );

                   GetWindowText(GetDlgItem(hDlg, IDC_YOFFSET_EDT), TmpStr,
                                GetWindowTextLength(GetDlgItem(hDlg,IDC_YOFFSET_EDT) ) + 1);

                   gCfg.FontShadowYOffset = (DWORD) atoi( TmpStr );
                  }
                 else
                  gCfg.Options &= ~OPT_DROPSHADOW; 

                  gRefresh = TRUE;  // windows OS bug fix...

                  EndDialog(hDlg, TRUE);
                  } // end ID_OK...
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
 * Date:          12/17/2014  ( 10:13:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:13:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// new
BOOL CALLBACK SetLogoImageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static char Filename[ 255 ], TmpStr[ 255 ];

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                        memset( Filename, 0, 255 );

                     if( gCfg.Options & OPT_LOGOTOPLEFT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_LOGOTOPCENTER )  
                       { // Center of screen
                        CheckDlgButton( hDlg, IDC_RADIO2, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_LOGOBOTTOMLEFT )  
                       { // Bottom left of screen
                        CheckDlgButton( hDlg, IDC_RADIO3, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_LOGOBOTTOMCENTER )  
                       { // Top  center of screen
                        CheckDlgButton( hDlg, IDC_RADIO4, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_LOGOCENTER )  
                       { // Bottom center of screen
                        CheckDlgButton( hDlg, IDC_RADIO5, BST_CHECKED );
                       }
                      else  // just use default...
                       {
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }

                     if( gCfg.Options & OPT_LOGOCUSTOM )
                       {
                        CheckDlgButton( hDlg, IDC_RADIO6, BST_CHECKED );
                        EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), TRUE );
                        EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), TRUE );

                        sprintf( TmpStr, "%ld", gCfg.LogoXPos);
                        SetWindowText( GetDlgItem( hDlg, IDC_XPOS_EDT), TmpStr );

                        sprintf( TmpStr, "%ld", gCfg.LogoYPos );
                        SetWindowText( GetDlgItem( hDlg, IDC_YPOS_EDT), TmpStr );
                       }
                      else
                       {
                        EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), FALSE );
                        EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), FALSE );
                       }

                      
                      if( gCfg.Options & OPT_LOGOTRANSPARENT )
                        {
                         CheckDlgButton( hDlg, IDC_TRANS_CHK, TRUE );
                         EnableWindow( GetDlgItem( hDlg, IDC_CHOOSETRANS_BTN), TRUE );
                        }
                       else
                        {
                         EnableWindow( GetDlgItem( hDlg, IDC_CHOOSETRANS_BTN), FALSE );
                        }


           return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_RADIO6:

                if( IsDlgButtonChecked( hDlg, IDC_RADIO6 ) )
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), TRUE );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_XPOS_EDT), FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_YPOS_EDT), FALSE );
                  }

               break;

         case IDC_TRANS_CHK:

                if( IsDlgButtonChecked( hDlg, IDC_TRANS_CHK ) )
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_CHOOSETRANS_BTN), TRUE );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_CHOOSETRANS_BTN), FALSE );
                  }

               break;

         case IDC_CHOOSETRANS_BTN:
                  {
                   gCfg.LogoTransColor = GetColor();
                  }
              break;


         case IDC_REMOVELOGO_BTN:

                   if( gLogoImage ) // in case they try and change it...
                       DeleteObject( gLogoImage );

                   memset( gCfg.LogoFileName, 0, 255 ); 

                   gCfg.Options &= ~OPT_LOGOIMAGE;

                   gRefresh = TRUE;  // windows OS bug fix...

              break;


         case IDC_LOADLOGO_BTN:
                      strcpy( Filename, GetImageFileName() );
                      break;

         case IDOK:
               {
                DWORD w = 0, h = 0, Flags = 0;

                // Get image placement/action... ( from radio buttons... )
                
                // Clear the old Image placement flags...
                Flags = OPT_LOGOTOPLEFT | OPT_LOGOTOPCENTER |
                        OPT_LOGOBOTTOMLEFT | OPT_LOGOBOTTOMCENTER |
                        OPT_LOGOCENTER;

                gCfg.Options &= ~Flags;


                     if( IsDlgButtonChecked( hDlg, IDC_RADIO1 ) )  
                       { // Top left of screen
                        gCfg.Options |= OPT_LOGOTOPLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO2 ) )  
                       { // Top center of screen
                        gCfg.Options |= OPT_LOGOTOPCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO3 ) )  
                       { // Bottom left of screen
                        gCfg.Options |= OPT_LOGOBOTTOMLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO4 ) )  
                       { // Bottom Center of screen
                        gCfg.Options |= OPT_LOGOBOTTOMCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO5 ) )  
                       { // center of screen
                        gCfg.Options |= OPT_LOGOCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO6 ) )  
                       { // center of screen
                        gCfg.Options |= OPT_LOGOCUSTOM;

                        GetWindowText(GetDlgItem(hDlg, IDC_XPOS_EDT), TmpStr,
                                      GetWindowTextLength(GetDlgItem(hDlg,IDC_XPOS_EDT) ) + 1);

                        gCfg.LogoXPos = (DWORD) atoi( TmpStr );

                        GetWindowText(GetDlgItem(hDlg, IDC_YPOS_EDT), TmpStr,
                                      GetWindowTextLength(GetDlgItem(hDlg,IDC_YPOS_EDT) ) + 1);

                        gCfg.LogoYPos = (DWORD) atoi( TmpStr );
                       }
                      else  // just use default...
                       {
                        gCfg.Options |= OPT_LOGOTOPLEFT;
                       }
                      

                     if( IsDlgButtonChecked( hDlg, IDC_TRANS_CHK ) )
                         gCfg.Options |= OPT_LOGOTRANSPARENT; 
                        else
                         gCfg.Options &= ~OPT_LOGOTRANSPARENT; 



                if( strlen( Filename ) > 2 ) // did they change the image?
                  {
                   

                   if( gLogoImage ) // in case they try and change it...
                       DeleteObject( gLogoImage );

                   if( toupper( Filename[ strlen( Filename ) - 1 ] ) == 'P' )
                     { 
                      BITMAP bm;

                      gLogoImage = (HBITMAP) LoadImage( NULL, Filename, IMAGE_BITMAP,
                                                      0, 0, LR_LOADFROMFILE );

                      GetObject( gLogoImage, sizeof(BITMAP),(LPVOID) &bm );
                      w = bm.bmWidth;
                      h = bm.bmHeight;
                     } // Get bitmap info...
                    else
                     { 
                      gLogoImage = LoadJPG( Filename, &w, &h );
                     }

                    gLogoWidth = w;
                    gLogoHeight = h;

                    gCfg.Options |= OPT_LOGOIMAGE;

                    strcpy( gCfg.LogoFileName, Filename );

                  } // end if filename > 2

                  gRefresh = TRUE;  // windows OS bug fix...

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
 * Date:          12/17/2014  ( 10:14:43 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:14:43 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK SetBgImageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static char Filename[ 255 ];

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                        memset( Filename, 0, 255 );

                     if( gCfg.Options & OPT_BGSTRETCH )  
                       { // Stretch to screen size
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGTILE )  
                       { // Tile background image
                         CheckDlgButton( hDlg, IDC_RADIO2, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BG4TILE )  
                       { // Tile background image
                         CheckDlgButton( hDlg, IDC_RADIO8, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGTOPLEFT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_RADIO3, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGCENTER )  
                       { // Center of screen
                        CheckDlgButton( hDlg, IDC_RADIO4, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGBOTTOMLEFT )  
                       { // Bottom left of screen
                        CheckDlgButton( hDlg, IDC_RADIO5, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGTOPCENTER )  
                       { // Top  center of screen
                        CheckDlgButton( hDlg, IDC_RADIO6, BST_CHECKED );
                       }
                else if( gCfg.Options & OPT_BGBOTTOMCENTER )  
                       { // Bottom center of screen
                        CheckDlgButton( hDlg, IDC_RADIO7, BST_CHECKED );
                       }
                      else  // just use default...
                       {
                        CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
                       }
           return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_REMOVEIMAGE_BTN:

                   if( gBgImage ) // in case they try and change it...
                       DeleteObject( gBgImage );

                   memset( gCfg.BgFileName, 0, 255 ); 

                   gCfg.Options &= ~OPT_BGIMAGE;

                   gRefresh = TRUE;  // windows OS bug fix...

              break;


         case IDC_LOADIMAGE_BTN:
                      strcpy( Filename, GetImageFileName() );
                      break;

         case IDOK:
               {
                DWORD w = 0, h = 0, Flags = 0;

                // Get image placeement/action... ( from radio buttons... )
                
                // Clear the old Image placement flags...
                Flags = OPT_BGTILE | OPT_BG4TILE | OPT_BGSTRETCH | OPT_BGTOPLEFT |
                        OPT_BGTOPCENTER | OPT_BGBOTTOMLEFT |
                        OPT_BGBOTTOMCENTER | OPT_BGCENTER;

                gCfg.Options &= ~Flags;


                     if( IsDlgButtonChecked( hDlg, IDC_RADIO1 ) )  
                       { // Stretch to screen size
                        gCfg.Options |= OPT_BGSTRETCH;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO2 ) )  
                       { // Tile background image
                        gCfg.Options |= OPT_BGTILE;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO8 ) )  
                       { // Tile background image
                        gCfg.Options |= OPT_BG4TILE;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO3 ) )  
                       { // Top left of screen
                        gCfg.Options |= OPT_BGTOPLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO4 ) )  
                       { // Center of screen
                        gCfg.Options |= OPT_BGCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO5 ) )  
                       { // Bottom left of screen
                        gCfg.Options |= OPT_BGBOTTOMLEFT;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO6 ) )  
                       { // Top  center of screen
                        gCfg.Options |= OPT_BGTOPCENTER;
                       }
                else if( IsDlgButtonChecked( hDlg, IDC_RADIO7 ) )  
                       { // Bottom center of screen
                        gCfg.Options |= OPT_BGBOTTOMCENTER;
                       }
                      else  // just use default...
                       {
                        gCfg.Options |= OPT_BGSTRETCH;
                       }
                      

                if( strlen( Filename ) >  2) // did they change the image?
                  {
                   

                   if( gBgImage ) // in case they try and change it...
                       DeleteObject( gBgImage );

                   if( toupper( Filename[ strlen( Filename ) - 1 ] ) == 'P' )
                     { 
                      BITMAP bm;

                      gBgImage = (HBITMAP) LoadImage( NULL, Filename, IMAGE_BITMAP,
                                                      0, 0, LR_LOADFROMFILE );

                      GetObject( gBgImage, sizeof(BITMAP),(LPVOID) &bm );
                      w = bm.bmWidth;
                      h = bm.bmHeight;
                     } // Get bitmap info...
                    else
                     { 
                      gBgImage = LoadJPG( Filename, &w, &h );
                     }

                    gWidth = w;
                    gHeight = h;

                    gCfg.Options |= OPT_BGIMAGE;

                    strcpy( gCfg.BgFileName, Filename );

                  } // end if filename > 2

                  gRefresh = TRUE;  // windows OS bug fix...

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
 * Date:          12/17/2014  ( 10:15:35 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:15:35 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK StartUpDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       gRefresh = FALSE; 
           return TRUE;

    case WM_MOUSEMOVE:
                     {
                      int xPos = GET_X_LPARAM(lParam); 
                      int yPos = GET_Y_LPARAM(lParam); 

                      if( ( xPos > PREVIEW_X )           &&
                          ( xPos < PREVIEW_X+PREVIEW_W ) &&
                          ( yPos > PREVIEW_Y )           &&
                          ( yPos < PREVIEW_Y+PREVIEW_H ) )
                      {
                       SetCursor( gHandCursor );
                      } 
                     else  
                      {
                       SetCursor( gArrowCursor );   
                      }

            
                     }

                     return TRUE; 


    case WM_LBUTTONDOWN:
                     {
                      int xPos = GET_X_LPARAM(lParam); 
                      int yPos = GET_Y_LPARAM(lParam); 

                      if( ( xPos > PREVIEW_X )           &&
                          ( xPos < PREVIEW_X+PREVIEW_W ) &&
                          ( yPos > PREVIEW_Y )           &&
                          ( yPos < PREVIEW_Y+PREVIEW_H ) )
                      {
                       gPrevWnd = hDlg; 
                       ShowPreviewWindow();
                      } 

            
                     }
                     return TRUE;

    case WM_PAINT:
                  {
                   PAINTSTRUCT ps;
                   HDC hDC = BeginPaint( hDlg, &ps );

                   DrawPreviewAll( hDC ); 
                    
                   // Draw text over everyting if any...
                   SetBkMode( hDC, TRANSPARENT );
                   SetTextColor( hDC, gCfg.TextColor );
                   TextOut( hDC, 205, 120, "( Click to preview )", 20 );

                   if( gRefresh ) // Only use after back buffer blit!
                     {  
                      RECT r;

                      r.left = PREVIEW_X; // just for now till I hook up the back buffer!
                      r.top  = PREVIEW_Y;
                      r.right  = r.left + PREVIEW_W;
                      r.bottom = r.top  + PREVIEW_H;

                      gRefresh = FALSE; 
                      InvalidateRect( hDlg, &r, FALSE );
                      UpdateWindow( hDlg ); // quick hack - win fix.
                     }

                   EndPaint( hDlg, &ps ); 
                  }
                  return TRUE;

	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_DECOY_BTN:
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_DECOY_DLG ),
                         NULL, DecoyDlgProc );
                  ShowWindow( hDlg, SW_SHOW );
                  break;


         case IDC_SETBG_COLOR_BTN:
                  {
                   gCfg.BgColor = GetColor();
                   InvalidateRect( hDlg, NULL, TRUE );
                   UpdateWindow( hDlg );
                  }
                  break;

         case IDC_SETBGIMAGE_BTN: 
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETBGIMAGE_DLG ),
                         NULL, SetBgImageDlgProc );
                  ShowWindow( hDlg, SW_SHOW );
                  break;

         case IDC_LOGO_BTN:
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETLOGOIMAGE_DLG ),
                         NULL, SetLogoImageDlgProc );
                  ShowWindow( hDlg, SW_SHOW );
                  break;

         case IDC_TEXT_BTN:
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETTEXT_DLG ),
                         NULL, SetTextDlgProc );
                  ShowWindow( hDlg, SW_SHOW );
                  break;

         case IDCANCEL:
         case IDOK: 
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
 * Date:          12/17/2014  ( 10:16:16 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:16:16 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK DecoyDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static char TmpStr[ 255 ];
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 

                       if( gCfg.Options & OPT_DECOYREG )
                         {
                          CheckDlgButton( hDlg, IDC_REG_CHK, BST_CHECKED );
                         }

                       if( gCfg.Options & OPT_CUSTVIEWERNAME )
                         {
                          CheckDlgButton( hDlg, IDC_SETUPNAME_CHK, BST_CHECKED );
                          EnableWindow( GetDlgItem( hDlg, IDC_SETUPFILENAME_EDT ), TRUE );

                          SetWindowText(  GetDlgItem( hDlg, IDC_SETUPFILENAME_EDT ), gCfg.CustViewerFilename );
                         } 
                        else
                         EnableWindow( GetDlgItem( hDlg, IDC_SETUPFILENAME_EDT ), FALSE );


                       if( gCfg.Options & OPT_CUSTDATFILENAME )
                         {  
                          CheckDlgButton( hDlg, IDC_CUSTOM_DATFILE_CHK, BST_CHECKED );

                          EnableWindow( GetDlgItem( hDlg, IDC_DATBASENAME_EDT ),  TRUE );
                          EnableWindow( GetDlgItem( hDlg, IDC_DATEXTENSION_EDT ), TRUE );

                          SetWindowText(  GetDlgItem( hDlg, IDC_DATBASENAME_EDT ), gCfg.CustDatName );
                          SetWindowText(  GetDlgItem( hDlg, IDC_DATEXTENSION_EDT ), gCfg.CustDatExt );
                         }
                        else
                         {
                          EnableWindow( GetDlgItem( hDlg, IDC_DATBASENAME_EDT ),  FALSE );
                          EnableWindow( GetDlgItem( hDlg, IDC_DATEXTENSION_EDT ), FALSE );
                         }


           return TRUE;


    case WM_PAINT:
           {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint( hDlg, &ps );

            if( ghCustIcon )
                DrawIcon( hDC, 20,28, ghCustIcon );

            EndPaint( hDlg, &ps );
           }
           break;

	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_CUSTOM_DATFILE_CHK:

                if( IsDlgButtonChecked( hDlg, IDC_CUSTOM_DATFILE_CHK ) )
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_DATBASENAME_EDT ),  TRUE );
                   EnableWindow( GetDlgItem( hDlg, IDC_DATEXTENSION_EDT ), TRUE );
                  }
                 else
                  {
                   EnableWindow( GetDlgItem( hDlg, IDC_DATBASENAME_EDT ),  FALSE );
                   EnableWindow( GetDlgItem( hDlg, IDC_DATEXTENSION_EDT ), FALSE );
                  }

              break;

         case IDC_SETUPNAME_CHK:

                if( IsDlgButtonChecked( hDlg, IDC_SETUPNAME_CHK ) )
                    EnableWindow( GetDlgItem( hDlg, IDC_SETUPFILENAME_EDT ), TRUE );
                   else
                    EnableWindow( GetDlgItem( hDlg, IDC_SETUPFILENAME_EDT ), FALSE );
                
              break;


         case IDC_BROWSE_BTN:
              {
                DWORD index = 0;
                char *p = NULL;
  
               p = GetFileName();

               if(! p)
                  break; 

               SelectIconA(hDlg, p, 255, &index );
     
               if( ghCustIcon )
                   DestroyIcon( ghCustIcon );

               ExtractIconEx( p,  index, &ghCustIcon, NULL, 1 /* NumIcons*/ );
              

               InvalidateWindow( hDlg );
              }
              break;




         case IDC_HELPCUSTICON_BTN:

              OK_Msg("Help", "This option lets you choose an icon that appears in windows\r\n"\
				             "explorer when your CD/DVD is inserted.");

              break;


         case IDC_HELPCUSTICON_BTN4: // startup password dialog box help... ( enter reg code )...

              OK_Msg("Help", "This option shows 3 boxes as \"registration\" code entrys so\r\n" \
				             "the user does not know it's asking for a password. The password\r\n"\
							 "should be entered into the bottom box.");

              break;


         case IDC_HELPCUSTICON_BTN3: // change startup CD "viewer" filename...

              OK_Msg("Help", "This option lets you specify what the autorun file is named\r\n" \
				             "that asks for your password and decrypts the files.");

              break;



         case IDC_HELPDATNAME_BTN:

              OK_Msg("Help", "\".dat\" files are generated with \"basename00000.extention\" ,\r\n" \
                             "where \"00000\" is an auto-incremented number.\r\n\r\n" \
                             "If you have \"test\" as the base name and \"mpg\" as the\r\n" \
                             "extension the result filenames would be: test1.mpg, test2.mpg\r\n" \
                             "and so on... making others think their just corrupt .mpg files\r\n" \
                             "on the CD ( because they wont open ). ");

              break;


         case IDOK:

                if( IsDlgButtonChecked( hDlg, IDC_REG_CHK ) )
                    gCfg.Options |= OPT_DECOYREG; 
                   else
                    gCfg.Options &= ~OPT_DECOYREG; 

                if( IsDlgButtonChecked( hDlg, IDC_SETUPNAME_CHK ) )
                  {
                   gCfg.Options |= OPT_CUSTVIEWERNAME; 

                   GetWindowText(GetDlgItem(hDlg, IDC_SETUPFILENAME_EDT), TmpStr,
                                 GetWindowTextLength(GetDlgItem(hDlg,IDC_SETUPFILENAME_EDT) ) + 1);

                   if( TmpStr[ 0 ] )
                      strcpy( gCfg.CustViewerFilename, TmpStr );
                  } 
                 else
                  gCfg.Options &= ~OPT_CUSTVIEWERNAME; 

                if( IsDlgButtonChecked( hDlg, IDC_CUSTOM_DATFILE_CHK ) )
                  {
                   gCfg.Options |= OPT_CUSTDATFILENAME; 

                   GetWindowText(GetDlgItem(hDlg, IDC_DATBASENAME_EDT), TmpStr,
                                 GetWindowTextLength(GetDlgItem(hDlg,IDC_DATBASENAME_EDT) ) + 1);

                   if( TmpStr[ 0 ] )
                      strcpy( gCfg.CustDatName, TmpStr );

                   GetWindowText(GetDlgItem(hDlg, IDC_DATEXTENSION_EDT), TmpStr,
                                 GetWindowTextLength(GetDlgItem(hDlg,IDC_DATEXTENSION_EDT) ) + 1);

                   if( TmpStr[ 0 ] )
                      strcpy( gCfg.CustDatExt, TmpStr );

                  }  
                 else
                  gCfg.Options &= ~OPT_CUSTDATFILENAME; 

                  EndDialog( hDlg, TRUE );
                  break;

         case IDCANCEL: 
                  EndDialog(hDlg, TRUE);
                  break;
        }

     break; 
   }

  return FALSE;
}
