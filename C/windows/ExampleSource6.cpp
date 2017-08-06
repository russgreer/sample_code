
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 10:16:53 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:16:53 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK EulaDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 

                       if( gCfg.EulaText )
                          SetWindowText( GetDlgItem( hDlg, IDC_EULA_EDT ), &gCfg.EulaText[0] );

                        return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDOK:
                  {
                   DWORD Len = 0;

                   Len = GetWindowTextLength(GetDlgItem(hDlg, IDC_EULA_EDT) ) + 1;

                   if(! Len ) // didn't enter anything... clear it...
                     {
                      CustFreeAllocatedMem( &gCfg.EulaText[0],  "EulaDlgProc()", __LINE__ );
                      gCfg.EulaText = NULL;

                      break;
                     }

                   if(!(gCfg.EulaText = (char *) CustAllocateMem( Len + 100,  "EulaDlgProc()", __LINE__ ) ))
                   {
                    OK_Msg("Error: (2345234)", "Could not allocate memory for EULA text."); 
                    break;
                   }

                   memset( &gCfg.EulaText[0], 0, Len + 50 );

                   GetWindowText(GetDlgItem(hDlg, IDC_EULA_EDT ),
                                 &gCfg.EulaText[0], Len );

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
 * Date:          12/17/2014  ( 10:17:47 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:17:47 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK PropDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static CRYPT_ITEM *CryptItem = NULL;

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                       HTREEITEM hItem;
                       TVITEM tvi;
                        
                       char WorkStr[ 255 ];

                       if(! ( hItem = TreeView_GetSelection( ghCryptCDWnd ) ) )
                          break;

                       tvi.hItem = hItem;
                       tvi.mask = TVIF_STATE | TVIF_PARAM;
  
                       if(!(TreeView_GetItem( ghCryptCDWnd, &tvi )))
                          break;

                       if(! ( CryptItem = ( CRYPT_ITEM * ) tvi.lParam ) )
                          break;   

                       int n = 0;
                       USER_ID_LIST *IDListPtr = CryptItem ->ID_List_Head;
                       if( IDListPtr )
                         {
                           while( IDListPtr )
                           {
                            SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_INSERTSTRING, 0, (LPARAM)  GetUserNameFromID( IDListPtr ->User_ID ) );
  
                            n++;
                            IDListPtr = IDListPtr ->Next;
                           }
 
                         }


                      if( ! n ) 
                          SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "- No users assigned to this item -" );
                        
                      SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );
              

                       
                       if( CryptItem ->ItemName )
                           SetWindowText( GetDlgItem( hDlg, IDC_NAME_STATIC ), CryptItem ->ItemName );

#if !defined(HOME_VER)

                       if( CryptItem ->ItemNotes )
                           SetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), CryptItem ->ItemNotes );
                         
                       if( CryptItem ->SuspectName )
                           SetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), CryptItem ->SuspectName );
                      
                       if( CryptItem ->CaseNumber != -1 )
                         {
                          sprintf( WorkStr, "%ld", CryptItem ->CaseNumber );
                          SetWindowText( GetDlgItem( hDlg, IDC_CASENUM_EDT ), WorkStr );
                         }
#endif 

                       if( CryptItem ->InfoType == ID_DRIVES_TREEVIEW )
                         {
                          // Count and update the Files, Folders, Size of this Sub Tree
                          GetFolderProperties( hItem ); 

                          SetWindowText( GetDlgItem( hDlg, IDC_TYPE_STATIC ), "Folder" );

                          sprintf( WorkStr, "Total %ld Bytes ( %0.2f KB/%0.2f MB) - Files: %ld - Sub Folders: %ld",
                                   gProp.TotalByteSize,
                                   ((float) gProp.TotalByteSize / 1024 ),
                                   ((float)((float) gProp.TotalByteSize / 1024 ) / 1024 ),
                                   gProp.TotalNumFiles,
                                   gProp.TotalSubFolders );

                          SetWindowText( GetDlgItem( hDlg, IDC_SIZE_STATIC ), WorkStr );
                         }
                        else
                         {  
                          SetWindowText( GetDlgItem( hDlg, IDC_TYPE_STATIC ), "File" );

                          sprintf( WorkStr, "%ld Bytes ( %0.2f KB/%0.2f MB)",  CryptItem ->Size, ((float) CryptItem ->Size / 1024 ), ((float)((float) CryptItem ->Size / 1024 ) / 1024 ));
                          SetWindowText( GetDlgItem( hDlg, IDC_SIZE_STATIC ), WorkStr );
                         }


                       if( ! ( CryptItem ->Flags & FLAG_ENCRYPTED ) )
                          SetWindowText( GetDlgItem( hDlg, IDC_ENCTYPE_STATIC ), "Not encrypted." );
                         else
                          { 
                           memset( WorkStr, 0, 255 );

                           if( CryptItem ->Flags & FLAG_ENC_XOR )      strcat( WorkStr, "BitCrypt " );
                           if( CryptItem ->Flags & FLAG_ENC_SCRAMBLE ) strcat( WorkStr, "SCRAMBLE " );
                           if( CryptItem ->Flags & FLAG_ENC_PGP )      strcat( WorkStr, "PGP " );
                           if( CryptItem ->Flags & FLAG_ENC_BLOWFISH ) strcat( WorkStr, "BlowFish " );
                           if( CryptItem ->Flags & FLAG_ENC_2FISH )    strcat( WorkStr, "2Fish " );
                           if( CryptItem ->Flags & FLAG_ENC_AES )      strcat( WorkStr, "AES " );
                           if( CryptItem ->Flags & FLAG_ENC_BLOWFISH ) strcat( WorkStr, "BLOWFISH " );


                           if( CryptItem ->Flags & FLAG_ENC_FULL )
                              strcat( WorkStr, " - ( FULL )" );
                             else
                              strcat( WorkStr, " - ( FAST )" );

                           SetWindowText( GetDlgItem( hDlg, IDC_ENCTYPE_STATIC ), WorkStr );
                           
                          } 

                       }
                       return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
	              EndDialog(hDlg, TRUE);
                  break;

