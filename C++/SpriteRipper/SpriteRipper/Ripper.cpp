#include "stdafx.h"
#include <memory>
#include "Ripper.h"


Ripper::Ripper(void)
{
}


void Ripper::LoadBMP(wchar_t* fileName)
{
  HBITMAP hBM;
  HDC hdcScreen;
  HDC hDCMem;
  BITMAP bm;
  OLE_XSIZE_HIMETRIC Width;
  OLE_YSIZE_HIMETRIC Height;
  IPicture *Picture = NULL;


  if(FAILED(OleLoadPicturePath( fileName, 0, 0, 0, IID_IPicture, (void**)&Picture)))
    { 
	 wchar_t msg[255];
	 wsprintf(msg, L"Canno't load %s. Aborting.", fileName);
     MessageBox(NULL, msg,TEXT("Error:1235"), MB_OK);
     return;
    }

  hdcScreen = CreateDC(L"DISPLAY", (LPCWSTR) NULL, (LPCWSTR) NULL, (CONST DEVMODE *) NULL); 
  hDCMem = CreateCompatibleDC( hdcScreen ); 

  Picture ->get_Width(  &Width  );
  Picture ->get_Height( &Height );


  bm.bmWidth  = MAP_LOGHIM_TO_PIX( Width,  GetDeviceCaps(hdcScreen, LOGPIXELSX));
  bm.bmHeight = MAP_LOGHIM_TO_PIX( Height, GetDeviceCaps(hdcScreen, LOGPIXELSY));

  
  hBM = CreateCompatibleBitmap( hdcScreen, bm.bmWidth, bm.bmHeight );
  SelectObject( hDCMem, hBM );
  
  Picture ->Render(hDCMem, 0, 0, bm.bmWidth, bm.bmHeight,  0, Height, Width, -Height, NULL);

  DeleteDC(hDCMem);

  // Actually rip out the images from the BMP image
  if( ! ( RipImageToSprites( hBM, bm ) ) )
    {
      MessageBox(NULL, L"Canno't rip images from .bmp. Aborting.",TEXT("Error:1236"), MB_OK);
      return;
    }
 

  DeleteObject( hBM ); // Have all the frames... can now get rid of the source bitmap.

}




BOOL Ripper::RipImageToSprites( HBITMAP hBM, BITMAP bm )
{
  unsigned long  NumBytes = ( bm.bmWidth * bm.bmHeight);
  unsigned char  *Buffer  = new unsigned char[NumBytes * 4]; 

  if(! ( GetBitmapBits( hBM, NumBytes * 4, (LPVOID)Buffer ) ) )
    {
     MessageBox(NULL, L"Cannot get pointer to bitmap bits to find frames. Aborting.",TEXT("Error:1237"), MB_OK);
	 if(Buffer) delete Buffer;
     return FALSE;
    }


  int Boxes = 0;
  DWORD x,y;
  for( y = 0; y < (DWORD) bm.bmHeight; y++ )
     {
       for( x = 0; x < (DWORD) bm.bmWidth; x++)
          {
            BYTE *ByteA = &Buffer[ (x * 4) + ( y * (bm.bmWidth*4) ) ];

            BOOL hotSpot = FALSE;
            if( IsBoxColor( ByteA, &hotSpot ) ) // Is this pixel the box color?
              {
               BYTE *ByteB = &Buffer[ ((x+1) * 4) + ( y * (bm.bmWidth*4) ) ];
               BYTE *ByteC = &Buffer[ (x * 4) + ( (y+1) * (bm.bmWidth*4) ) ];

                if( IsBoxColor( ByteB, &hotSpot  ) && IsBoxColor( ByteC, &hotSpot  ) )
                  {
                    Boxes++;

                    // Found a box... Save ByteA Ptr to top corner and grab those bytes...
                    int hotSpotX = 0;
                    int hotSpotY = 0;
                    int BoxWidth  = CountBoxWidth( &Buffer[ (x * 4) + ( y * (bm.bmWidth*4) ) ], x, y, &hotSpotX );              
                    int BoxHeight = CountBoxHeight( Buffer, x, y, (bm.bmWidth*4), &hotSpotY );

                    if( ! ( ClipOutSpriteFrame( ByteA, BoxWidth-2, BoxHeight-2, (bm.bmWidth*4), hotSpotX-2, hotSpotY-2 ) ) )
                      { 
                        MessageBox(NULL, L"Failed to clip sprite...",TEXT("Error:1238"), MB_OK);

                        if( Buffer ) delete Buffer; 

                        return FALSE; // Quit clipping and abort... user screwed up and tried to overwrite an existing anim...
                      }
                  }
              }        
          }
     }


  if( Buffer ) delete Buffer; 

  return TRUE;
}


