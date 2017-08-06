
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:06:11 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:06:11 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL XORCopyEncrypt( FILE *Too, FILE *From, DWORD Size, CRYPT_ITEM *CryptItem  )
{
  BYTE *p = NULL;
  DWORD Parts, BytesLeft;
 
  if( ! (p = (BYTE *) CustAllocateMem( MAX_FILE_BUFFER + 100, "XORCopyEncrypt()", __LINE__ )))
    {
     OK_Msg("Error:",  "Cannot allocate memory for file!");
     return FALSE;
    }

 
  if( Size <= MAX_FILE_BUFFER ) // nope. will fit in 1 chunk...
    {
     fread(  p, 1, Size, From ); // read in the file to memory...

     Encrypt( gCfg.Password, p, Size ); // XOR this file /w password...

     fwrite( p, 1, Size, Too );// pack into the setup.exe file...
    }
   else  // File is > MAX_FILE_BUFFER... read/write in chunks...
     {
      Parts     = (Size / MAX_FILE_BUFFER );
      BytesLeft = (Size % MAX_FILE_BUFFER );

      while( Parts )
           {
            fread(  p, 1, MAX_FILE_BUFFER, From ); // read in the file to memory...

            Encrypt(gCfg.Password, p, MAX_FILE_BUFFER); // XOR this file /w password...

            fwrite( p, 1, MAX_FILE_BUFFER, Too );// pack into the setup.exe file...

            Parts--;
           }

      if( BytesLeft )
        {
         fread(  p, 1, BytesLeft, From ); // read in the file to memory...

         Encrypt(gCfg.Password, p, BytesLeft); // XOR this file /w password...

         fwrite( p, 1, BytesLeft, Too );// pack into the setup.exe file...
        }

    } // end else...


  CustFreeAllocatedMem( p,"XORCopyEncrypt()", __LINE__ );


  return TRUE;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:07:39 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:07:39 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL ScrambleCopyEncrypt( FILE *Too, FILE *From, DWORD Size, CRYPT_ITEM *CryptItem )
{
  BYTE *p = NULL;
  DWORD Parts, BytesLeft;
 
  if( ! (p = (BYTE *) CustAllocateMem( MAX_FILE_BUFFER + 100,  "ScrambleCopyEncrypt()", __LINE__ )))
    {
     OK_Msg("Error:",  "Cannot allocate memory for file!");
     return FALSE;
    }

 
  if( Size <= MAX_FILE_BUFFER ) // nope. will fit in 1 chunk...
    {
     fread(  p, 1, Size, From ); // read in the file to memory...

     ScrambleEncrypt( gCfg.Password, p, Size ); // XOR this file /w password...

     fwrite( p, 1, Size, Too );// pack into the setup.exe file...
    }
   else  // File is > MAX_FILE_BUFFER... read/write in chunks...
     {
      Parts     = (Size / MAX_FILE_BUFFER );
      BytesLeft = (Size % MAX_FILE_BUFFER );

      while( Parts )
           {
            fread(  p, 1, MAX_FILE_BUFFER, From ); // read in the file to memory...

            ScrambleEncrypt(gCfg.Password, p, MAX_FILE_BUFFER); // XOR this file /w password...

            fwrite( p, 1, MAX_FILE_BUFFER, Too );// pack into the setup.exe file...

            Parts--;
           }

      if( BytesLeft )
        {
         fread(  p, 1, BytesLeft, From ); // read in the file to memory...

         ScrambleEncrypt(gCfg.Password, p, BytesLeft); // XOR this file /w password...

         fwrite( p, 1, BytesLeft, Too );// pack into the setup.exe file...
        }

    } // end else...


  CustFreeAllocatedMem( p, "ScrambleCopyEncrypt()", __LINE__ );


  return TRUE;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/29/2014  ( 12:10:14 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/29/2014  ( 12:10:14 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CopyToDatNoEncrypt( FILE *Too, FILE *From, DWORD Size, CRYPT_ITEM *CryptItem )
{
  BYTE *p = NULL;
  DWORD Parts, BytesLeft;

  if( ! (p = (BYTE *) CustAllocateMem( MAX_FILE_BUFFER + 100,  "CopyToDatNoEncrypt()", __LINE__ )))
    {
     OK_Msg("Error:",  "Cannot allocate memory for file!");
     return FALSE;
    }

 
  if( Size <= MAX_FILE_BUFFER ) // nope. will fit in 1 chunk...
    {
     fread(  p, 1, Size, From ); // read in the file to memory...

     fwrite( p, 1, Size, Too );// pack into the setup.exe file...
    }
   else  // File is > MAX_FILE_BUFFER... read/write in chunks...
     {
      Parts     = ( Size / MAX_FILE_BUFFER );
      BytesLeft = ( Size % MAX_FILE_BUFFER );

      while( Parts )
           {
            fread(  p, 1, MAX_FILE_BUFFER, From ); // read in the file to memory...

            fwrite( p, 1, MAX_FILE_BUFFER, Too );// pack into the setup.exe file...

            Parts--;
           }

      if( BytesLeft )
        {
         fread(  p, 1, BytesLeft, From ); // read in the file to memory...

         fwrite( p, 1, BytesLeft, Too );// pack into the setup.exe file...
        }

    } // end else...


  CustFreeAllocatedMem( p, "CopyToDatNoEncrypt", __LINE__ );


  return TRUE;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:08:22 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:08:22 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ScrambleEncrypt( char *pw, unsigned char *EncData, long LenData )
{
  int  n = 0;
  long i = LenData;
  int pwlen = strlen(pw);
  DWORD Blocks    = ( LenData / SCRAMBLE_BLOCKSIZE );
  DWORD WhatsLeft = ( LenData % SCRAMBLE_BLOCKSIZE );
  BYTE *b = NULL;

  if(! LenData || ! pw) return; 

  b = EncData; // point to the data...

  while( Blocks-- ) // while blocks to encrypt...
  { 

   n = 0;
   while(n < pwlen) // go through each letter in the password...
   {

    for( int i = 0; i < pw[ n ]; i++);
        TranslateBlock( b );

    n++;
   }


   b += SCRAMBLE_BLOCKSIZE; // next block...
  }


  if( WhatsLeft ) // if there are a few bytes left just XOR encrypt them...
     Encrypt( pw, b, WhatsLeft );
 
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:08:54 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:08:54 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void Encrypt( char *pw, unsigned char *EncData, long LenData )
{
  int  n = 0;
  long i = LenData;
  int pwlen = strlen( pw );

  if(! LenData || ! pw) return; 


  n = 0;
  while(n < pwlen)
  {
   i = 0;
   while(i < LenData)  EncData[i++] ^= pw[n];
   n++;
  }

}


 


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:25:15 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:25:15 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL OnControl( int x, int y, CTRL_POS *Ctrl )
{

  if( (x >=  Ctrl ->x) &&
      (x <= (Ctrl ->x + Ctrl ->w)) &&
      (y >=  Ctrl ->y) &&
      (y <= (Ctrl ->y + Ctrl ->h)) )
        return TRUE;

  return FALSE;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:26:15 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:26:15 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// because windows API should have this but doesnt!
void InvalidateWindow( HWND hWnd )
{
  InvalidateRect( hWnd, NULL, TRUE );
  UpdateWindow( hWnd );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:27:33 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:27:33 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         DANGEROUS! make ONLY delete installed list of created files!                       
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Clear any old setup.exe, .dat etc. files...
// ( NOTE: Should make this a del *.* type funtion name ( like DelDirFiles(char *DirPath) ) )...
void ClearOldCDImage( void )
{ 

  FILE *fpCreatedFilesList = NULL;
  char ListStr[ 255 ];
  sprintf( ListStr, "%s\\..\\CreatedFiles.lst", gCfg.CreateCDDir );

  if( ! ( FileExists( ListStr ) ) ) // list of files to remove?
          return;                     

  FILE *in = NULL;

  if( ! ( in = fopen( ListStr, "rb" ) ) )
    { 
   	  return;
    }

  char WorkStr[ 255 ];
  while( fgets( WorkStr, 255, in ) ) // read in a line...
       {           
        // wipe the file...
        _unlink( WorkStr );
       }


  if( in ) fclose( in ); 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:29:47 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:29:47 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void EncryptViewer( char *Filename, DWORD ViewerSize )
{
  FILE *fp = NULL;
  DWORD NumChunks = 0;
  DWORD WhatsLeft = 0;
  DWORD FileSize  = 0;
  char Buff[ COPYBUFFER ]; 


  // open this file
  if( !( fp = fopen(Filename, "rb+") ) ) // MUST fseek before changing r & w modes...
    {
     OK_Msg("Error:",  "Cannot viewer for encrypting!");
     return;
    }

  // Get this files size ( add-on's too )...
  fseek( fp, 0L, SEEK_END ); 
  FileSize = ftell( fp );  

  // Seek past the viewer .exe
  fseek( fp, ViewerSize, SEEK_SET ); 

  FileSize -= ViewerSize; // adjust for skipping the viewer.exe...
  FileSize -= sizeof( FOOTER ); // DONT encrypt footer...
  
  NumChunks = ( FileSize / COPYBUFFER ); 
  WhatsLeft = ( FileSize % COPYBUFFER );

  while( NumChunks-- )
  {
   fread( &Buff, 1, COPYBUFFER, fp );
   Encrypt(  (char *) VIEWER_PASSWORD, (BYTE *) &Buff, COPYBUFFER );

   fseek( fp, (long)-(int)COPYBUFFER, SEEK_CUR ); // MUST do a fseek...( switch mode )
   fwrite( &Buff, 1, COPYBUFFER, fp );
  
   fseek( fp, 0, SEEK_CUR ); // over MUST do a fseek...( switch mode )
  }


  if( WhatsLeft )
    {
     fread( &Buff, 1, WhatsLeft, fp );
     Encrypt(  (char *) VIEWER_PASSWORD, (BYTE *) &Buff, WhatsLeft );

     fseek( fp, (long)-(int) WhatsLeft, SEEK_CUR ); // MUST do a fseek...( switch mode )
     fwrite( &Buff, 1, WhatsLeft, fp );
  
     fseek( fp, 0, SEEK_CUR ); // over MUST do a fseek...( switch mode )
    }


  if( fp ) fclose( fp );

}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:30:58 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:30:58 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddViewer( FILE *out )
{
  DWORD ViewerSize = 0L;
  char TmpStr[ 255 ];
  char Filename[ 255 ];
  char *Buff = NULL;
  FILE *in = NULL; 

  sprintf( Filename, "%s\\%s", gThisFilePath, VIEWERFILETOADD ); 

  // Save to a temp file... ( so we can check to make sure the size will work )
  sprintf( TmpStr, "%s\\%s", gThisFilePath, "cd.ico" );
  if( ! SUCCEEDED( SaveIconToFile( ghCustIcon, TmpStr ) ) )
    {
     OK_Msg("ERROR: (3464353)", "cannot write custom icon file. [%s]", TmpStr );
    }


  // From File...
  if( ! ( in = fopen( Filename, "rb" ) ) ) 
    {
     gStopCreate = TRUE; 
     OK_Msg("Error: (655432)", "Cannot locate needed .DLL ( %s )", Filename );
     return 0;
    }

  
  fseek( in, 0L, SEEK_END );
  ViewerSize = ftell( in );
  fseek( in, 0L, SEEK_SET );

  gAddedBytes += ViewerSize;


  // allocate the viewer file...
  if(!(Buff = (char *) CustAllocateMem( ViewerSize,  "AddViewer()", __LINE__ ) ))  
    {
     if( in ) fclose( in );
     OK_Msg("Error: (655432b)", "Cannot allocate needed .DLL ( %s )", Filename );
     return 0;
    }

  
  fread( Buff, 1, ViewerSize, in );
  if( in ) fclose( in );

  fwrite( Buff, 1, ViewerSize, out );

  CustFreeAllocatedMem( Buff,  "AddViewer()", __LINE__ );

  // setup the footer variable...
  gFooter.ViewerSize     = ViewerSize;

  SendMessage( ghCreateProg, PBM_SETPOS, (WPARAM) gAddedBytes, (LPARAM) 0 ); 

  return ViewerSize;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:31:40 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:31:40 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
HRESULT SaveIconIntoFile( DWORD  SeekPos, HICON hIcon, LPCTSTR lpFileName )
{
   IPicture* pPicture = NULL;
   IStream*  pStream  = NULL;
   PICTDESC  pict;
   HGLOBAL   hGlobal  = NULL;
   HRESULT   hr, hrIni;
   HANDLE    hFile;
   PVOID     pData;
   DWORD     dwWritten;
   LONG      lSize;

   if((!hIcon) || (!lpFileName))
      return(E_INVALIDARG);

   pict.cbSizeofstruct = sizeof(pict);
   pict.picType = PICTYPE_ICON;
   pict.icon.hicon = hIcon;

   hrIni = OleInitialize(NULL);

   if(SUCCEEDED((hr = OleCreatePictureIndirect(&pict, IID_IPicture, 
                                FALSE, (void**)&pPicture))))
   {
      if(SUCCEEDED((hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream))))
      {
         if(SUCCEEDED((hr = pPicture->SaveAsFile(pStream, TRUE, &lSize))))
         {
            GetHGlobalFromStream(pStream, &hGlobal);
            pData = GlobalLock(hGlobal);

            if(INVALID_HANDLE_VALUE != (hFile = CreateFile(lpFileName, GENERIC_WRITE, 
                            0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
              SetFilePointer( hFile, SeekPos, NULL, FILE_CURRENT);   

               hr = (WriteFile(hFile, pData, lSize, &dwWritten, NULL)) ? S_OK : 
                     HRESULT_FROM_WIN32(GetLastError());
               CloseHandle(hFile);
            }
            else
            {
               hr = HRESULT_FROM_WIN32(GetLastError());
            }

            GlobalUnlock(hGlobal);
         }
         pStream->Release();
      }
      pPicture->Release();
   }

   if(SUCCEEDED(hrIni))
   OleUninitialize();

   return(hr);
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:32:55 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:32:55 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
HRESULT SaveIconToFile( HICON hIcon, LPCTSTR lpFileName )
{
   IPicture* pPicture = NULL;
   IStream*  pStream  = NULL;
   PICTDESC  pict;
   HGLOBAL   hGlobal  = NULL;
   HRESULT   hr, hrIni;
   HANDLE    hFile;
   PVOID     pData;
   DWORD     dwWritten;
   LONG      lSize;

   if((!hIcon) || (!lpFileName))
      return(E_INVALIDARG);

   pict.cbSizeofstruct = sizeof(pict);
   pict.picType = PICTYPE_ICON;
   pict.icon.hicon = hIcon;

   hrIni = OleInitialize(NULL); 

   if(SUCCEEDED((hr = OleCreatePictureIndirect(&pict, IID_IPicture, 
                                FALSE, (void**)&pPicture))))
   {
      if(SUCCEEDED((hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream))))
      {

       if(SUCCEEDED((hr = pPicture->SaveAsFile(pStream, TRUE, &lSize))))
         {
            GetHGlobalFromStream(pStream, &hGlobal);
            pData = GlobalLock(hGlobal);

            if(INVALID_HANDLE_VALUE != (hFile = CreateFile(lpFileName, GENERIC_WRITE, 
                            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
            {
               hr = (WriteFile(hFile, pData, lSize, &dwWritten, NULL)) ? S_OK : 
                     HRESULT_FROM_WIN32(GetLastError());
               CloseHandle(hFile);
            }
            else
            {
               hr = HRESULT_FROM_WIN32(GetLastError());
            }

            GlobalUnlock(hGlobal);
         }
         pStream->Release();
      }
      pPicture->Release();
   }

   if(SUCCEEDED(hrIni))
   OleUninitialize();

   return(hr);
}






/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:37:02 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:37:02 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES: TODO: A) Add ID list for each node to the setup.exe file ( append )...                               
 *                                       
 * Returns: 0 on error... ( NUMBER OF BYTES ADDED OTHERWISE )                    
 *                                       
 ****************************************/
DWORD AddItemToViewer( FILE *ViewerOut, CRYPT_ITEM *CryptItem )
{
  FILE *DatOut = NULL;
  DWORD DatFileSize = 0, BytesWritten = 0;
  DWORD Len = 0;
  DWORD EncryptedSize = 0, OriginalSize = 0;
  char FileToAddName[ 255 ];
  char DatFilename[ 255 ];
  char DatFullPath[ 255 ];


  if( ! ViewerOut )             return 0;
  if( ! CryptItem )             return 0; 

  // initally set this size... will change is AES or Blowfish is used... ( bloat )
  OriginalSize = CryptItem ->Size;  // save for restoring later...


  // fix the path ( if NT may have a trailing backslash... )
  if( CryptItem ->ItemPath && CryptItem ->ItemName )
    {
     if( CryptItem ->ItemPath[ strlen( CryptItem ->ItemPath ) - 1 ] == '\\' )
         sprintf( FileToAddName, "%s%s", CryptItem ->ItemPath, CryptItem ->ItemName );
        else
         sprintf( FileToAddName, "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName );
    }




#ifdef FULL_VERSION

  // change CryptItem size HERE for AES file size!
  // ( AES will bloat... blowfish too )
  // NOTE: just getting the "will be" encrypted item size for later use
  //       in AddFileToDat()...
  if( CryptItem ->Flags & FLAG_ENC_AES )
    {
     if( ! ( GetAESEncryptedSize( CryptItem ) ) )   return 0;
    }

  if( CryptItem ->Flags & FLAG_ENC_BLOWFISH )
    {
     if( ! ( GetBlowFishEncryptedSize( CryptItem ) ) )  return 0;
    }

#endif 



  // will this item put it over the MAX dat size? if so, increment the dat filename number...
  if( ( gCurrDatSize + CryptItem ->Size ) > MAX_DAT_SIZE )
    {
     gCurrDatSize = 0;
     gCurrDatNum++;   
    }

  // add to this .dat's total size...
  gCurrDatSize += CryptItem ->Size;
  
  // reset total bytes written...
  BytesWritten = 0;

  
  // Add... item to treeview section of this file...
  fwrite( &CryptItem ->ID,           1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->ParentID,     1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->InfoType,     1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->Size,         1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->Attributes,   1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->Flags,        1, sizeof(DWORD), ViewerOut );
  fwrite( &CryptItem ->CaseNumber,   1, sizeof(DWORD), ViewerOut ); 
  fwrite( &CryptItem ->NumItemUsers, 1, sizeof(DWORD), ViewerOut ); 

  BytesWritten += sizeof( DWORD ) * 8;

  // Put in ItemName...
  BytesWritten += WriteVarStr( CryptItem ->ItemName, ViewerOut );

  // Put in ItemNotes... 
  BytesWritten += WriteVarStr( CryptItem ->ItemNotes, ViewerOut );    

  // Put in Item Suspect name... 
  BytesWritten += WriteVarStr( CryptItem ->SuspectName, ViewerOut );    


  // NumUsers is in the Cfg struct top DWORDS...
  USER_ID_LIST *TmpIdNode = CryptItem ->ID_List_Head;
  if( TmpIdNode )
    {
     DWORD ID = 0;
     while( TmpIdNode )
          {
           ID = TmpIdNode ->User_ID;
           fwrite( &ID, 1, sizeof( DWORD ), ViewerOut );
           BytesWritten += sizeof( DWORD );

           TmpIdNode = TmpIdNode ->Next;
          }
    }

  

  // If its a folder don't add it to a new .dat file... 
  // IF INFOTYPE == FOLDER THERE IS NO DAT FILENAME NEXT!!!
  // JUST SKIP ( LEN AND FILENAME ) IN READER -
  // ( and the seek pos and filesize ( DWORD ) * 2 )!
  if( ( CryptItem ->InfoType & ID_DRIVES_TREEVIEW ) ||
      ( CryptItem ->InfoType & FLAG_TREEROOT ) )
        return BytesWritten;   

  // ok, it's a file with a Path and a Name ( or it's an error! )
  if( ( ! CryptItem ->ItemPath ) || ( ! CryptItem ->ItemName ) )
    {
     OK_Msg( "Error: 2347621", "CryptItem name or path was NULL. Aborting." );
     return BytesWritten;   
    }


  // Add item to a .dat file ( if its a file )... ( otherwise no need to )... 
  sprintf( DatFilename, "%s%ld.%s", gCfg.CustDatName, gCurrDatNum, gCfg.CustDatExt );


  // Write the .dat filename string this item is in...
  BytesWritten += WriteVarStr( DatFilename, ViewerOut );    
 

  // Create or add to this .dat file...
  sprintf( DatFullPath, "%s\\%s", gCfg.CreateCDDir, DatFilename );

  // for delete list of files later...
  AddToCreatedList( DatFullPath );


  if( ! ( DatOut = fopen( DatFullPath, "rb+" ) ) ) // NOTE! MUST use fseek( fp, 0L, SEEK_CUR ); when switching from reading to writing in the r+ or w+ modes!
    {

     if( ! ( DatOut = fopen( DatFullPath, "wb" ) ) ) // File NOT created yet... create it..
       {        
        OK_Msg( "Error: (452345)", "Could not add %s to %s file!", CryptItem ->ItemName, DatFilename );
        return 0;
       }

    }

  fseek( DatOut, 0L, SEEK_END );
  DatFileSize = ftell( DatOut );


  // Add seek position to this item in this .dat file in viewer file...
  Len = DatFileSize;
  fwrite( &Len, 1, sizeof( DWORD ), ViewerOut );
  BytesWritten += sizeof( DWORD );


  // Add this file to the new .DAT file...
  if( CryptItem ->ItemPath[ strlen( CryptItem ->ItemPath ) - 1 ] == '\\' )
    sprintf( FileToAddName, "%s%s", CryptItem ->ItemPath, CryptItem ->ItemName );
   else
    sprintf( FileToAddName, "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName );


  if( ! AddFileToDat( DatOut, FileToAddName, CryptItem ) )
    {
     OK_Msg( "Error:(2345423)", "Cannot add %s to .dat file. ", FileToAddName );
     if( DatOut ) fclose( DatOut );
     return 0;
    }


  if( DatOut ) fclose( DatOut );


  // restore the cryptitem...
  CryptItem ->Size = OriginalSize;


  return BytesWritten;
}





/********************************************

  Function Name: 

  Params: 


  Notes: 

  Created:       12/11/2003   10:42:24 PM
  Last Modified: 12/11/2003   10:42:24 PM   By: Russ Greer

  Returns: 

 ********************************************/
char *GetAppDirectory( void )
{
  char *p = NULL;
  static char AppDir[ 255 ];

  memset( AppDir, 0, 255 );

  // Get this apps modual base dir...
  GetModuleFileName(NULL, (char *)AppDir, 255 );
  p = &AppDir[strlen(AppDir)-1];
 
  if( p )
  {

   while( p )
   {
    if( *p == '\\' )
      {
       *p = '\0';
        break;
      }

    p--;
   }

  }

  return ((char *) AppDir );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:48:08 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:48:08 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Convert the lame __TIME__ build constant into a
// useable time ( not military )
char *NowTime( void )
{
  static char now[ 25 ];
  char WorkStr[ 25 ];
  int ampm = 0;  // 0 == am, 1 == pm...
  int hour = 0, minute = 0, second = 0;

  memset( WorkStr, 0, 25 );
  strcpy( now, __TIME__ );

  // convert 1st part of time string into numric
  WorkStr[0] = now[0];
  WorkStr[1] = now[1];
  hour = atoi( WorkStr );  

  WorkStr[0] = now[3];
  WorkStr[1] = now[4];
  minute = atoi( WorkStr ); 

  WorkStr[0] = now[6];
  WorkStr[1] = now[7];
  second = atoi( WorkStr ); 

  if( hour >= 13 )
    { 
     ampm = 1;
     hour -= 12;  
    }
  
  sprintf( now, "%d:%.2d:%.2d %s", hour, minute, second, (ampm) ? "PM" : "AM" );

  return ((char *) now );
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:48:58 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:48:58 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *GetFileName(void)
{

 static OPENFILENAME ofn;
 static char Filter[255];
 static char TmpStr[1024];

 memset(Filter,0,255);

 strcpy(Filter, "All Files (*.*)");
 strcpy(&Filter[strlen(Filter)+1],"*.*");
  
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
 * Date:          12/17/2014  ( 4:49:50 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:49:50 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Gets a font ( /w font dlg box ) and returns HFONT handle to it...
// PASS: HWND of owner window...
HFONT GetFont( HWND hWnd )
{
   CHOOSEFONT cf;
   LOGFONT lf;
   char *p = NULL;

   InitCommonControls();

   GetObject( GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf ) ;

   // Initialize the LOGFONT structure and change the members as needed. 
   lf.lfHeight = -13 ;                           // change height
   lstrcpy(lf.lfFaceName,TEXT("Courier New")) ;  // change to courier new 

   cf.lStructSize    = sizeof (CHOOSEFONT) ;
   cf.hwndOwner      = hWnd ;
   cf.hDC            = NULL ;
   cf.lpLogFont      = &lf ;
   cf.iPointSize     = 0 ;
   cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS | CF_FORCEFONTEXIST;
   cf.rgbColors      = 0 ;
   cf.lCustData      = 0 ;
   cf.lpfnHook       = NULL ;
   cf.lpTemplateName = NULL ;
   cf.hInstance      = NULL ;
   cf.lpszStyle      = NULL ;
   cf.nFontType      = 0 ;
   cf.nSizeMin       = 0 ;
   cf.nSizeMax       = 0 ;

   ChooseFont(&cf);

   lf = *(cf.lpLogFont);
   strcpy( gCfg.FontName, lf.lfFaceName ); 

   memset( &gCfg.FontFileName[0], 0, 255 );

   if( (p = GetFontFileName( lf.lfFaceName )) )
           strcpy( gCfg.FontFileName, p );   
   
   gCfg.TextColor = cf.rgbColors;
   gCfg.FontSize  = cf.lpLogFont->lfHeight;

   return CreateFontIndirect(cf.lpLogFont);
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:49:52 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:49:52 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *GetFontFolder( void )
{
  HKEY Key;
  DWORD Size = 0;
  static char FontFolder[ 255 ];

  if(ERROR_SUCCESS != (RegOpenKeyEx(HKEY_CURRENT_USER, 
     "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_ALL_ACCESS, &Key))) 
     { 
      return NULL; 
     }

  Size = 255;
  RegQueryValueEx( Key, "Fonts", 0, NULL, (BYTE *) FontFolder, &Size );
  RegCloseKey( Key );  

  return ((char *) FontFolder );
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:50:07 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:50:07 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char *GetFontFileName( char *FaceName )
{
  HKEY Key;
  int Len = 0;
  DWORD dwName, dwValue;
  static char Name[ 255 ];
  char ValueName[ 255 ], Value[ 255 ];

  // Search through the registry and match the FaceName then
  // return the fontfilename.ext
  if(ERROR_SUCCESS != (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
     "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", 0, KEY_ALL_ACCESS, &Key))) 
     { 
      // FAILED! maybe its NT?
      if(ERROR_SUCCESS != (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
      "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_ALL_ACCESS, &Key))) 
        { 
         // nope... that didn't work either, just bail - it's not that important.
         OK_Msg("Error:(6AFG2)", "Cannot add custom font, try again or another font."); 
         return NULL;
        }

    }

    memset( &ValueName[0], 0, 255 );
    memset( &Value[0], 0, 255 );

    int i = 0;    
    while( 1 ) 
    {
     dwName  = 255;
     dwValue = 255;
     if( ERROR_NO_MORE_ITEMS == RegEnumValue( Key, i++, &ValueName[0], &dwName, NULL, NULL,
                                              (BYTE *) &Value[0], &dwValue) )
       {
        // end of fonts in registry...  
        break;
       }

     if( strstr( ValueName, FaceName ) )
       {
        strcpy( Name, Value );
        return ((char *) Name);
       }  

    }
   
  RegCloseKey( Key );

  memset( gTmpFontName, 0, 255 );

  // Make them choose it again from my custom dialog box...
  DialogBox( ghInst, MAKEINTRESOURCE( IDD_FASTCHOOSEFONT_DLG ),
                     NULL, FastChooseFontDlgProc );

   if( gTmpFontName[ 0 ] )
    {
     strcpy( Name, gTmpFontName );
     return Name;
    }

  return NULL;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:50:34 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:50:34 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL CALLBACK FastChooseFontDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 
   switch (uMsg)
   {   
 	case WM_INITDIALOG: 
                       {// gTmpFontName 
                         HKEY Key;
                         int Len = 0;
                         DWORD dwName, dwValue;
                         char ValueName[ 255 ], Value[ 255 ], TmpStr[ 255 ];

                         if(ERROR_SUCCESS != (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                           "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", 0, KEY_ALL_ACCESS, &Key))) 
                           {  
                            // FAILED! maybe its NT?
                            if(ERROR_SUCCESS != (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                               "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_ALL_ACCESS, &Key))) 
                              { 
                               // nope... that didn't work either, just bail - it's not that important.
                              OK_Msg("Error:(6AFG2)", "Cannot add custom font, try again or another font."); 
                              return TRUE;
                             }

                           }

                         memset( &ValueName[0], 0, 255 );
                         memset( &Value[0], 0, 255 );

                         int i = 0;    
                         while( 1 ) 
                         {
                          dwName  = 255;
                          dwValue = 255;
                          if( ERROR_NO_MORE_ITEMS == RegEnumValue( Key, i++, &ValueName[0], &dwName, NULL, NULL,
                                                                   (BYTE *) &Value[0], &dwValue) )
                            {
                             // end of fonts in registry...  
                             break;
                            }


                         sprintf( TmpStr, "%s  {%s}", ValueName, Value );
                         SendMessage( GetDlgItem( hDlg, IDC_FONT_LIST ), LB_INSERTSTRING, (WPARAM) -1, (LPARAM) TmpStr );
                        }
   
                       RegCloseKey( Key );

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
                   char TmpStr[ 255 ];
                   char *p = NULL, *p2 = NULL;

                   memset( TmpStr, 0, 255 );

                   int i = SendMessage( GetDlgItem( hDlg, IDC_FONT_LIST ), LB_GETCURSEL, 0,0 ); 
                   SendMessage( GetDlgItem( hDlg, IDC_FONT_LIST ), LB_GETTEXT, (WPARAM) i, (LPARAM) TmpStr ); 
                              
                   if( ! TmpStr[ 0 ] )
                     {
                      OK_Msg( "Error:", "You must choose a font name before clicking ok." );
                      break;
                     }

                   p = strchr( TmpStr, '{' );
                   p++;
                   p2 = strchr( TmpStr, '}' );
                   
                   i = ( p2 - p ); 
                   strncpy( gTmpFontName, p, i ); 

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
 * Pass:          NULL ( dummy ptr for new thread )    
 *
 * Desc:          Actually create the CD image...                    
 *                                       
 * Date:          12/17/2014  ( 5:02:10 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:59:00 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         .:::::TODO:::::.
 *                Changed from seek positions to just
 *                IN ORDER from viewer down... if seek pos's are needed by
 *                the viewer, set those globally and in the split up functions
 *                for clarity! Also, should place some "Magic" flag DWORD in
 *                each section to tell if the option has been used and it's actually
 *                in the file so the viewer will know about it.                       
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void Createit( void *p )
{
  DWORD res;

  FILE *out = NULL, *in = NULL;
  CRYPT_ITEM  *TmpNode = NULL;
  char Filename[ 255 ];
  char TmpStr[ 255 ]; 

  gAddedBytes = 0;     // reset...
  gStopCreate = FALSE;


  if( ! gNumFiles  ) // anything to do?
    {
     OK_Msg("INFO:", "No CD items to create CD with. Please add some folders or files and try again.");  
     return; // No list
    }

  // start the copy animation...
  if( _beginthread( &CopyAnim, 0, NULL ) == -1 )
    {
     OK_Msg( "ERROR:", "Failed to start copy anim." );
    } 

 
  // open new setup.exe file and add everything to it...
  if( ! FileExists( gCfg.CreateCDDir ) )
    {
     // create directory structure... ( its not there )...
     strcpy( TmpStr, gCfg.CreateCDDir );
     MakeDirs( TmpStr );
    } 
 

  // Clear Old files...
  if( (IDYES == YesNo_Msg("Clear out Burn directory?", "Clear old files from burn directory:\r\n\r\n%s\r\n\r\nbefore creating new CD image?", gCfg.CreateCDDir )) )
       ClearOldCDImage(); 
 

  // wipe out old created files list
  KillOldCreateList();
 
  // TooFile... ( the packed viewer )
  sprintf( Filename, "%s\\%s", gCfg.CreateCDDir, gCfg.CustViewerFilename );
  if(!( out = fopen( Filename, "wb"))) 
    {
     gStopCreate = TRUE;

     OK_Msg( "Error: (923451)", "Cannot create viewer (%s). Please try again.", Filename ); 
     return;
    }

 

  // for delete list of files later...
  AddToCreatedList( Filename );
 
  // Add viewer ( setup.exe )...
  if( ! ( res = AddViewer( out ) ) )
    {
     gStopCreate = TRUE;

     OK_Msg( "Error: (923452)", "Cannot create viewer. Please try again." ); 
     if( out ) fclose( out );
    }
 
  // add the crypt items tree...
  // out MUST point to the seek position where you want the tree to be written!
  if(! ( AddCryptTreeToViewer( out ) ) )
    {
     gStopCreate = TRUE;

     OK_Msg( "Error: (923452)", "Failed to create the tree list. Aborting." ); 
     if( out ) fclose( out );
     return;
    }

 
  // Add users ( if any )...
  gAddedBytes += AddUsersToViewer( out );

  // Add EULA...
  gAddedBytes += AddEulaToViewer( out );

  // Add  CFG ( decoy text, image filenames etc )...
  gAddedBytes += AddCfgToViewer( out );

  // add the bg image...
  gAddedBytes += AddBGImageToViewer( out );

  // add logo image...
  gAddedBytes += AddLogoImageToViewer( out );

  // Add About image ( if one )...
  gAddedBytes += AddAboutImageToViewer( out );
 
  // Add Font file ( if one )...
  gAddedBytes += AddFontToViewer( out );  

  // Write out the footer...
  AddFooterToViewer( out );

  // close the new setup.exe ( viewer file )...
  if( out ) fclose( out );
 
  // create the autoboot file... 
  if( gCfg.Options & OPT_AUTOBOOT )
      CreateAutoBootFile();
  
  // Encrypt from viewer down...
  EncryptViewer( Filename, gFooter.ViewerSize );

  // kill create encrypted image ( viewer ) animation...
  gStopCreate = TRUE;


  // Hide the create Dlg Box...  
  ShowWindow( gBurnWnd, SW_HIDE );

 
  // put up the "Created Dialog box" ( stats and info )...
  DialogBox( ghInst, MAKEINTRESOURCE( IDD_CREATED_DLG ),
             NULL,  CreatedDlgProc );

  // kill the burn window...
  DestroyWindow( gBurnWnd );

  // put back up the main window ( show it )...
  ShowWindow( ghMainWnd, SW_SHOW );

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:02:49 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:02:49 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Run copyanim in the dialog box as another thread...
void CopyAnim( void *Dmy )
{
  RECT r;

  r.left   = COPYANIM_X;
  r.top    = COPYANIM_Y;
  r.right  = r.left + COPYANIM_W;
  r.bottom = r.top  + COPYANIM_H;

  while( 1 )
  {
   if( gStopCreate )
      return;

   InvalidateRect( gBurnWnd, &r, FALSE );
   UpdateWindow( gBurnWnd );

   Sleep( 200 );
  }


}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:04:20 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:04:20 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         users are added directly AFTER the treeview items in the viewer file!                      
 *                Get working - THEN encrypt this data... easier to code it fast...   
 *                    
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddUserToViewer( USER *UserPtr, FILE *out )
{
  char TmpAdminPw[ 255 ];
  DWORD Size = 0L, Len = 0L, StructSize = 0L;


  if( ! UserPtr )
    {
     OK_Msg( "ERROR (3449sc):", "Cannot add user to CD/DVD image." );
     return 0L;
    }


  if( ( ! UserPtr ->Password ) || ( ! UserPtr ->UserName ) )
    {
     OK_Msg( "ERROR (34592sc):", "Cannot add user to CD/DVD image." );
     return 0L;
    }



  // add this user to the file...
  fwrite( &(UserPtr ->List_ID), 1, sizeof( DWORD ), out );
  Size += sizeof( DWORD );

  // user id
  fwrite( &(UserPtr ->User_ID), 1, sizeof( DWORD ), out );
  Size += sizeof( DWORD );

  // flags
  fwrite( &(UserPtr ->UserFlags), 1, sizeof( DWORD ), out );
  Size += sizeof( DWORD );

  // permissions...
  fwrite( &(UserPtr ->UserPermissions), 1, sizeof( DWORD ), out );
  Size += sizeof( DWORD );

  // expire date...
  fwrite( &(UserPtr ->ExpireDate), 1, sizeof( SYSTEMTIME ), out );
  Size += sizeof( SYSTEMTIME);

  // user name
  Size += WriteVarStr( UserPtr ->UserName, out );

  // password...   ( this is the password encryped with their password...
  Size += WriteVarStr( UserPtr ->Password, out );


  // Admin password ( main CD / DVD password - encrypted with this users password )
  Len = strlen( gCfg.Password );
  
  memset( TmpAdminPw, 0, 255 );
  memcpy( TmpAdminPw, gCfg.Password, Len );

  Encrypt( UserPtr ->Password, (BYTE *) TmpAdminPw, Len ); // encrypt admin pw with user pw...


  // write the encrypted password out ( may be 0's in it so use WriteVarStrLen()...
  Size += WriteVarStrLen( TmpAdminPw, Len, out );

  // user note
  Size += WriteVarStr( UserPtr ->UserNote, out );


  return Size;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:04:51 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:04:51 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void PwEncrypt( char *pw, unsigned char *EncData, long LenData )
{
  int  n = 0;
  long i = LenData;
  int pwlen = strlen( pw );

  if(! LenData || ! pw) return; 


  n = 0;
  while(n < pwlen)
  {
   i = 0;
 
   while(i < LenData)  EncData[i++] ^= ( pw[n] ) + n;

   n++;
  }

}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:05:12 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:05:12 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
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




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:13:19 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:13:19 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Loading in either BgImage or LogoImage...
DWORD AddImageToViewer( char *Filename, FILE *fpViewer )
{
  DWORD FileSize = 0;
  FILE *in = NULL;
  DWORD NumChunks = 0;
  DWORD WhatsLeft = 0;
  char Buff[ COPYBUFFER ]; 

  if( ! ( in = fopen( Filename, "rb" ) ) )
    {
      return 0;
    }

  // Get this files size...
  fseek( in, 0L, SEEK_END );
  FileSize = ftell( in );
  fseek( in, 0L, SEEK_SET );

  // write the size before the image so we know it's there in the viewer...
  fwrite( &FileSize, 1, sizeof( DWORD ), fpViewer );

  // Copy to the viewer...
  NumChunks = ( FileSize / COPYBUFFER ); 
  WhatsLeft = ( FileSize % COPYBUFFER );

  while( NumChunks-- )
  {
   fread(  &Buff, 1, COPYBUFFER, in );
   fwrite( &Buff, 1, COPYBUFFER, fpViewer );
  }


  if( WhatsLeft )
    {
     fread( &Buff, 1, WhatsLeft, in );
     fwrite( &Buff, 1, WhatsLeft, fpViewer );
    }


  if( in ) fclose( in );

  return FileSize;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:13:23 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:13:23 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Add the selected font to the viewer ( pack it in there )
DWORD AddFontFileToViewer( char *Filename, FILE *fpViewer )
{
  DWORD FileSize = 0;
  FILE *in = NULL;
  DWORD NumChunks = 0;
  DWORD WhatsLeft = 0;
  char Buff[ COPYBUFFER ]; 

  if( ! ( in = fopen( Filename, "rb" ) ) )
    {
     fwrite( &FileSize, 1, sizeof( DWORD ), fpViewer );
     return 0;
    }

  // Get this files size...
  fseek( in, 0L, SEEK_END );
  FileSize = ftell( in );
  fseek( in, 0L, SEEK_SET );

  fwrite( &FileSize, 1, sizeof( DWORD ), fpViewer );

  // Copy to the viewer...
  NumChunks = ( FileSize / COPYBUFFER ); 
  WhatsLeft = ( FileSize % COPYBUFFER );

  while( NumChunks-- )
  {
   fread(  &Buff, 1, COPYBUFFER, in );
   fwrite( &Buff, 1, COPYBUFFER, fpViewer );
  }


  if( WhatsLeft )
    {
     fread( &Buff, 1, WhatsLeft, in );
     fwrite( &Buff, 1, WhatsLeft, fpViewer );
    }


  if( in ) fclose( in );


  gFooter.FontSize = FileSize; 

  return FileSize;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 5:13:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 5:13:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void CreateAutoBootFile( void )
{
   FILE *out = NULL;
   char WorkStr[ 255 ];

  // create autoboot file...
  sprintf( WorkStr, "%s\\Autorun.inf", gCfg.CreateCDDir );

  if( !( out = fopen( WorkStr, "wb" ) ) )
    {
     OK_Msg("Error:",  "Cannot open autorun.inf file for writing!");
     return;
    }

  fprintf( out, "[autorun]\r\n" );
  fprintf( out, "open = %s\r\n", gCfg.CustViewerFilename );
  fprintf( out, "icon=cd.ico\r\n" );

  fclose( out );

  // for delete list of files later...
  AddToCreatedList( WorkStr );
 
}