#if !defined(HOME_VER)

         case IDC_EDIT_USERS_BTN: 
                  {
                   if( ! ( CryptItem ->ID_List_Head ) ) // put up add user dlg
                     DialogBox( ghInst, MAKEINTRESOURCE( IDD_USER_DLG ),
                                NULL, AdminUsersDlgProc);
                   else
                   // put up "edit users" dialog for this item...
                   DialogBox( ghInst, MAKEINTRESOURCE( IDD_ITEMUSERS_DLG ),
                              NULL, AssignUserDlgProc);
            
                   // refresh the dialog box num users edit...
                   ResetShowAssignedUsers(  CryptItem, GetDlgItem(hDlg, IDC_USERS_COMBO ) );
                  }
                  break;

#endif

	 	 case IDC_DONE_BTN: 
                  {
                   char TmpStr[ 100 ];

                  // Get the case #... if any...
                  memset( TmpStr, 0, 100 );

#if !defined(HOME_VER)

                  GetWindowText( GetDlgItem( hDlg, IDC_CASENUM_EDT ), TmpStr, GetWindowTextLength( GetDlgItem( hDlg, IDC_CASENUM_EDT ) ) + 1 );
                  if( TmpStr[ 0 ] )
                      CryptItem ->CaseNumber = atoi( TmpStr );
                    else
                      CryptItem ->CaseNumber = -1;


                  // Get notes ( if any )...
                  if( CryptItem ->ItemNotes )
                    {
                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), p, Len + 1 );       
                            CryptItem ->ItemNotes = p;                         
                           }
                          //else return FALSE;


                       }
                      else  // remove the note if there was one...
                       {
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;
                       }

                    }
                   else // was no note... are we adding one?
                    {

                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), p, Len + 1 );       
                            CryptItem ->ItemNotes = p;                         
                           }
                          


                       }

                    }

 

                  // suspect name..
                  if( CryptItem ->SuspectName )
                    {
                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), p, Len + 1 );       
                            CryptItem ->SuspectName = p;                         
                           }
                          


                       }
                      else  // remove the note if there was one...
                       {
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;
                       }

                    }
                   else // was no note... are we adding one?
                    {

                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), p, Len + 1 );       
                            CryptItem ->SuspectName = p;                         
                           }
                          


                       }

                    }
                  // end suspect name
#endif

                  }
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
 * Date:          5/22/2014  ( 9:22:23 AM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 5/22/2014  ( 9:22:23 AM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK PropFileDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static CRYPT_ITEM *CryptItem = NULL;

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                         char WorkStr[ 255 ];
                         memset( WorkStr, 0, 255 );

                         LV_ITEM lvi;
                         lvi.mask = LVIF_TEXT | LVIF_PARAM;
                         lvi.iItem = ListView_GetSelectionMark( ghCryptFilesWnd );
                         lvi.pszText = WorkStr;
                         lvi.cchTextMax = 255; 

                         if( ! ( ListView_GetItem( ghCryptFilesWnd, &lvi ) ) )
                                 break;                           


                         CryptItem = ( CRYPT_ITEM * ) lvi.lParam;
                        
                         

                       int n = 0;
                       USER_ID_LIST *IDListPtr = CryptItem ->ID_List_Head;
                       if( IDListPtr )
                         {
                           while( IDListPtr )
                           {
                            SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_INSERTSTRING, 0, (LPARAM)  GetUserNameFromID( IDListPtr ->User_ID ) );
  
                            n++;
                            IDListPtr = IDListPtr ->Next;
                           }
 
                         }


                      if( ! n ) 
                          SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "- No users assigned to this item -" );
                        
                      SendMessage( GetDlgItem(hDlg, IDC_USERS_COMBO ), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0 );
              

                       
                       if( CryptItem ->ItemName )
                           SetWindowText( GetDlgItem( hDlg, IDC_NAME_STATIC ), CryptItem ->ItemName );

#if !defined(HOME_VER)

                       if( CryptItem ->ItemNotes )
                           SetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), CryptItem ->ItemNotes );
                         
                       if( CryptItem ->SuspectName )
                           SetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), CryptItem ->SuspectName );
                      
                       if( CryptItem ->CaseNumber != -1 )
                         {
                          sprintf( WorkStr, "%ld", CryptItem ->CaseNumber );
                          SetWindowText( GetDlgItem( hDlg, IDC_CASENUM_EDT ), WorkStr );
                         }
