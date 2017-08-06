
/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:42:19 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:42:19 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddBGImageToViewer( FILE *out )
{
  DWORD BgImageSize = 0;

  gFooter.BgImageSeekPos = ftell( out );

  // Add BG image ( if one )...
  if( gCfg.Options & OPT_BGIMAGE )
    {  

     if( !( BgImageSize = AddImageToViewer( gCfg.BgFileName, out ) ) )
       {  
        // failed... just write "0" size...
        fwrite( &BgImageSize, 1, sizeof( DWORD ), out );
      
        OK_Msg("Error:(893452)", "Could not add background image to viewer!");
        return 0; 
       }  

     gFooter.BgImageSize = BgImageSize;

     return BgImageSize;
    }

  // not using a bg image... just write 0 size...
  fwrite( &BgImageSize, 1, sizeof( DWORD ), out );

  return 0;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:45:07 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:45:07 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddLogoImageToViewer( FILE *out )
{
  DWORD LogoImageSize = 0;

  gFooter.LogoImageSeekPos = ftell( out );

  // Add Logo image ( if one )...
  if( gCfg.Options & OPT_LOGOIMAGE )
    {
     if( ! ( LogoImageSize = AddImageToViewer( gCfg.LogoFileName, out ) ) )
       {
        // failed... just write "0" size...
        fwrite( &LogoImageSize, 1, sizeof( DWORD ), out );

        OK_Msg("Error:(893453)", "Could not add logo image to viewer!");
        return 0; 
       }  

     gFooter.LogoImageSize = LogoImageSize;

     return LogoImageSize;
    }


  // failed... just write "0" size...
  fwrite( &LogoImageSize, 1, sizeof( DWORD ), out );

  return 0;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:47:47 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:47:47 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddAboutImageToViewer( FILE *out )
{
  DWORD AboutImageSize = 0;

  gFooter.AboutImageSeekPos = ftell( out );

  // Add About image ( if one )...
  if( gCfg.Options & OPT_CUSTABOUTBOX )
    {

     if( gCustAboutImageBmp )
       {

        if( ! ( AboutImageSize = AddImageToViewer( gCustAboutImageFile, out ) ) )
          {
           // failed... just write "0" size...
           fwrite( &AboutImageSize, 1, sizeof( DWORD ), out );

           OK_Msg( "Error:(893453)", "Could not add custom about image to viewer!" );
           return 0; 
          }  

        gFooter.AboutImageSize = AboutImageSize;

        return AboutImageSize;
       }

    }


  // no image... just write "0" size...
  fwrite( &AboutImageSize, 1, sizeof( DWORD ), out );

  return 0;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 9:51:00 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 9:51:00 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD AddFontToViewer( FILE *out )
{
  DWORD FontFileSize = 0;

  gFooter.FontSeekPos = ftell( out );


  if( gCfg.MoreOptions & OPT_CUSTOMFONT )
    {  
     // Get this systems font directory...
     char *FontFolder = GetFontFolder();
     char  FontFilePathName[ 255 ];
     
     if( ! FontFolder )
       {    
        fwrite( &FontFileSize, 1, sizeof( DWORD ), out );

        OK_Msg( "Error:(893496A)", "Could not get font folder to retrieve custom font. Aborting." );
        return 0; 
       }
         
     sprintf( FontFilePathName, "%s\\%s", FontFolder, gCfg.FontFileName );

     if( ! ( FontFileSize = AddFontFileToViewer( FontFilePathName, out ) ) )
       {       
        fwrite( &FontFileSize, 1, sizeof( DWORD ), out );

        OK_Msg( "Error:(8934972B)", "Could not add %s custom font to viewer!", FontFilePathName );
        return 0; 
       }  

     return FontFileSize;
    }


  // not using a font, just write 0 size...
  fwrite( &FontFileSize, 1, sizeof( DWORD ), out );

  return 0;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/29/2014  ( 9:30:40 AM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/29/2014  ( 9:30:40 AM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void AddFooterToViewer( FILE *out )
{

  // Add footer...
  gFooter.CfgSize        = sizeof( CFG ) - sizeof( char * );
  gFooter.FooterSize     = sizeof( FOOTER );
  gFooter.FooterVersion  = VIEWER_FOOTER_SIG; // SIG...

  fwrite( &gFooter, 1, sizeof( FOOTER ), out );

}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 4:47:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 4:47:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         This is ONLY for NULL terminated strings, NOT encrypted text that may 
 *                have 0's ( NULL's ) within the string!...                       
 *                                                   
 *                Also, always returns AT LEAST size of DWORD because it ALWAYS
 *                writes AT LEAST the length even if its 0...
 *                        
 * Returns:       Number of bytes written to the file.                     
 *                                       
 ****************************************/
DWORD WriteVarStr( char *Str, FILE *out )
{
  DWORD Len = 0, BytesWritten = 0;

  if( ! Str )  // no string ptr... just write 0 for the length and return...
    {
     return ( DWORD ) fwrite( &Len, 1, sizeof( DWORD ), out );          
    }

  
  Len = strlen( Str );
  if( ! Len )  // no string length... just write 0 for the length and return...
    {
     return ( DWORD ) fwrite( &Len, 1, sizeof( DWORD ), out );          
    }
  

  // write the length and the string...
  BytesWritten  = ( DWORD ) fwrite( &Len, 1, sizeof( DWORD ), out );  
  BytesWritten += ( DWORD ) fwrite( Str, 1, Len, out );

  return BytesWritten;
}






/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/28/2014  ( 6:28:34 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/28/2014  ( 6:28:34 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         This is ONLY for NULL terminated strings, NOT encrypted text that may 
 *                have 0's ( NULL's ) within the string!...                       
 *                                       
 * Returns:       Read in string if it's there,
 *                NULL if it's 0 length or memallocation failed.                     
 *                                       
 ****************************************/
char *ReadVarStr( FILE *in )
{
  char *ReadString = NULL;
  DWORD Len = 0;

  // read the length and the string...
  fread( &Len, 1, sizeof( DWORD ), in );  

  if( ! Len )
      return NULL;

  if( ! ( ReadString = ( char * ) malloc( Len + 10 ) ) )
      return NULL;

  memset( ReadString, 0, Len + 5 );

  fread( ReadString, 1, Len, in );


  return ReadString;
}






/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/29/2014  ( 11:48:34 AM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/29/2014  ( 11:48:34 AM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:         Used for encrypted strings or data that MAY contain
 *                0's ( NULL's ) in its string...                       
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
DWORD WriteVarStrLen( char *Str, DWORD Len, FILE *out )
{
  DWORD BytesWritten = 0;

  if( ! Str )  // no string ptr... just write 0 for the length and return...
    {
     return ( DWORD ) fwrite( &BytesWritten, 1, sizeof( DWORD ), out );          
    }
 

  if( ! Len )  // no string length... just write 0 for the length and return...
    {
     return ( DWORD ) fwrite( &BytesWritten, 1, sizeof( DWORD ), out );          
    }
  

  // write the length and the string...
  BytesWritten  = ( DWORD ) fwrite( &Len, 1, sizeof( DWORD ), out );  
  BytesWritten += ( DWORD ) fwrite( Str, 1, Len, out );

  return BytesWritten;
}



char *AllocateString( char *Str )
{
  char *NewStr = NULL;

  if( ! Str ) return NULL;

  int len = strlen( Str ) + 10;

  if( ! ( NewStr = (char *) malloc( len ) ) )
      return NULL;

  memset( NewStr, 0, len );

  strcpy( NewStr, Str );

  return NewStr;
}




#ifdef FULL_VERSION

/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/29/2014  ( 5:59:13 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/29/2014  ( 5:59:13 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL GetAESEncryptedSize( CRYPT_ITEM *CryptItem )
{
  char OriginalFile[ 255 ];
  char NewEncryptedFile[ 255 ];
  char TmpDir[ 255 ];

  if( ! CryptItem ->ItemPath )   
    { 
      return TRUE;
    }
 

  // Get windows temp dir to copy too...
  if( ! ( GetTempPath( MAX_PATH, TmpDir ) ) )
    {
     // There might not be a TEMP directory. use the Windows directory if not
     if( ! ( GetWindowsDirectory( TmpDir, MAX_PATH ) ) )
            return FALSE;         
    }


  // have to get the encrypted file size before it gets added to the hive...
  sprintf( OriginalFile,     "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName ); 
  sprintf( NewEncryptedFile, "%s%s",   TmpDir, CryptItem ->ItemName );

  // encrypt the copied file to the new "encrypted" file...
  // NOTE: NewEncryptedFile is the "outfilename" for the encrypted result...
  if( ! ( InsomAES_Encrypt( gCfg.Password, OriginalFile, NewEncryptedFile ) ) )
          return FALSE;

  // Get the new items size...
  CryptItem ->Size = CryptGetFileSize( NewEncryptedFile );


  // wipe out the temp file... have the size now...
  DeleteFile( NewEncryptedFile ); 


  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          1/29/2014  ( 6:06:33 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 1/29/2014  ( 6:06:33 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL GetBlowFishEncryptedSize( CRYPT_ITEM *CryptItem )
{
  char OriginalFile[ 255 ];
  char NewEncryptedFile[ 255 ];
  char TmpDir[ 255 ];

  if( ! CryptItem ->ItemPath )  // it's a folder...  
    { 
      return TRUE;
    }

  // Get windows temp dir to copy too...
  if( ! ( GetTempPath( MAX_PATH, TmpDir ) ) )
    {
     // There might not be a TEMP directory. use the Windows directory if not
     if( ! ( GetWindowsDirectory( TmpDir, MAX_PATH ) ) )
             return FALSE;         
    }


  // have to get the encrypted file size before it gets added to the hive...
  sprintf( OriginalFile,     "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName ); 
  sprintf( NewEncryptedFile, "%s%s", TmpDir, CryptItem ->ItemName );

  // encrypt the copied file to the new "encrypted" file...
  if( ! ( InsomBlo_Encrypt( gCfg.Password, OriginalFile, NewEncryptedFile ) ) )
          return FALSE;

  // get the "will be" size...
  CryptItem ->Size = CryptGetFileSize( NewEncryptedFile );

  // wipe out the temp file... have the size now...
  DeleteFile( NewEncryptedFile ); 


  return TRUE;
}

#endif



DWORD GetFolderSize( char *pstrFolderPath, bool blnSubFolder )
{
    WIN32_FIND_DATA Win32FindData;
    HFILE hFindFile;
    BOOL blnFind = true;
    char strFileName[260];
     ULONG ulFileSize = 0;
     char strFolderPath[260];

     strcpy(strFolderPath, pstrFolderPath);
     int strLen = strlen(strFolderPath);
 

     if ( strFolderPath[strLen - 1] !=  '\\' )
          strcat(strFolderPath, "\\");

     char strWild[260];
     sprintf(strWild, "%s*.*", strFolderPath);

    
    hFindFile = (HFILE)FindFirstFile(strWild, &Win32FindData);
    
    while ( blnFind )
     {
        strcpy(strFileName, Win32FindData.cFileName);
        
        if ( strcmp(strFileName, ".") && strcmp(strFileName, "..") )
          {
            if ( Win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
               {
                if ( blnSubFolder )
                    {
                         char strTmp[260];
                         sprintf(strTmp, "%s%s\\", strFolderPath, strFileName);
                         ulFileSize += GetFolderSize(strTmp, blnSubFolder);
                    }
               }
            else
               {
                ulFileSize += Win32FindData.nFileSizeLow;
               }     
          }

        
        blnFind = FindNextFile((void*)hFindFile, &Win32FindData);
    }
    

    FindClose((void *)hFindFile);
    
     return ulFileSize;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          5/22/2014  ( 5:45:26 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 5/22/2014  ( 5:45:26 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void SetSubFolderFlags( DWORD CryptItemID, DWORD EncType )
{

  // go through all sub folders and files and set these flags...
  CRYPT_ITEM  *TmpNode = NULL, *WorkNode = NULL;

  if(! gCryptCDItemRoot ) 
    {
       OK_Msg("ERROR: 90723", "No items to set.");
       return; // No CryptItems linked list - bail.
    }


  DWORD ClearFlags = FLAG_ENC_XOR      |
                     FLAG_ENC_PGP      |
                     FLAG_ENC_SCRAMBLE |
                     FLAG_ENC_BLOWFISH |
                     FLAG_ENC_2FISH    |
                     FLAG_ENC_AES;

  if( !( WorkNode = GetCryptItemFromID( CryptItemID ) ) )
     return;

  // Clear ALL other encrypt type flags
  WorkNode ->Flags &= ~ClearFlags;


  // Set this items crypt type to FLAG_ENC_SCRAMBLE
  if( EncType == IDM_SCRAMBLE )
      WorkNode ->Flags |= FLAG_ENC_SCRAMBLE;

#if ! defined( HOME_VER )

  else
  if( EncType == IDM_AES )
      WorkNode ->Flags |= FLAG_ENC_AES;

  #if defined(FOREN_VER) || (ENTERPRISE_VER)

  else
  if( EncType == IDM_BLOWFISH )
      WorkNode ->Flags |= FLAG_ENC_BLOWFISH;
  #endif
#endif

  else
  if( EncType == IDM_XOR )
      WorkNode ->Flags |= FLAG_ENC_XOR;
  else
  if( EncType == IDM_FULL )
      WorkNode ->Flags |= FLAG_ENC_FULL;
  else
  if( EncType == IDM_FAST )
      WorkNode ->Flags &= ~FLAG_ENC_FULL;
  

  WorkNode ->Flags |= FLAG_ENCRYPTED;


  // go through each node, 
  // if the node had CryptItemID as it's parent node, set the flags.
  WorkNode = gCryptCDItemRoot;
  while( WorkNode )
  {
   
   if( WorkNode ->ParentID == CryptItemID )
     {
                         
      // Clear ALL other encrypt type flags
      WorkNode ->Flags &= ~ClearFlags;


      // Set this items crypt type to FLAG_ENC_SCRAMBLE
      if( EncType == IDM_SCRAMBLE )
          WorkNode ->Flags |= FLAG_ENC_SCRAMBLE;

#if ! defined( HOME_VER )

  else
  if( EncType == IDM_AES )
      WorkNode ->Flags |= FLAG_ENC_AES;

  #if defined(FOREN_VER) || (ENTERPRISE_VER)

  else
  if( EncType == IDM_BLOWFISH )
      WorkNode ->Flags |= FLAG_ENC_BLOWFISH;
  #endif
#endif

      else
      if( EncType == IDM_XOR )
          WorkNode ->Flags |= FLAG_ENC_XOR;
      else
      if( EncType == IDM_FULL )
          WorkNode ->Flags |= FLAG_ENC_FULL;
      else
      if( EncType == IDM_FAST )
          WorkNode ->Flags &= ~FLAG_ENC_FULL;
  

      WorkNode ->Flags |= FLAG_ENCRYPTED;


      // recursively set sub files and folders too.
      SetSubFolderFlags( WorkNode ->ID, EncType );
     }
    

   WorkNode = WorkNode ->Next;
  }


}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          8/15/2014  ( 9:34:18 AM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 8/15/2014  ( 9:34:18 AM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
CRYPT_ITEM *GetCryptItemFromID( DWORD CryptItemID )
{
  CRYPT_ITEM *WorkNode = NULL;

  WorkNode = gCryptCDItemRoot;
  while( WorkNode )
  {
   
   if( WorkNode ->ID == CryptItemID )
       return WorkNode;
 
   WorkNode = WorkNode ->Next;
  }  

  return NULL;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          5/22/2014  ( 10:10:22 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 5/22/2014  ( 10:10:22 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void RecursiveSetUser( DWORD CryptItemID, DWORD UserID )
{

  // go through all sub folders and files and set these flags...
  CRYPT_ITEM  *TmpNode = NULL, *WorkNode = NULL;

  if(! gCryptCDItemRoot ) 
       return; // No CryptItems linked list - bail.


  // go through each node, 
  // if the node had CryptItemID as it's parent node, set the flags.
  WorkNode = gCryptCDItemRoot;
  while( WorkNode )
  {
   
   if( WorkNode ->ParentID == CryptItemID )
     {
       AddUserIDToCryptItem( WorkNode, UserID );

       // recursively set sub files and folders too.
       RecursiveSetUser( WorkNode ->ID, UserID );
     }
    

   WorkNode = WorkNode ->Next;
  }


}





void DebugWriteString( char *Str )
{
  FILE *out = NULL;

  if( ! Str ) 
       { 
         OK_Msg( "DEBUG:", "passed string was NULL in DebugWriteString()" );  
         return;
       }

  if( ! FileExists( "c:\\ccddebugdump.txt" ) )
    {
     if( ! ( out = fopen( "c:\\ccddebugdump.txt", "wb" ) ) )
       { 
         OK_Msg( "", "Cannot open debug file ( 1 )" );  
         return;
       }
    }
   else
    {
     if( ! ( out = fopen( "c:\\ccddebugdump.txt", "ab" ) ) )
       { 
         OK_Msg( "", "Cannot open debug file ( 2 )" );  
         return;
       }
    }

  
  fwrite( Str, 1, strlen( Str ), out );


  if( out ) fclose( out ); 

}



