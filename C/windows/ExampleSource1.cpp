/****************************************
 *                                       
 * File:   commonutil.cpp                          
 *                                       
 * Description:   common functions not really specific to any one part of the program
 *                 
 *                                       
 * Date:          12/17/2014  ( 2:17:55 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:17:55 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                  
 *                                       
 * ( c ) 2002-2014 InsomTech --- all rights reserved 
 *                                       
 ****************************************/

#include "..\\include\\commonutil.hpp"

 

/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:17:52 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:17:52 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void OK_Msg(char *Title, char *Format, ...)
{
  char TmpBuff[1024];
  char *pArgs = (char *) &Format + sizeof(Format);

  vsprintf(TmpBuff, Format, pArgs); 
 
  MessageBox( NULL,
              TmpBuff,
              Title, MB_OK | MB_SYSTEMMODAL | MB_ICONQUESTION ); 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:17:45 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:17:45 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL HasSubFolders( char *FolderFullPath )
{
  HANDLE file;
  WIN32_FIND_DATA data;

  file = FindFirstFile( FolderFullPath, &data );
  FindNextFile( file, &data );
  while( FindNextFile( file, &data) )
       {
    
		if( ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
          { 
           FindClose( file );
           return TRUE;
          }

       }

  FindClose( file );

  return FALSE;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:20:00 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:20:00 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*
 DWORD ParentID;     // This items parent ( 0 is CD root )
 DWORD InfoType;     // is it a file or Directory...FLAG_TREEROOT
 DWORD Size;         // filesize or dir 0 for size...
 DWORD Attributes;   // file or dir attributes...
 DWORD Flags;        // Encrypted, Crypt Type, etc.
 char  *ItemName;  // filename or dir name
*/
LPARAM AddCryptItem( DWORD ParentID, DWORD ItemType,
                     DWORD Size, DWORD Attribs,
                     DWORD Flags, char *ItemText, char *ItemPath ) 
{
  static DWORD ID = 1;
  CRYPT_ITEM *NewItem = NULL;
 

  if(! ItemText )
    { 
     OK_Msg("Error: (3123)", "Cannot add cryptcd item. ItemText was NULL.");
     return NULL;
    }
 

  if(!(NewItem = (CRYPT_ITEM *) CustAllocateMem( sizeof(CRYPT_ITEM)+100, "AddCryptItem()", __LINE__ )) )
    { 
     OK_Msg("Error: (3423)", "Cannot add cryptcd item.");
     return NULL;
    } 

  memset( NewItem, 0, sizeof( CRYPT_ITEM ) + 50);
 
  NewItem ->Attributes = Attribs;
  NewItem ->CaseNumber = -1;      // -1 == NONE
  NewItem ->Flags = Flags;
  NewItem ->InfoType = ItemType;
  NewItem ->ParentID = ParentID;
  NewItem ->Size = Size;
  NewItem ->ItemName = NULL;
  NewItem ->ItemPath = NULL;
  NewItem ->ItemNotes = NULL;
  NewItem ->SuspectName = NULL;
  NewItem ->ID_List_Head = NULL;
  NewItem ->Last = GetLastCryptNode(); 
  NewItem ->Next = NULL;


  if( ItemText && ItemPath )
    {
     int Len = strlen( ItemText );
     char *p = NULL;

     if( Len )
       {
    
        if( ( p = (char *) CustAllocateMem( Len + 10,  "AddCryptItem()", __LINE__ ) ) )
          {   
           strcpy( p, ItemText );
           NewItem ->ItemName = p;
          }

       }

     // new... only add path if it's a file!
     if( (ItemType != ID_DRIVES_TREEVIEW)  &&
         (ItemType != FLAG_TREEROOT) )
       {
        Len = strlen( ItemPath );
        p = NULL;

        if( Len )
          {
    
           if( ( p = (char *) CustAllocateMem( Len + 10,  "AddCryptItem()", __LINE__ ) ) )
             {   
              strcpy( p, ItemPath );
              NewItem ->ItemPath = p;
             }
          }
       }

    }


  NewItem ->ID = ++ID;

  // Hook up to previous item...
  if( NewItem ->Last ) // if not ROOT node
      NewItem ->Last ->Next = NewItem;


   // add it's size etc...
   if( ItemType == ID_DRIVES_TREEVIEW )
       gNumFolders++;
      else if( ItemType == ID_FILE_LISTVIEW )
       {
        gNumFiles++;
        gTotalBytes += Size;
       }
 
  UpdateStatusBar();

  return ( (LPARAM) NewItem );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:22:04 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:22:04 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
CRYPT_ITEM *GetLastCryptNode( void )
{
  CRYPT_ITEM *CryptItem = NULL;

  CryptItem = gCryptCDItemRoot;

  if(! CryptItem )
    {
     // No list ( or items ) yet...
     return NULL;
    }

  while( CryptItem ->Next )
  {
   CryptItem = CryptItem ->Next ;
  }

  return CryptItem;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:22:55 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:22:55 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void UpdateStatusBar( void )
{
  char str[ 512 ];

  long double UsedMB = 0;//DWORD

  if( gTotalBytes >= ( 1024 * 1024 ) )
      UsedMB = ((long double)((long double) gTotalBytes / 1024 ) / 1024);


  sprintf( str, "%I64Ld Bytes : %0.2I64f KB : %0.2I64f MB : %0.2I64f GB : %I64Ld Folders : %I64Ld Files",
                 gTotalBytes,                     // bytes
                 ((long double)gTotalBytes/1024), // KB
                 UsedMB,                          // MB
                 ((long double)UsedMB/1024),      // GB
                 gNumFolders, gNumFiles );  

  SetWindowText( ghStatusWnd, str );

}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:31:08 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:31:08 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// from: itemname/subdir/subdir/subdir/C:
// to:   C:/subdir/subdir/subdir/itemname
char *ReversePath( char *Path )
{
  int Len = 0;
  char *p = NULL;
  static char RevPath[ MAX_PATH ];

  memset( RevPath, 0, MAX_PATH );

  if( ! Path )  return NULL;

  if( Path[strlen( Path )-1] == '\\' ) 
      Path[strlen( Path )-1] = '\0';

  p = Path; 
  while( p = strrchr( p, '\\' ) ) //   name//sub//sub//c: 
  {
   strcat( RevPath, p+1 );
   strcat( RevPath, "\\" );
   *p = '\0';
    p = Path;
  }

  strcat( RevPath, Path );

  return ((char *) RevPath );
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:34:32 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:34:32 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ClearFilesLinkList( void )
{
  LIST_ITEM  *TmpNode = NULL, *WorkNode = NULL;

  if(! gpListItem) 
       return; // No list

  WorkNode = gpListItem;
  while( WorkNode )
  {
   TmpNode = WorkNode; 

   WorkNode = WorkNode ->Next;

   if( TmpNode ->FileName )
     {
      CustFreeAllocatedMem( TmpNode ->FileName, "ClearFilesLinkList()", __LINE__ );
      TmpNode ->FileName = NULL;
     }

   if( TmpNode ->FilePath )
     {
      CustFreeAllocatedMem( TmpNode ->FilePath, "ClearFilesLinkList()", __LINE__ );
      TmpNode ->FilePath = NULL;
     }

   CustFreeAllocatedMem( TmpNode, "ClearFilesLinkList()", __LINE__ );
  }

  gpListItem = NULL;

  gNumListFiles = 0;

}







/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:35:26 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:35:26 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL AddFilesListNode( char *FileName, char *Path, long FileSize, DWORD Flags )
{
  DWORD ID = 0;
  LIST_ITEM  *TmpNode = NULL;

  TmpNode = gpListItem;

  if( ! TmpNode ) // creating 1st node in list... 
    {

     if( ! ( TmpNode = ( LIST_ITEM * ) CustAllocateMem( sizeof(LIST_ITEM), "AddFilesListNode()", __LINE__ )   )) 
       {
        OK_Msg("ERROR: 70", "Cannot allocate LIST_ITEM (root)" );
        return FALSE; 
       }

     if(!( TmpNode ->FileName = ( char * ) CustAllocateMem( strlen(FileName)+10, "AddFilesListNode()", __LINE__ )))
       {
        CustFreeAllocatedMem( TmpNode, "AddFilesListNode()", __LINE__ );
        CustFreeAllocatedMem( gpListItem, "AddFilesListNode()", __LINE__ );
        gpListItem = NULL;

        OK_Msg("ERROR: 71", "Cannot allocate LIST_ITEM (root)->FileName" );
        return FALSE; 
       } 

     if(!( TmpNode ->FilePath = ( char * ) CustAllocateMem( strlen(Path)+10, "AddFilesListNode()", __LINE__ )))
       {
        CustFreeAllocatedMem( TmpNode ->FileName, "AddFilesListNode()", __LINE__ );
        CustFreeAllocatedMem( TmpNode, "AddFilesListNode()", __LINE__ );
        CustFreeAllocatedMem( gpListItem, "AddFilesListNode()", __LINE__ );
        gpListItem = NULL;

        OK_Msg("ERROR: 72", "Cannot allocate LIST_ITEM (root)->Path" );
        return FALSE; // FAILED...
       } 

     strcpy( TmpNode ->FileName, FileName ); 
     strcpy( TmpNode ->FilePath, Path ); 
     TmpNode ->ID = 0; // 0 for starting node...
     TmpNode ->Flags = Flags;
     TmpNode ->FileSize = FileSize;
     TmpNode ->Last = NULL;
     TmpNode ->Next = NULL;

     gpListItem = TmpNode;
    }
   else
    {

     while( TmpNode ->Next ) // go to last node 
            TmpNode = TmpNode ->Next;  

     if(!( TmpNode ->Next = ( LIST_ITEM * ) CustAllocateMem( sizeof(LIST_ITEM), "AddFilesListNode()", __LINE__ )))
       {
        OK_Msg("ERROR: 73", "Cannot allocate LIST_ITEM ->Next " );
        return FALSE; 
       }

     ID = TmpNode ->ID + 1;
     TmpNode ->Next ->Last = TmpNode;
     TmpNode = TmpNode ->Next;

     if(!( TmpNode ->FileName = ( char * ) CustAllocateMem( strlen(FileName)+10, "AddFilesListNode()", __LINE__ )))
       {
        CustFreeAllocatedMem( TmpNode, "AddFilesListNode()", __LINE__ );

        OK_Msg("ERROR: 74", "Cannot allocate LIST_ITEM ->FileName " );
        return FALSE; 
       } 

     if(!( TmpNode ->FilePath = ( char * ) CustAllocateMem( strlen(Path)+10, "AddFilesListNode()", __LINE__ )))
       {
        CustFreeAllocatedMem( TmpNode ->FileName, "AddFilesListNode()", __LINE__ );
        CustFreeAllocatedMem( TmpNode, "AddFilesListNode()", __LINE__ );

        OK_Msg("ERROR: 75", "Cannot allocate LIST_ITEM ->Path" );
        return FALSE;
       } 

     strcpy( TmpNode ->FileName, FileName ); 
     strcpy( TmpNode ->FilePath, Path ); 
     TmpNode ->ID = ID; // Last nodes ID + 1...
     TmpNode ->Flags = Flags;
     TmpNode ->FileSize = FileSize;
     TmpNode ->Next = NULL;
    }

  gNumListFiles++;

  return TRUE;
}







/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:41:27 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:41:27 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

  Pass: SourceID, // treeview ( folders) or list view ( files )
        Name  // item text ( multiple calls to this function if more items )    
        FullPath // c:/sub/sub/filename.txt ( includes filename! )

*/
BOOL AddToDragInfo( DWORD SourceID, char *Name )
{
  static DWORD ID = 0;
  CRYPTDRAGINFO *TmpNode = NULL;
  char FullPath[ MAX_PATH ];

  if( ! gCurrBrowseDir[ 0 ] )
        strcpy( gCurrBrowseDir, "c:\\" );

  sprintf( FullPath, "%s\\%s", gCurrBrowseDir, Name );
 
  if( ! gDragInfoRoot ) // Creating Root node of drag items...
  {

   if(!( TmpNode = ( CRYPTDRAGINFO * ) CustAllocateMem( sizeof(CRYPTDRAGINFO), "AddToDragInfo()", __LINE__ )))
       {
        OK_Msg("ERROR: 100", "Cannot allocate CRYPTDRAGINFO (root)" );
        return FALSE; 
       } 

   memset( TmpNode, 0, sizeof(CRYPTDRAGINFO));

   TmpNode ->Attributes = GetFileAttributes( FullPath );
   TmpNode ->Size = CryptGetFileSize( FullPath ); // Will open and close the file...
   TmpNode ->ID = ID;
   TmpNode ->InfoType = SourceID;
   strcpy( TmpNode ->CurrentDirectory, gCurrBrowseDir );
   strcpy( TmpNode ->Name, Name );

   TmpNode ->Last = NULL;
   TmpNode ->Next = NULL;

   gDragInfoRoot = TmpNode;

   return TRUE;
  }
 else // adding item to end of the list
  {
   // find the end of the list...
   TmpNode = gDragInfoRoot;

   while( TmpNode ->Next ) TmpNode = TmpNode ->Next;
   
   if(!( TmpNode ->Next = ( CRYPTDRAGINFO * ) CustAllocateMem( sizeof(CRYPTDRAGINFO),  "AddToDragInfo()", __LINE__ )))
       {
        OK_Msg("ERROR: 105", "Cannot allocate CRYPTDRAGINFO item" );
        return FALSE; 
       } 

   memset( TmpNode ->Next, 0, sizeof(CRYPTDRAGINFO));

   TmpNode ->Next ->Last = TmpNode;
   TmpNode = TmpNode ->Next;

   TmpNode ->Attributes = GetFileAttributes( FullPath );
   TmpNode ->Size = CryptGetFileSize( FullPath ); // Will open and close the file...
   TmpNode ->ID = ++ID;
   TmpNode ->InfoType = SourceID;
   strcpy( TmpNode ->CurrentDirectory, gCurrBrowseDir );
   strcpy( TmpNode ->Name, Name );
   TmpNode ->Next = NULL;

   return TRUE;
  }

  return FALSE;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:42:14 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:42:14 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*
  NOTE! Change to use ONLY FullPaths!

*/
DWORD CryptGetFileSize( char *FileName )
{
  FILE *in = NULL;
  DWORD Size = 0L;
 

  if( GetFileAttributes( FileName ) & FILE_ATTRIBUTE_DIRECTORY ) 
    {
     return GetDiskFolderSize( FileName );
    }


  if( !(in = fopen( FileName, "rb")))
    {
     OK_Msg( "ERROR:", "Could not open \"%s\" in CryptGetFileSize();", FileName );
     return 0;
    }


  fseek( in, 0L, SEEK_END );
  Size = (DWORD) (long) ftell( in );
  fclose(  in );

  return Size;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:42:58 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:42:58 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Count total of all file byte size(s) in this folder
// and return that number...
DWORD GetDiskFolderSize( char *szDir )
{
   DWORD lSize = 0;
   WIN32_FIND_DATA FileData; 
   HANDLE hSearch = NULL; 
   char szPath[ 255 ];     

   strcpy( szPath, szDir );

   if( szPath[ strlen(szPath) -1 ] != '\\' )
       strcat( szPath, "\\" );

   strcat( szPath, "*.*" );

   hSearch = FindFirstFile( szPath, &FileData); 
   if(INVALID_HANDLE_VALUE != hSearch) 
   { 
        while(FindNextFile(hSearch, &FileData)) 
        { 

         //Search for . and .. special files
         if( FileData.cFileName[ 0 ] == '.' )
           {
            continue;
           }

         //Check if it is directory
         if(FILE_ATTRIBUTE_DIRECTORY == FileData.dwFileAttributes)
           {
            strcpy(szPath, szDir);
            strcat(szPath, "\\");
            strcat(szPath, FileData.cFileName);
            strcat(szPath, "\\");

            lSize += GetDiskFolderSize(szPath);//Recursive call
           }
          else
           {
            lSize = lSize + FileData.nFileSizeLow;
           }


        }//End of while 

   }//End of if(hSearch == INVALID_HANDLE_VALUE)
      
  //Close the search handle. 
  FindClose(hSearch);

  return lSize;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:47:04 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:47:04 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
LIST_ITEM *FindListItem( DWORD ID )
{
  LIST_ITEM *li;

  li = gpListItem;
  while( li )
  {
   if( li ->ID == ID ) 
     {
      return ((LIST_ITEM *) li ); 
     }

   li = li ->Next;
  }

  return NULL;
}






/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:49:05 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:49:05 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ClearCDTreeList( void )
{
  CRYPT_ITEM  *TmpNode = NULL, *WorkNode = NULL;

  if(! gCryptCDItemRoot ) 
       return; // No list - no files


  WorkNode = gCryptCDItemRoot;
  while( WorkNode )
  {
   TmpNode = WorkNode; 

   WorkNode = WorkNode ->Next;

   if( TmpNode ->ItemName )
       CustFreeAllocatedMem( TmpNode ->ItemName, "ClearCDTreeList()", __LINE__ );

   if( TmpNode ->ItemPath )
       CustFreeAllocatedMem( TmpNode ->ItemPath, "ClearCDTreeList()", __LINE__ );

   if( TmpNode ->ItemNotes )
       CustFreeAllocatedMem( TmpNode ->ItemNotes, "ClearCDTreeList()", __LINE__ );

   if( TmpNode ->SuspectName )
       CustFreeAllocatedMem( TmpNode ->SuspectName, "ClearCDTreeList()", __LINE__ );

   if( TmpNode ->ID_List_Head )
     {
      USER_ID_LIST *TmpListID = TmpNode ->ID_List_Head;

      while( TmpListID )
           {
            USER_ID_LIST *TmpDelNode = TmpListID;
            TmpListID = TmpListID ->Next;         
            CustFreeAllocatedMem( TmpDelNode, "ClearCDTreeList()", __LINE__ );
           } 
     }


   CustFreeAllocatedMem( TmpNode, "ClearCDTreeList()", __LINE__ );
  }

  gCryptCDItemRoot = NULL;

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:49:50 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:49:50 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void ClearDragLinkList( void )
{
  CRYPTDRAGINFO  *TmpNode = NULL;

  if(! gDragInfoRoot) return; // No list

  while( gDragInfoRoot )
  {
   TmpNode = gDragInfoRoot; 

   gDragInfoRoot = gDragInfoRoot ->Next;

   CustFreeAllocatedMem( TmpNode, "ClearDragLinkList()", __LINE__ );
  }

  gDragInfoRoot = NULL;

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 2:54:44 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 2:54:44 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void EndDrag( void ) 
{ 
   ImageList_EndDrag(); 
 
   ReleaseCapture(); 

   ClearDragLinkList();

   gbDragging = FALSE; 
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:00:56 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:00:56 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL FileExists(char *Filename)
{

 // Is file there?
 if(GetFileAttributes( Filename ) != -1)  return TRUE;

 return FALSE;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:11:37 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:11:37 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/////////
///

typedef WINSHELLAPI BOOL (WINAPI *fnPickIconDlg)(HWND hWndParent, LPTSTR pszFilename, LPDWORD pdwBufferSize, LPDWORD pdwIndex);

BOOL SelectIconA(HWND hWndParent, LPSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex)
{
	BOOL result = FALSE;
	OSVERSIONINFO versioninfo;
	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&versioninfo);
	HMODULE hShell32 = ::LoadLibrary("shell32.dll"); // LoadLibrary(_T("shell32.dll"));
	if (hShell32)
	{
		fnPickIconDlg PickIconDlg = (fnPickIconDlg)::GetProcAddress(hShell32, (LPCSTR)62);
		if (PickIconDlg)
		{
			if (versioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				LPWSTR pszWideName = new WCHAR[dwBufferSize];
				::MultiByteToWideChar(CP_ACP, 0, lpszFilename, -1, pszWideName, dwBufferSize);
				result = PickIconDlg(hWndParent, (LPTSTR)pszWideName, &dwBufferSize, pdwIndex);
				WideCharToMultiByte(CP_ACP, 0, pszWideName, -1, lpszFilename, dwBufferSize, NULL, NULL);
				delete[] pszWideName;
			}
			else
			{
				result = PickIconDlg(hWndParent, (LPTSTR)lpszFilename, &dwBufferSize, pdwIndex);
			}
		}
		FreeLibrary(hShell32);
	}
	return result;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:11:44 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:11:44 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL SelectIconW(HWND hWndParent, LPWSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex)
{
	BOOL result = FALSE;
	OSVERSIONINFO versioninfo;
	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&versioninfo);
	HMODULE hShell32 = ::LoadLibrary("shell32.dll"); // LoadLibrary(_T("shell32.dll"));
	if (hShell32)
	{
		fnPickIconDlg PickIconDlg = (fnPickIconDlg)::GetProcAddress(hShell32, (LPCSTR)62);
		if (PickIconDlg)
		{
			if (versioninfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
			{
				LPSTR pszMBName = new CHAR[dwBufferSize];
				WideCharToMultiByte(CP_ACP, 0, lpszFilename, -1, pszMBName, dwBufferSize, NULL, NULL);
				result = PickIconDlg(hWndParent, (LPTSTR)pszMBName, &dwBufferSize, pdwIndex);
				MultiByteToWideChar(CP_ACP, 0, pszMBName, -1, lpszFilename, dwBufferSize);
				delete[] pszMBName;
			}
			else
			{
				result = PickIconDlg(hWndParent, (LPTSTR)lpszFilename, &dwBufferSize, pdwIndex);
			}
		}
		FreeLibrary(hShell32);
	}
	return result;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:12:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:12:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

  Generates a random string based on the passed "seed" 
  
*/
char *GenerateRandomString( int seed )
{
//  char StringToMakeRandom[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  static char NewStr[ 512 ];

  // generate the string to make random ( anti-hacker - so they can't see the actual characters in notepad etc. )...
  int i, n = 0;

  memset( gNormChars, 0, 255 );
 

  for( i = 0; i < 26; i++ )
       gNormChars[ n + i ] = 'a' + i;
  n += 26;

  for( i = 0; i < 26; i++ )
       gNormChars[ n + i ] = 'A' + i;
  n += 26;


  for( i = 0; i <= 9; i++ )
       gNormChars[ n + i ] = '0' + i;
  n += 10;


  int Len = strlen( gNormChars );

  memset( NewStr, 0, 512 );
  memset( NewStr, '*', Len );

  srand( seed );

  // go through each character of the string and put it in a random position in the new string... 
  n = 0;
  int TooIndex = 0;
  int NumCharsReplaced = 0;
  while( NumCharsReplaced != Len )
       {
 
        // NOTE: this may get caught in a LONG cycle! or even endless!
        do{  // find a free spot in NewStr

           TooIndex = rand() % Len;
  
          }while( NewStr[ TooIndex ] != '*' );
            
 
        NewStr[ TooIndex ] = gNormChars[ n ];

        n++;
        NumCharsReplaced++;
       }


  return NewStr;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:14:06 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:14:06 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// Converts an ascii string to a hexadecimal value
int atox( char *str ) 
{
  int i = 0;
  int result = 0;

  // Iterate through the string
  while( str[ i ] != '\0' ) 
       {
        result = result << 4;

 		 if( str[ i ] >= '0' && str[ i ] <= '9' )
			result |= ( str[ i ] - '0' );
           else if ( str[ i ] >= 'a' && str[ i ] <= 'f' )
			result |= ( str[ i ] + 10 - 'a' );
           else if ( str[ i ] >= 'A' && str[ i ] <= 'F' )
			result |= ( str[ i ] + 10 - 'A' );

		i++;
	}

  return result;
}


/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:15:25 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:15:25 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
void UnEncText( char *EncStr, char *Data )
{
  int i = 0;
  int Len = 0;

  if(! Data) 
       OK_Msg("ERROR!","*Data == NULL! in UnEncText!");

  Len = strlen( Data );

  while( i < Len )
  {
   Data[ i ] = (char) UnEncChar( EncStr, Data[ i ] ); 
   i++;
  }

}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:15:28 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:15:28 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char UnEncChar( char *EncStr, char c )
{
  char nc;
  char *p = NULL;

  if( c == '-' )
      return ' ';

  if(! ( p = strchr(gNormChars, c ) ) )
     return ' '; // not found... just encrypt it as a space...

  nc = EncStr[ IndexOf( gNormChars, c) ];  

  return nc;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:16:04 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:16:04 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
int IndexOf( char *Data, char c ) 
{
  int i = 0;
  
  while(*Data)
  {
   
   if(*Data == c) return i; 
   Data++;
   i++;
  }
  
  return '*';  /* if 0 AN ERROR occured... */
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:16:55 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:16:55 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL XCopy( char *From, char *Too )
{
  BOOL bRet = TRUE;
 

  if( strlen( Too ) < 4 )
    {
     // D://  ?
     if( Too[ 1 ] != ':' )
         if( ! CreateDirectory(Too, NULL))
             return FALSE;
    }
   else
    {
    // directory may already be there...
     if( ! ( FileExists( Too ) ) )
       {


         if( ! CreateDirectory(Too, NULL))
           {
          
            OK_Msg("DEBUG:", "create directory failed... on create: %s", Too );  // DEBUG
          
            return FALSE;
           }
       }
 
    }

  char csPathMask[ _MAX_PATH ];
  char csFullPath[ _MAX_PATH ];
  char csNewFullPath[ _MAX_PATH ];
    
   strcat( From, "\\" );
   sprintf( csPathMask, "%s*.*", From );
       
    WIN32_FIND_DATA ffData;
    HANDLE hFind;
    hFind = FindFirstFile(csPathMask, &ffData);


    if (hFind == INVALID_HANDLE_VALUE)
	{
         return FALSE;
    }
    

    // Copying all the files
    while (hFind && FindNextFile(hFind, &ffData)) 
    {
        sprintf( csFullPath, "%s%s", From, ffData.cFileName ); 
        sprintf( csNewFullPath, "%s\\%s", Too, ffData.cFileName ); 

        if( !(ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
        { 
            if( !CopyFile(csFullPath, csNewFullPath, FALSE) ) 
            { 
                // Not stopping here, trying to copy the rest of the files
                bRet = FALSE;
            }
        }
        else // it is a directory -> Copying recursivly
        { 
            if( (strcmp(ffData.cFileName, ".") != 0) &&
                (strcmp(ffData.cFileName, "..") != 0) ) 
            {

                if( ! XCopy(csFullPath, csNewFullPath) )
                {
                    // Not stopping here, trying to copy the rest of the files
                    bRet = FALSE;
                }
            }
        }
    }

  FindClose(hFind);

  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:41:33 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:41:33 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

  returns:
  IDCANCEL, IDYES or IDNO
*/
DWORD YesNoCancel_Msg(char *Title, char *Format, ...)
{
  char TmpBuff[1024];
  char *pArgs = (char *) &Format + sizeof(Format);

  vsprintf(TmpBuff, Format, pArgs);

  return MessageBox(NULL, TmpBuff, Title, MB_YESNOCANCEL | MB_SYSTEMMODAL | MB_ICONQUESTION); 
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:41:37 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:41:37 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

  returns:
  IDYES or IDNO
*/
DWORD YesNo_Msg(char *Title, char *Format, ...)
{
  char TmpBuff[1024];
  char *pArgs = (char *) &Format + sizeof(Format);

  vsprintf(TmpBuff, Format, pArgs);

  return MessageBox(NULL, TmpBuff, Title, MB_YESNO | MB_SYSTEMMODAL | MB_ICONQUESTION); 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:53:39 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:53:39 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL MakeDirs( char *Path )
{
  char *token = NULL;
  char TmpStr[1024];
  char WorkPath[1024];

  memset(WorkPath, 0, 1024);

  strcpy(WorkPath, Path);  // strtok kills passed string, so copy it...

  memset(TmpStr, 0, 1024);

  token = strtok( (char *) WorkPath, "\\" );
  strcpy( TmpStr, token );       // "c:" by now... add a backslash...

  token = strtok( NULL, "\\" );   // get 1st dir...
  while( token != NULL )
  {
   // While there are backslashes in the path...
   sprintf(TmpStr, "%s\\%s",TmpStr, token );
    
   // Make this directory...
   
   // check if it already exists...
   if(GetFileAttributes( TmpStr ) == -1)  // don't make it if it already exists...
   {

    if( ! (CreateDirectory(TmpStr, NULL)))
    {
     MessageBox(NULL, TmpStr, "Error Making Dir...(4311)", MB_OK);
     return FALSE;  
    }
   
   }

   // Get next dir (if one exists...)
   token = strtok( NULL, "\\" );
  }

  
  return TRUE;  // success...
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:57:27 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:57:27 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
char ChangeDrive(char Drive)
{
  int i = _getdrive();   
  char CurrDrive;
  
  CurrDrive = (i + 'A' - 1);  

  _chdrive( toupper(Drive) - 'A' + 1 );
       
  return CurrDrive;
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:58:44 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:58:44 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
int LocalToWideChar( LPWSTR pWide, LPTSTR pLocal, DWORD dwChars )
{

   *pWide = 0;

#ifdef UNICODE
   lstrcpyn(pWide, pLocal, dwChars);
#else
   MultiByteToWideChar( CP_ACP, 0, pLocal, -1, 
                        pWide, dwChars ); 
#endif

   return lstrlenW(pWide);
}




/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 3:59:29 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 3:59:29 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
// NOTE: NULLS end of passed path/filename!
void StripFilenameFromPath( char *Path )
{
  char *p = NULL;

  if( ! Path ) 
     return;

  p = &Path[ strlen(Path) ];

  while( p )
  {
   if( *p == '\\' )
     {
      *p = '\0';
      return;
     }

   p--;
  }
 
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:00:31 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:00:31 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
BOOL AddFileToDat( FILE *DatOut, char *FileToAddName, CRYPT_ITEM *CryptItem )
{
  FILE *in = NULL;
  DWORD NumChunks = 0L;
  DWORD FileSize = 0L;
  DWORD WhatsLeft = 0L;


  // not encrypted... 
  if( ! ( CryptItem ->Flags & FLAG_ENCRYPTED ) ) 
    {

     if( ! ( in = fopen( FileToAddName, "rb" ) ) )  
       { 
		 return TRUE;
       }

     fseek( in, 0L, SEEK_END );
     FileSize = ftell( in );
     fseek( in, 0L, SEEK_SET );


     if( ! CopyToDatNoEncrypt( DatOut, in, FileSize, CryptItem ))
       {
        if( in ) fclose( in );
        OK_Msg( "Error: (452375)", "Cannot encrypt .dat file! Aborting." );
        return FALSE; 
       }

     if( in ) fclose( in );

     return TRUE; 
    }



#ifdef FULL_VERSION

     if( CryptItem ->Flags & FLAG_ENC_AES ) 
       {                          

        if( ! AESCopyEncrypt( DatOut, CryptItem ) )
          {
           OK_Msg( "Error: (454447)", "Cannot encrypt .dat file! Aborting." );
           return FALSE; 
          }

        return TRUE;
       }


// -- FLAG_ENC_BLOWFISH
     if( CryptItem ->Flags & FLAG_ENC_BLOWFISH ) 
       {                          

        if( ! BlowFishCopyEncrypt( DatOut, CryptItem ) )
          {
           OK_Msg( "Error: (454447)", "Cannot encrypt .dat file! Aborting." );
           return FALSE; 
          }

        return TRUE;
       }
//
#endif // end FULL VERSION...

     // open file to add...
     if( ! ( in = fopen( FileToAddName, "rb" ) ) ) // FILE may be ZERO bytes... don't error.
       { 
		 return TRUE;
       }

     fseek( in, 0L, SEEK_END );
     FileSize = ftell( in );
     fseek( in, 0L, SEEK_SET );
       

     if( CryptItem ->Flags & FLAG_ENC_XOR )
       {

        if( ! XORCopyEncrypt( DatOut, in, FileSize, CryptItem ))
          {
           if( in ) fclose( in );
           OK_Msg( "Error: (452375)", "Cannot encrypt .dat file! Aborting." );
           return FALSE; 
          }


        if( in ) fclose( in );

        return TRUE;
       }

      else if( CryptItem ->Flags & FLAG_ENC_SCRAMBLE ) 
             {

              if( ! ScrambleCopyEncrypt( DatOut, in, FileSize, CryptItem ))
                {
                 if( in ) fclose( in );
                 OK_Msg( "Error: (452377)", "Cannot encrypt .dat file! Aborting." );
                 return FALSE; 
                }


              if( in ) fclose( in );

              return TRUE;
             }

            else
             {
              if( in ) fclose( in );

              OK_Msg( "Error: (452399)", "No valid encrypt type for file %s! Aborting.", CryptItem ->ItemName );

              return FALSE; 
             }



  return TRUE;
}





/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:05:42 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:05:42 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

    AES DOES NOT USE CHUNKS! Just encrypt it and add it to the .dat file...
         to decrypt, do the same...

// Function from the .LIB 
BOOL InsomAESEncrypt( char *Password, char *Filename, char *OutFilename );
BOOL InsomAESDecrypt( char *Password, char *Filename, char *OutFilename );

  NOTES:  
*/
#ifdef FULL_VERSION

BOOL AESCopyEncrypt( FILE *TooDatFile, CRYPT_ITEM *CryptItem  )
{
  FILE *FromNewEncryptedFile = NULL;
  BYTE *p = NULL;
  DWORD Parts, BytesLeft, Size;
  char TmpDir[ 255 ],
       OriginalFile[ 255 ], // ORIGPATH/ORIGFILENAME.EXT
       NewEncryptedFile[ 255 ]; // TEMP/NewEncryptedfile.ext


  if( ! CryptItem ->ItemPath )  // NO PATH... so it's just a folder! ( I think. CHECK THIS! )
    { 
      return TRUE;
    }

  // Get windows temp dir to copy too...
  if(! ( GetTempPath( MAX_PATH, TmpDir ) ) )
    {
      /*
      There might not be a TEMP directory. If this is the case, use the
      Windows directory.
      */ 
      if(! ( GetWindowsDirectory( TmpDir, MAX_PATH ) ) )
        {
         return FALSE;
        }

    }


  Size = CryptItem ->Size;  


  // HDD file...  ( ItemPath May be NULL ( if it's a folder ) )
  sprintf( OriginalFile, "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName ); 

  // ...new encrypted file to ADD to the .DAT file ( dat == FILE *Too )
  sprintf( NewEncryptedFile, "%sR%s", TmpDir, CryptItem ->ItemName );  // why is there an R here???

  // encrypt the copied file to the new "encrypted" file...
  if( ! ( InsomAES_Encrypt( gCfg.Password, OriginalFile, NewEncryptedFile ) ) )
    {
     OK_Msg( "ERROR:", "AES encrypt failed. Aborting." ); 
     return FALSE;
    }

 
  // open the NEW encrypted file...
  if( ! ( FromNewEncryptedFile = fopen( NewEncryptedFile, "rb" ) ) )
    {
     OK_Msg( "ERROR:", "Cannot open AES tmp." ); 
     return FALSE;
    }
  
  fseek( FromNewEncryptedFile, 0, SEEK_SET );  // just in case...

  if( ! (p = (BYTE *) CustAllocateMem( MAX_FILE_BUFFER + 100, "AESCopyEncrypt()", __LINE__ )))
    {
     OK_Msg("Error:",  "Cannot allocate memory for file!");
     return FALSE;
    }

 
  // THIS DOES NOT SEEM TO BE COPYING TO THE .DAT FILE
  if( Size <= MAX_FILE_BUFFER ) // nope. will fit in 1 chunk...
    {
     fread(  p, 1, Size, FromNewEncryptedFile ); // read in the file to memory...
     fwrite( p, 1, Size, TooDatFile );// pack into the setup.exe file...
    }
   else  // File is > MAX_FILE_BUFFER... read/write in chunks...
     {
      Parts     = (Size / MAX_FILE_BUFFER );
      BytesLeft = (Size % MAX_FILE_BUFFER );

      while( Parts )
           {
            fread(  p, 1, MAX_FILE_BUFFER, FromNewEncryptedFile ); // read in the file to memory...
            fwrite( p, 1, MAX_FILE_BUFFER, TooDatFile );// pack into the setup.exe file...

            Parts--;
           }

      if( BytesLeft )
        {
         fread(  p, 1, BytesLeft, FromNewEncryptedFile ); // read in the file to memory...
         fwrite( p, 1, BytesLeft, TooDatFile );// pack into the setup.exe file...
        }

    } // end else...


  if( FromNewEncryptedFile ) fclose( FromNewEncryptedFile );

  CustFreeAllocatedMem( p,"AESCopyEncrypt()", __LINE__ );
  
  // delete the temp encrypted file in the temp dir...
  DeleteFile( NewEncryptedFile ); 


  return TRUE;
}



/****************************************
 *                                       
 * Function:                             
 *                                       
 * Pass: Nothing.                        
 *                                       
 * Date:          12/17/2014  ( 4:05:32 PM )
 *                                       
 * Code By:       Russ Greer                   
 *                                       
 * Last modified: 12/17/2014  ( 4:05:32 PM )
 *                                       
 * BUGS:          None known.                  
 *                                       
 * NOTES:                                
 *                                       
 * Returns:       Nothing.                     
 *                                       
 ****************************************/
/*

    BLOWFISH DOES NOT USE CHUNKS! Just encrypt it and add it to the .dat file...
         to decrypt, do the same...

// Function from the .LIB 
BOOL InsomBlowFishEncrypt( char *Password, char *Filename, char *OutFilename );
BOOL InsomBlowFishDecrypt( char *Password, char *Filename, char *OutFilename );

  NOTES: 
*/
BOOL BlowFishCopyEncrypt( FILE *TooDatFile, CRYPT_ITEM *CryptItem  )
{
  FILE *FromNewEncryptedFile = NULL;
  BYTE *p = NULL;
  DWORD Parts, BytesLeft, Size;
  char TmpDir[ 255 ],
       OriginalFile[ 255 ], // ORIGPATH/ORIGFILENAME.EXT
       NewEncryptedFile[ 255 ]; // TEMP/NewEncryptedfile.ext

  if( ! CryptItem ->ItemPath )  // NO PATH... so it's just a folder! 
    { 
      return TRUE;
    }

  // Get windows temp dir to copy too...
  if(! ( GetTempPath( MAX_PATH, TmpDir ) ) )
    {
      /*
      There might not be a TEMP directory. If this is the case, use the
      Windows directory.
      */ 
      if(! ( GetWindowsDirectory( TmpDir, MAX_PATH ) ) )
        {
         return FALSE;
        }

    }


  Size = CryptItem ->Size;  

  // ...file to copy from ( original on the HDD )
  sprintf( OriginalFile, "%s\\%s", CryptItem ->ItemPath, CryptItem ->ItemName ); 

  // ...new encrypted file to ADD to the .DAT file ( dat == FILE *Too )
  sprintf( NewEncryptedFile, "%sR%s", TmpDir, CryptItem ->ItemName );

  // encrypt the copied file to the new "encrypted" file...
  if( ! ( InsomBlo_Encrypt( gCfg.Password, OriginalFile, NewEncryptedFile ) ) )
    {
     OK_Msg( "ERROR:", "BlowFish encrypt failed. Aborting." ); 
     return FALSE;
    }


  // open the NEW encrypted file...
  if( ! ( FromNewEncryptedFile = fopen( NewEncryptedFile, "rb" ) ) )
    {
     OK_Msg( "ERROR:", "Cannot open Blo tmp." ); 
     return FALSE;
    }
  
  fseek( FromNewEncryptedFile, 0, SEEK_SET );  // just in case...

  if( ! (p = (BYTE *) CustAllocateMem( MAX_FILE_BUFFER + 100, "BloCopyEncrypt()", __LINE__ )))
    {
     OK_Msg("Error:",  "Cannot allocate memory for file!");
     return FALSE;
    }

 
  // THIS DOES NOT SEEM TO BE COPYING TO THE .DAT FILE
  if( Size <= MAX_FILE_BUFFER ) // nope. will fit in 1 chunk...
    {
     fread(  p, 1, Size, FromNewEncryptedFile ); // read in the file to memory...
     fwrite( p, 1, Size, TooDatFile );// pack into the setup.exe file...
    }
   else  // File is > MAX_FILE_BUFFER... read/write in chunks...
     {
      Parts     = (Size / MAX_FILE_BUFFER );
      BytesLeft = (Size % MAX_FILE_BUFFER );

      while( Parts )
           {
            fread(  p, 1, MAX_FILE_BUFFER, FromNewEncryptedFile ); // read in the file to memory...
            fwrite( p, 1, MAX_FILE_BUFFER, TooDatFile );// pack into the setup.exe file...

            Parts--;
           }

      if( BytesLeft )
        {
         fread(  p, 1, BytesLeft, FromNewEncryptedFile ); // read in the file to memory...
         fwrite( p, 1, BytesLeft, TooDatFile );// pack into the setup.exe file...
        }

    } // end else...


  if( FromNewEncryptedFile ) fclose( FromNewEncryptedFile );

  CustFreeAllocatedMem( p,"BloCopyEncrypt()", __LINE__ );
  
  // delete the temp encrypted file in the temp dir...
  DeleteFile( NewEncryptedFile ); 


  return TRUE;
}

#endif // end if FULL VERSION