#endif 


                       SetWindowText( GetDlgItem( hDlg, IDC_TYPE_STATIC ), "File" );

                       sprintf( WorkStr, "%ld Bytes ( %0.2f KB/%0.2f MB)",  CryptItem ->Size, ((float) CryptItem ->Size / 1024 ), ((float)((float) CryptItem ->Size / 1024 ) / 1024 ));
                       SetWindowText( GetDlgItem( hDlg, IDC_SIZE_STATIC ), WorkStr );
                         


                       if( ! ( CryptItem ->Flags & FLAG_ENCRYPTED ) )
                          SetWindowText( GetDlgItem( hDlg, IDC_ENCTYPE_STATIC ), "Not encrypted." );
                         else
                          { 
                           memset( WorkStr, 0, 255 );

                           if( CryptItem ->Flags & FLAG_ENC_XOR )      strcat( WorkStr, "BitCrypt " );
                           if( CryptItem ->Flags & FLAG_ENC_SCRAMBLE ) strcat( WorkStr, "SCRAMBLE " );
                           if( CryptItem ->Flags & FLAG_ENC_PGP )      strcat( WorkStr, "PGP " );
                           if( CryptItem ->Flags & FLAG_ENC_BLOWFISH ) strcat( WorkStr, "BlowFish " );
                           if( CryptItem ->Flags & FLAG_ENC_2FISH )    strcat( WorkStr, "2Fish " );
                           if( CryptItem ->Flags & FLAG_ENC_AES )      strcat( WorkStr, "AES " );
                           if( CryptItem ->Flags & FLAG_ENC_BLOWFISH ) strcat( WorkStr, "BLOWFISH " );


                           if( CryptItem ->Flags & FLAG_ENC_FULL )
                              strcat( WorkStr, " - ( FULL )" );
                             else
                              strcat( WorkStr, " - ( FAST )" );

                           SetWindowText( GetDlgItem( hDlg, IDC_ENCTYPE_STATIC ), WorkStr );
                           
                          } 

                       }
                       return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDCANCEL:
	              EndDialog(hDlg, TRUE);
                  break;

#if !defined(HOME_VER)

         case IDC_EDIT_USERS_BTN: 
                  {

                   if( ! ( CryptItem ->ID_List_Head ) ) // put up add user dlg
                     DialogBox( ghInst, MAKEINTRESOURCE( IDD_USER_DLG ),
                                NULL, AdminUsersDlgProc);
                   else
                   // put up "edit users" dialog for this item...
                   DialogBox( ghInst, MAKEINTRESOURCE( IDD_ITEMUSERS_DLG ),
                              NULL, AssignFileUserDlgProc);
            
                   // refresh the dialog box num users edit...
                   ResetShowAssignedUsers(  CryptItem, GetDlgItem(hDlg, IDC_USERS_COMBO ) );
                  }
                  break;

#endif

	 	 case IDC_DONE_BTN: 
                  {
                   char TmpStr[ 100 ];

                  // Get the case #... if any...
                  memset( TmpStr, 0, 100 );

#if !defined(HOME_VER)

                  GetWindowText( GetDlgItem( hDlg, IDC_CASENUM_EDT ), TmpStr, GetWindowTextLength( GetDlgItem( hDlg, IDC_CASENUM_EDT ) ) + 1 );
                  if( TmpStr[ 0 ] )
                      CryptItem ->CaseNumber = atoi( TmpStr );
                    else
                      CryptItem ->CaseNumber = -1;


                  // Get notes ( if any )...
                  if( CryptItem ->ItemNotes )
                    {
                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), p, Len + 1 );       
                            CryptItem ->ItemNotes = p;                         
                           }
                          


                       }
                      else  // remove the note if there was one...
                       {
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;
                       }

                    }
                   else // was no note... are we adding one?
                    {

                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->ItemNotes )
                            CustFreeAllocatedMem( CryptItem ->ItemNotes, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->ItemNotes = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_PROP_NOTES_EDT ), p, Len + 1 );       
                            CryptItem ->ItemNotes = p;                         
                           }
                          


                       }

                    }

 

                  // suspect name..
                  if( CryptItem ->SuspectName )
                    {
                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), p, Len + 1 );       
                            CryptItem ->SuspectName = p;                         
                           }
                         


                       }
                      else  // remove the note if there was one...
                       {
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;
                       }

                    }
                   else // was no note... are we adding one?
                    {

                     int Len = GetWindowTextLength( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ) );
                     if( Len )
                       {
                        // update this note...
                        if( CryptItem ->SuspectName )
                            CustFreeAllocatedMem( CryptItem ->SuspectName, "RemoveCryptItem()", __LINE__  );

                        CryptItem ->SuspectName = NULL;


                         char *p = NULL;
                         if( ( p = (char *) CustAllocateMem( Len + 10,  "EditUserDlgProc()", __LINE__ ) ) )
                           {
                            memset( p, 0, Len + 5 );
                            GetWindowText( GetDlgItem( hDlg, IDC_SUSPECTNAME_EDT ), p, Len + 1 );       
                            CryptItem ->SuspectName = p;                         
                           }
                          


                       }

                    }
                  // end suspect name