// TODO: use options for box/hotspot colors
BOOL Ripper::IsBoxColor( unsigned char *Pixels, BOOL *hotSpot )
{
   *hotSpot = FALSE;

   if( (Pixels[0] == 0xff) &&
       (Pixels[1] == 0x00) && 
       (Pixels[2] == 0xff) )     // Is this pixel the box color?
        return TRUE;

   if( (Pixels[0] == 0x00) &&
       (Pixels[1] == 0xff) && 
       (Pixels[2] == 0x00) )     // Is this pixel the box hotspot color?
       {
        *hotSpot = TRUE;
        return TRUE;
       }

   return FALSE;
}



int Ripper::CountBoxHeight( unsigned char *Buffer, unsigned long x, unsigned long y, unsigned long width, int *hotSpotY )
{
   int BoxHeight = 0;
   BOOL hotSpot = FALSE; 

   unsigned char *WorkPtr = &Buffer[ (x * 4) + ( (y+1) * width ) ];
                  
   while( IsBoxColor( WorkPtr, &hotSpot ) )
        {  
         BoxHeight++;
         WorkPtr = &Buffer[ (x * 4) + ( (y+BoxHeight) * width)  ];


          if( hotSpot )
              *hotSpotY = BoxHeight;  // save the hotspot y...  
        } 

   return BoxHeight;
}





int Ripper::CountBoxWidth( unsigned char *Buffer, unsigned long x, unsigned long y, int *hotSpotX )
{
   int BoxWidth = 0;  
   BOOL hotSpot = FALSE;              

   unsigned char *WorkPtr = &Buffer[0];

   while( IsBoxColor( WorkPtr, &hotSpot ) )
        {
          BoxWidth++;
          WorkPtr += 4;

          if( hotSpot )
              *hotSpotX = BoxWidth;  // save the hotspot x...
        } 

   return BoxWidth;
}



BOOL Ripper::ClipOutSpriteFrame( unsigned char *FromPtr, unsigned long BoxWidth, unsigned long BoxHeight, unsigned long BufferWidth, int hotSpotX, int hotSpotY )
{
   static long frameNumber = 0;

   // get the raw image data for this frame...
   unsigned char* ImageData = new unsigned char[(BoxWidth * BoxHeight) * 4];

   if( !ImageData)
     {
	   MessageBox(NULL, L"Cannot allocate image data. Aborting.",TEXT("Error:1239"), MB_OK);
       return FALSE;
     }

   memset( ImageData, 0, ( BoxWidth * BoxHeight) * 4 );  

 
   // Crop out the image... 
   int x = 4; // 4 = bpp
   for( unsigned long y = 0; y < BoxHeight; y++ )
        memcpy( &ImageData[ y * ( BoxWidth * 4 )], &FromPtr[ x + ((y+1) * BufferWidth) ], BoxWidth * 4 );
      

    std::shared_ptr<Sprite> newSprite = std::make_shared<Sprite>( ImageData, BoxWidth, BoxHeight, hotSpotX, hotSpotY );     
		
	newSprite ->SetFrameNumber( ++frameNumber );   
	newSprite ->SetTransparentColor( RGB(0,0,255) ); 

    gApp ->AddSprite(newSprite);
    gApp ->SetNumSprites(gApp ->GetNumSprites()+1);
  
   return TRUE;
} 