#endif

                  }
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
 * Date:          12/17/2014  ( 10:18:45 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:18:45 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK AssignUserDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static CRYPT_ITEM *CryptItem = NULL;

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                       // Get this CryptItem Ptr...
                       HTREEITEM hItem;
                       TVITEM tvi;
                        
                       if(! ( hItem = TreeView_GetSelection( ghCryptCDWnd ) ) )
                          break;

                       tvi.hItem = hItem;
                       tvi.mask = TVIF_STATE | TVIF_PARAM;
  
                       if(!(TreeView_GetItem( ghCryptCDWnd, &tvi )))
                          break;

                       if(! ( CryptItem = ( CRYPT_ITEM * ) tvi.lParam ) )
                          break;   

                        // put ALL users in the combo box...
                        // reset fills the user list too...
                        ResetShowUsers( GetDlgItem( hDlg, IDC_USERS_COMBO ) );

                        // put ALL users assigned to this item in the other combo box...
                        ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );

                      }
                      return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_CREATE_BTN:
                               gTmpEditUser = NULL;
                               ShowWindow( hDlg, SW_HIDE );
                               DialogBox( ghInst, MAKEINTRESOURCE( IDD_ADDUSER_DLG ),
                                          NULL, AddUserDlgProc);
                               ShowWindow( hDlg, SW_SHOW );
                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
                               break;

         case IDC_REMOVE_USER_BTN:
                              { 
                               DWORD User_ID = -1;
                               // Assign the selected user to this node
                               // ( if they are NOT already assigned to it...
                               // Get Selected username from the combo box...

                               char Name[ 255 ];
                               memset( Name, 0, 255 );

                               // get the user ID to edit...
                               int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                               SendMessage(  GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ), 
                                                           (UINT) CB_GETLBTEXT,
                                                           (WPARAM) lResult, (LPARAM) Name );  
   
                               if(! Name[0])
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }
                              
                              
                               // find the user_id by name... 
                               USER  *TmpNode = gUserListHead;           
                               BOOL Found = FALSE;
                               while( TmpNode )
                                    {
 
                                     if( ! strcmp( Name, TmpNode ->UserName ) )
                                       {
                                        Found = TRUE;
                                        User_ID = TmpNode ->User_ID;
                                        break;
                                       }  

                                     TmpNode = TmpNode ->Next;
                                    }                                

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }  
                               

                               // remove this user assignment from this node...
                               // Node = CryptItem ->UserIDList ->User_ID

                               // find the Node to remove...
                               USER_ID_LIST *TmpUserIDNode = CryptItem ->ID_List_Head;
                               Found = FALSE;
                               while( TmpUserIDNode )
                               {
                                if( TmpUserIDNode ->User_ID == User_ID )
                                  {
                                    Found = TRUE;
                                    break;
                                  } 

                                TmpUserIDNode = TmpUserIDNode ->Next;
                               }

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user ID. Please select and try again." );
                                  break;
                                 }  
                                else                            
                                 RemoveUserIDNode( CryptItem, TmpUserIDNode );

                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
  
                              }
                              break;


         case IDC_ASSIGN_USER:
                              {
                               DWORD User_ID = -1;
                               // Assign the selected user to this node
                               // ( if they are NOT already assigned to it...
                               // Get Selected username from the combo box...

                               char Name[ 255 ];
                               memset( Name, 0, 255 );

                               // get the user ID to edit...
                               int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_USERS_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                               SendMessage(  GetDlgItem( hDlg, IDC_USERS_COMBO ), 
                                                           (UINT) CB_GETLBTEXT,
                                                           (WPARAM) lResult, (LPARAM) Name );  
   
                               if(! Name[0])
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }
                              
                               USER  *TmpNode = gUserListHead;           
                               BOOL Found = FALSE;
                               while( TmpNode )
                                    {
 
                                     if( ! strcmp( Name, TmpNode ->UserName ) )
                                       {
                                        Found = TRUE;
                                        User_ID = TmpNode ->User_ID;
                                        break;
                                       }  

                                     TmpNode = TmpNode ->Next;
                                    }                                

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }  
                              

                               AddUserIDToCryptItem( CryptItem, User_ID );

                               // user was assigned to a folder, assign the user to all subfolders and files too...
                               RecursiveSetUser( CryptItem ->ID, User_ID );


                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
                              }
                              break;



         case IDFINSIHED: // "Done >>"
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
 * Date:          5/22/2014  ( 10:18:02 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 5/22/2014  ( 10:18:02 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         TODO: change this to use ONLY the crypt listview!!!                       
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK AssignFileUserDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static CRYPT_ITEM *CryptItem = NULL;

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                       // Get this CryptItem Ptr...
                         LV_ITEM lvi;
                         lvi.mask = LVIF_PARAM;
                         lvi.iItem = ListView_GetSelectionMark( ghCryptFilesWnd );

                         if( ! ( ListView_GetItem( ghCryptFilesWnd, &lvi ) ) )
                                 break;   
                        
                         if( ! ( CryptItem = ( CRYPT_ITEM * ) lvi.lParam ) )
                                break;

                        // put ALL users in the combo box...
                        // reset fills the user list too...
                        ResetShowUsers( GetDlgItem( hDlg, IDC_USERS_COMBO ) );

                        // put ALL users assigned to this item in the other combo box...
                        ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );

                      }
                      return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_CREATE_BTN:
                               gTmpEditUser = NULL;
                               ShowWindow( hDlg, SW_HIDE );
                               DialogBox( ghInst, MAKEINTRESOURCE( IDD_ADDUSER_DLG ),
                                          NULL, AddUserDlgProc);
                               ShowWindow( hDlg, SW_SHOW );
                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
                               break;

         case IDC_REMOVE_USER_BTN:
                              { 
                               DWORD User_ID = -1;
                               // Assign the selected user to this node
                               // ( if they are NOT already assigned to it...
                               // Get Selected username from the combo box...

                               char Name[ 255 ];
                               memset( Name, 0, 255 );

                               // get the user ID to edit...
                               int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                               SendMessage(  GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ), 
                                                           (UINT) CB_GETLBTEXT,
                                                           (WPARAM) lResult, (LPARAM) Name );  
   
                               if(! Name[0])
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }
                              
                              
                               // find the user_id by name... 
                               USER  *TmpNode = gUserListHead;           
                               BOOL Found = FALSE;
                               while( TmpNode )
                                    {
 
                                     if( ! strcmp( Name, TmpNode ->UserName ) )
                                       {
                                        Found = TRUE;
                                        User_ID = TmpNode ->User_ID;
                                        break;
                                       }  

                                     TmpNode = TmpNode ->Next;
                                    }                                

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }  
                               

                               // remove this user assignment from this node...
                               // Node = CryptItem ->UserIDList ->User_ID

                               // find the Node to remove...
                               USER_ID_LIST *TmpUserIDNode = CryptItem ->ID_List_Head;
                               Found = FALSE;
                               while( TmpUserIDNode )
                               {
                                if( TmpUserIDNode ->User_ID == User_ID )
                                  {
                                    Found = TRUE;
                                    break;
                                  } 

                                TmpUserIDNode = TmpUserIDNode ->Next;
                               }

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user ID. Please select and try again." );
                                  break;
                                 }  
                                else                            
                                 RemoveUserIDNode( CryptItem, TmpUserIDNode );

                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
  
                              }
                              break;


         case IDC_ASSIGN_USER:
                              {
                               DWORD User_ID = -1;
                               // Assign the selected user to this node
                               // ( if they are NOT already assigned to it...
                               // Get Selected username from the combo box...

                               char Name[ 255 ];
                               memset( Name, 0, 255 );

                               // get the user ID to edit...
                               int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_USERS_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                               SendMessage(  GetDlgItem( hDlg, IDC_USERS_COMBO ), 
                                                           (UINT) CB_GETLBTEXT,
                                                           (WPARAM) lResult, (LPARAM) Name );  
   
                               if(! Name[0])
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }
                              
                               USER  *TmpNode = gUserListHead;           
                               BOOL Found = FALSE;
                               while( TmpNode )
                                    {
 
                                     if( ! strcmp( Name, TmpNode ->UserName ) )
                                       {
                                        Found = TRUE;
                                        User_ID = TmpNode ->User_ID;
                                        break;
                                       }  

                                     TmpNode = TmpNode ->Next;
                                    }                                

                               if( ! Found )
                                 {
                                  OK_Msg("INFO", "Cannot find user. Please select and try again." );
                                  break;
                                 }  
                              

                               AddUserIDToCryptItem( CryptItem, User_ID );

                               // user was assigned to a folder, assign the user to all subfolders and files too...
                               RecursiveSetUser( CryptItem ->ID, User_ID );


                               ResetShowAssignedUsers( CryptItem, GetDlgItem( hDlg, IDC_CURRENTUSERS_COMBO ) );
                              }
                              break;



         case IDFINSIHED: // "Done >>"
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
 * Date:          12/17/2014  ( 10:19:57 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:19:57 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK SaveLayoutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  char *p = NULL;
  static char Password[ 255 ]; 
  static char TmpStr[ 255 ]; 

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                      {
                       memset( &gLayoutHeader, 0, sizeof(LAYOUT_HEADER ) );

                       if( gLayoutHeader.Flags & FLAG_LAYOUTSAVEALL )
                           CheckDlgButton( hDlg, IDC_SAVEALL_CHK, TRUE );

                       if( gLayoutHeader.Flags & FLAG_LAYOUTPASSWORD )
                         {
                          CheckDlgButton( hDlg, IDC_PASSWORDPROTECT_CHK, TRUE );
                          EnableWindow( GetDlgItem( hDlg, IDC_PASSWORD_EDT), TRUE );
                         }
                        else
                         {
                          EnableWindow( GetDlgItem( hDlg, IDC_PASSWORD_EDT), FALSE );
                         }

                      }
                      return TRUE;


	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_PASSWORDPROTECT_CHK:
              if( IsDlgButtonChecked( hDlg, IDC_PASSWORDPROTECT_CHK ) )  
                  EnableWindow( GetDlgItem( hDlg, IDC_PASSWORD_EDT), TRUE );
                 else
                  EnableWindow( GetDlgItem( hDlg, IDC_PASSWORD_EDT), FALSE );
                   
              break;

        case IDCANCEL:
	              EndDialog(hDlg, TRUE);
                  return TRUE;

	 	 case IDOK: // Save this layout... 


                  if( IsDlgButtonChecked( hDlg, IDC_SAVEALL_CHK ) )  
                      gLayoutHeader.Flags |= FLAG_LAYOUTSAVEALL;
                     else
                      gLayoutHeader.Flags &= ~FLAG_LAYOUTSAVEALL;

                  if( IsDlgButtonChecked( hDlg, IDC_PASSWORDPROTECT_CHK ) )  
                    { 
                     gLayoutHeader.Flags |= FLAG_LAYOUTPASSWORD;

                     GetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDT), Password,
                                   GetWindowTextLength(GetDlgItem(hDlg,IDC_PASSWORD_EDT) ) + 1);

                    }
                   else gLayoutHeader.Flags &= ~FLAG_LAYOUTPASSWORD;

                   p = SaveLayoutFileName();

                   if(! p[0] )
                     {
                      OK_Msg("INFO:", "File not saved.");
                      EndDialog(hDlg, TRUE);
                      return TRUE;
                     } 


                  strcpy( TmpStr, p );

                  if( gLayoutHeader.Flags & FLAG_LAYOUTPASSWORD ) 
                     SaveLayout( TmpStr, Password  );
                    else
                     SaveLayout( TmpStr, NULL ); 

	              EndDialog(hDlg, TRUE);
                  return TRUE;
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
 * Date:          12/17/2014  ( 10:20:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 10:20:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static char TmpStr[ 255 ];

   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                    {
                     // set the current default encryption type

                     // reset the combo...
                     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_RESETCONTENT , (WPARAM) 0, (LPARAM) 0 );

#if defined(PRO_VER)

   #if defined(ENTERPRISE_VER) || (FORENSIC_VER)
               	     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "Blowfish" );                     
   #endif

   #ifndef DEMO_VERSION
               	     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "AES" );
   #endif

#endif

               	     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "Scramble" );
               	     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_INSERTSTRING, 0, (LPARAM) "BitCrypt" );


                     if( gCfg.Options & OPT_AUTOSAVE )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_AUTOSAVE_CHK, BST_CHECKED );
                       }

                      
                     int i = 0;
                          if( gCfg.MoreOptions & OPT_DEFAULT_XOR )      i = 0;
                     else if( gCfg.MoreOptions & OPT_DEFAULT_SCRAMBLE ) i = 1;
                     else if( gCfg.MoreOptions & OPT_DEFAULT_AES )      i = 2;
                     else if( gCfg.MoreOptions & OPT_DEFAULT_BLOWFISH ) i = 3;

                     SendMessage( GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), CB_SETCURSEL, (WPARAM) i, (LPARAM) 0 );



                     if( gCfg.Options & OPT_AUTOBOOT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_AUTOBOOT_CHK, BST_CHECKED );
                       }

                     if( gCfg.Options & OPT_NOEXPORT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_NO_EXPORT_CHK, BST_CHECKED );
                       }

                     if( gCfg.Options & OPT_CLEARRECENT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_RECENT_CHK, BST_CHECKED );
                       }

                     if( gCfg.Options & OPT_PASSPROTECT )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_PROTECT_CHK, BST_CHECKED );
                        EnableWindow( GetDlgItem(hDlg, IDC_SETPASSWORD_BTN ), TRUE );
                       }
                      else EnableWindow( GetDlgItem(hDlg, IDC_SETPASSWORD_BTN ), FALSE );

#if !defined(HOME_VER)
                                       
                     if( gCfg.Options & OPT_EULA )  
                       { // Top left of screen
                        CheckDlgButton( hDlg, IDC_EULA_CHK, BST_CHECKED );
                        EnableWindow( GetDlgItem(hDlg, IDC_SETEULA_BTN ), TRUE );
                       }
                      else EnableWindow( GetDlgItem(hDlg, IDC_SETEULA_BTN ), FALSE );


                     if( gCfg.Options & OPT_CUSTABOUTBOX )  
                       { 
                        CheckDlgButton( hDlg, IDC_CUSTABOUT_CHK, BST_CHECKED );
                        EnableWindow( GetDlgItem(hDlg, IDC_EDITABOUT_BTN ), TRUE );
                       }
                      else EnableWindow( GetDlgItem(hDlg, IDC_EDITABOUT_BTN ), FALSE );
#endif

#if defined(ENTERPRISE_VER) || (FORENSIC_VER)

                     if( gCfg.Options & OPT_SECUREDELETE )  
                       { 
                        CheckDlgButton( hDlg, IDC_ERASESECURE_CHK, BST_CHECKED );
                        EnableWindow( GetDlgItem(hDlg, IDC_REPS_EDT ), TRUE );
                        char TmpStr[ 20 ];
                        sprintf( TmpStr, "%ld", gCfg.NumReps );
                        SetWindowText( GetDlgItem(hDlg, IDC_REPS_EDT ), TmpStr );
                       }
                      else EnableWindow( GetDlgItem(hDlg, IDC_REPS_EDT ), FALSE );
#endif


                     SetWindowText( GetDlgItem( hDlg, IDC_CDFOLDER_EDT), gCfg.CreateCDDir );

           }
           return TRUE;
           

	case WM_COMMAND:

		switch (GET_WM_COMMAND_ID(wParam, lParam))
		{
         case IDC_BROWSE_BTN: // Browse for drive/dir to create image CD in...
              {
               char *p = NULL;
 
               p = GetDirName( "Choose a folder to place generated files in." );

               if(! p)
                  break; 

               strcpy( gCfg.CreateCDDir, p );

               SetWindowText( GetDlgItem( hDlg, IDC_CDFOLDER_EDT), gCfg.CreateCDDir );

              }
              break;


         case IDC_PROTECT_CHK:

                  if( IsDlgButtonChecked( hDlg, IDC_PROTECT_CHK ) )  
                     EnableWindow( GetDlgItem(hDlg, IDC_SETPASSWORD_BTN ), TRUE );
                   else
                     EnableWindow( GetDlgItem(hDlg, IDC_SETPASSWORD_BTN ), FALSE );

                  break;

#if defined(ENTERPRISE_VER) || (FORENSIC_VER)
         case  IDC_ERASESECURE_CHK:

                  if( IsDlgButtonChecked( hDlg,  IDC_ERASESECURE_CHK ) )  
                     EnableWindow( GetDlgItem(hDlg, IDC_REPS_EDT ), TRUE );
                   else
                     EnableWindow( GetDlgItem(hDlg, IDC_REPS_EDT ), FALSE );

                  break;
#endif


#if !defined(HOME_VER)

         case  IDC_CUSTABOUT_CHK:

                  if( IsDlgButtonChecked( hDlg,  IDC_CUSTABOUT_CHK ) )  
                     EnableWindow( GetDlgItem(hDlg, IDC_EDITABOUT_BTN ), TRUE );
                   else
                     EnableWindow( GetDlgItem(hDlg, IDC_EDITABOUT_BTN ), FALSE );

                  break;


         case IDC_EULA_CHK:

                  if( IsDlgButtonChecked( hDlg, IDC_EULA_CHK ) )  
                     EnableWindow( GetDlgItem(hDlg, IDC_SETEULA_BTN ), TRUE );
                   else
                     EnableWindow( GetDlgItem(hDlg, IDC_SETEULA_BTN ), FALSE );

                  break;
#endif


         case IDC_SETPASSWORD_BTN:  
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETPASSWORD_DLG ),
                            NULL,  SetPWDlgProc);
                  ShowWindow( hDlg, SW_SHOW );
                  break; 

#if !defined(HOME_VER)

         case IDC_SETEULA_BTN:
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_EULA_DLG ),
                             NULL, EulaDlgProc);
                  ShowWindow( hDlg, SW_SHOW );

                  break;

          
         case IDC_EDITABOUT_BTN:
                  ShowWindow( hDlg, SW_HIDE );
                  DialogBox( ghInst, MAKEINTRESOURCE( IDD_SETCUSTOMABOUT_DLG ),
                             NULL, CustAboutBoxDlgProc);
                  ShowWindow( hDlg, SW_SHOW );

                  break;

#endif

         case IDOK:
                   {           
          
                    GetWindowText(GetDlgItem(hDlg, IDC_CDFOLDER_EDT), gCfg.CreateCDDir,
                                  GetWindowTextLength(GetDlgItem(hDlg, IDC_CDFOLDER_EDT) ) + 1);



                    if( IsDlgButtonChecked( hDlg, IDC_AUTOSAVE_CHK ) )  
                        gCfg.Options |= OPT_AUTOSAVE;
                       else
                        gCfg.Options &= ~OPT_AUTOSAVE;


                    int lResult = (int) SendMessage(  GetDlgItem( hDlg, IDC_DEFAULTENC_COMBO ), 
                                                           (UINT) CB_GETCURSEL,
                                                           (WPARAM) 0, (LPARAM) 0 );  

                     gCfg.MoreOptions &= ~OPT_DEFAULT_XOR;
                     gCfg.MoreOptions &= ~OPT_DEFAULT_SCRAMBLE;
                     gCfg.MoreOptions &= ~OPT_DEFAULT_AES;
                     gCfg.MoreOptions &= ~OPT_DEFAULT_BLOWFISH;

                     if( lResult == 0 )
                         gCfg.MoreOptions |= OPT_DEFAULT_XOR;
                        else
                     if( lResult == 1 )
                         gCfg.MoreOptions |= OPT_DEFAULT_SCRAMBLE;
                        else
                     if( lResult == 2 )
                         gCfg.MoreOptions |= OPT_DEFAULT_AES;
                        else
                     if( lResult == 3 )
                         gCfg.MoreOptions |= OPT_DEFAULT_BLOWFISH;
                        else
                         gCfg.MoreOptions |= OPT_DEFAULT_XOR;

 
                   if( IsDlgButtonChecked( hDlg, IDC_AUTOBOOT_CHK ) )  
                       gCfg.Options |= OPT_AUTOBOOT;
                      else
                       gCfg.Options &= ~OPT_AUTOBOOT;

#if !defined(HOME_VER)

                   if( IsDlgButtonChecked( hDlg, IDC_CUSTABOUT_CHK ) )  
                       gCfg.Options |= OPT_CUSTABOUTBOX;
                      else
                       gCfg.Options &= ~OPT_CUSTABOUTBOX;

#endif

                   if( IsDlgButtonChecked( hDlg, IDC_NO_EXPORT_CHK ) )  
                       gCfg.Options |= OPT_NOEXPORT;
                      else
                       gCfg.Options &= ~OPT_NOEXPORT;


                   if( IsDlgButtonChecked( hDlg, IDC_RECENT_CHK ) )  
                       gCfg.Options |= OPT_CLEARRECENT;
                      else
                       gCfg.Options &= ~OPT_CLEARRECENT;


                   if( IsDlgButtonChecked( hDlg, IDC_PROTECT_CHK ) )  
                       gCfg.Options |= OPT_PASSPROTECT;
                      else
                       gCfg.Options &= ~OPT_PASSPROTECT;

#if !defined(HOME_VER)

                   if( IsDlgButtonChecked( hDlg, IDC_EULA_CHK ) )  
                       gCfg.Options |= OPT_EULA;
                      else
                       gCfg.Options &= ~OPT_EULA;

#endif


#if defined(ENTERPRISE_VER) || (FORENSIC_VER)

                   if( IsDlgButtonChecked( hDlg, IDC_ERASESECURE_CHK ) )  
                     {
                      char TmpStr[ 255 ];
                      gCfg.Options |= OPT_SECUREDELETE;

                      GetWindowText(GetDlgItem(hDlg, IDC_REPS_EDT), TmpStr,
                                    GetWindowTextLength(GetDlgItem(hDlg, IDC_REPS_EDT) ) + 1);

                      gCfg.NumReps = atoi( TmpStr );
                     } 
                    else
                     {
                      gCfg.Options &= ~OPT_SECUREDELETE;
                     }
#endif

                   }
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
 * Date:          12/17/2014  ( 11:52:36 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 11:52:36 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD GetTotalUsers( void )
{
  DWORD NumUsers = 0L;
  USER *User = gUserListHead;  

  if( ! User )
      return 0L;


  while( User )
  {
   NumUsers++;

   User = User ->Next;
  }

  return NumUsers; 
}







/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/23/2014  ( 3:56:33 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/23/2014  ( 3:56:33 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void AddToCreatedList( char *Filename )
{
  FILE *fp = NULL;
  char ListStr[ 255 ];

  if( ! Filename )
      return;

  sprintf( ListStr, "%s\\..\\CreatedFiles.lst", gCfg.CreateCDDir );

  if( ! ( fp = fopen( ListStr, "ab" ) ) )  // try to open for appending...
    { 
     // may not exist then, try creating the file outright...
     if( ! ( fp = fopen( ListStr, "wb" ) ) )
       {
        return; // could not add this to the list...
       } 

    }


  fprintf( fp, "%s\r\n", Filename );

  if( fp ) fclose( fp );

}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/23/2014  ( 3:56:26 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/23/2014  ( 3:56:26 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void KillOldCreateList( void )
{
  char ListStr[ 255 ];
  
  sprintf( ListStr, "%s\\..\\CreatedFiles.lst", gCfg.CreateCDDir );

  if( FileExists( ListStr ) )
      DeleteFile( ListStr );

}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/23/2014  ( 3:56:18 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/23/2014  ( 3:56:18 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         viewerout MUST point to the position in the file
 *                where you want the tree to be written.
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL AddCryptTreeToViewer( FILE *viewerout )
{
  CRYPT_ITEM  *TmpNode = NULL;
  DWORD res;


  if( ! viewerout )
        return FALSE;


  // Add Tree structure
  // ( each pointing to a .dat file ( the actual file it's encrypted in )... 
  DWORD CryptItemsSize = 0;
  DWORD NumCryptItems = 0;

  TmpNode = gCryptCDItemRoot; 
  while( TmpNode )
  {

   if( gStopCreate )
       return FALSE;
     

   // When adding a file or folder show it here in the Dialog box...
   if( TmpNode ->ItemName[ 0 ] )
      SetWindowText( gAddingToCDWnd, TmpNode ->ItemName );
     else
      SetWindowText( gAddingToCDWnd, " " );


   // Actually add this item to the viewer...
   if( ( res = AddItemToViewer( viewerout, TmpNode ) ) )
     {
      // add to the total size of the tree...
      CryptItemsSize += res;

      // add it to the total bytes added to the file...
      gAddedBytes += TmpNode ->Size;

      // Bump progress bar...
      SendMessage( ghCreateProg, PBM_SETPOS, ( WPARAM ) ( ( gAddedBytes / 1024 ) / 1024 ), ( LPARAM ) 0 ); 

      // add it to the total item count...
      NumCryptItems++;
     }

   TmpNode = TmpNode ->Next;  
  }


  gFooter.CryptItemsSize = CryptItemsSize;
  gFooter.NumCryptItems  = NumCryptItems;

  return TRUE;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/24/2014  ( 5:26:17 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/24/2014  ( 5:26:17 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddUsersToViewer( FILE *out )
{
  DWORD res = 0;
  DWORD NumUsers     = 0;
  DWORD NumUsersSize = 0;

  USER *TmpUserPtr = gUserListHead;

  gFooter.UsersSeekPos = ftell( out );

  while( TmpUserPtr )
       {
 
        if( res = AddUserToViewer( TmpUserPtr, out ) ) 
          {
           // only write users ( and add them ) if not 0 bytes!
           NumUsersSize += res;
           NumUsers++;
          }

        TmpUserPtr = TmpUserPtr ->Next;
       }

  gFooter.NumUsersSize   = NumUsersSize;   // Size of users array...
  gFooter.NumUsers       = NumUsers;
  
  return NumUsersSize;

}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/24/2014  ( 5:30:32 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/24/2014  ( 5:30:32 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddEulaToViewer( FILE *out )
{
  DWORD BytesWritten = 0;
  
  gFooter.EulaSeekPos = ftell( out );

  BytesWritten += WriteVarStr( gCfg.EulaText, out );

  SendMessage( ghCreateProg, PBM_SETPOS, ( WPARAM ) ( ( gAddedBytes / 1024 ) / 1024 ), ( LPARAM ) 0 ); 

  gFooter.EulaSize = BytesWritten;

  return BytesWritten;
}




/****************************************
 *                                       
 * Function: void LoadSettings( char *Filename )                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:37:50 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:37:50 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL LoadSettings( char *Filename )
{
  FILE *in = NULL; 

  if( ! ( in = fopen( Filename, "rb" ) ) )
      return false;

  memset( &gCfg, 0, sizeof( CFG ) );

  fread( &gCfg, 1, sizeof( CFG ) - sizeof( char *), in );


  memset( gCfg.Password, 0, 255 ); // <--- kill this or the customers WILL bitch

  gCfg.EulaText = NULL;

  if( in ) fclose( in );

  return true;
}








/****************************************
 *                                       
 * Function: SaveSettings()                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:37:50 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:37:50 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void SaveSettings( void )
{
  FILE *out = NULL; 

  char *p = GetAppDirectory();
  
  char WorkStr[ 255 ];
  sprintf( WorkStr, "%s\\%s", p, SETTINGS_FILENAME );

  if( ! ( out = fopen( WorkStr, "wb" ) ) )
      return;

  CFG cfg;

  memcpy( &cfg, &gCfg, ( sizeof( CFG ) - sizeof( char * ) ) );

  fwrite( &cfg, 1, ( sizeof( CFG ) - sizeof( char * ) ), out );

  if( out ) fclose( out );

}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:37:50 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:37:50 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddCfgToViewer( FILE *out )
{
  char Password[ 255 ];
  DWORD BytesWritten = sizeof( CFG ) - sizeof( char * );

  strcpy( Password, gCfg.Password );
  strcpy( gCfg.Password, ( char * ) EVAL_STRING ); // DONT put real password in viewer!!!

  // Encrypt EVAL_STRING with users password...
  PwEncrypt( Password, ( BYTE * ) &gCfg.Password[ 0 ], strlen( ( char * ) EVAL_STRING ) );

  gFooter.CfgSeekPos = ftell( out );

  fwrite( &gCfg, 1, BytesWritten, out );

  strcpy( gCfg.Password, Password );

  SendMessage( ghCreateProg, PBM_SETPOS, ( WPARAM ) ( ( gAddedBytes / 1024 ) / 1024 ), ( LPARAM ) 0 ); 

  gFooter.CfgSize = BytesWritten;


  return BytesWritten;
}
