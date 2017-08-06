<?php

/*
   Desc: Class to create a watermark and other security features on an image

   functions in this class:
   
   MergeMasked();         // merges two images together ( ONLY copies to the "mask" color in the destination image )
   RandColor();           // generates a random color 
   NumberToPixels();      // takes a number, retuns an image representing the bits of the number
   NumberToPixelLines();  // takes a number, draws a virticle row of thick and thin lines - like a bar code ( except verticle ) 
   NumberToSquare();      // takes a number, draws a square based on the bits of the passed number
   DrawMultiColorText();  // draws out text with random color lines per character
   ImageTTFtextMulti();   // does the actual drawing, masking etc. for DrawMultiColorText()

   code by: Russ Greer
*/


class WaterMark
{

  /*
      Desc: merges two images together ( ONLY copies to the "mask" color in the destination image )    

      Pass: image to write on the "inverse mask" color,
               image to grab the pixel colors from,
              color to draw on in the destination image

      Returns: Nothing.
  */
  function MergeMasked( $ToImg, $FromImg, $mask )
 {
   $h = imagesy( $ToImg );
   $w = imagesx( $ToImg );

   // go through the entire image and only write on the pixel color
   // that matches the inverse "mask" color... ( optimize for speed later )
   for( $y = 0; $y < $h; $y++ ) 
       {
          for( $x = 0; $x < $w; $x++ ) 
           {
             // get this pixel color
             if( ( $color = imagecolorat( $ToImg, $x, $y ) ) == $mask )   
                   imagesetpixel( $ToImg, $x, $y, imagecolorat( $FromImg, $x, $y ) );
          } 
      }
 }





 /*
     Desc: generates a random color  

     Pass: array to check for colors NOT to generate ( 10 of them ),
              image to allocate the color to.
         
     Returns: an allocated, random color...      
 */
 function RandColor( $dontuse, $image )
 {
   $x=0;
  
   do
     {
          srand( ( ( int ) ( ( double ) microtime() * 1000000 ) ) ); 

          $color = imagecolorallocate( $image, rand( 0, 255 ), rand( 0, 255 ), rand( 0, 255 ) );

          if( array_search( $color, $dontuse ) )       // is this color in the last 10 array?
               imagecolordeallocate($image, $color );  // ...yes, free the mem ( don't overload the server )...
             else
               break;        
 
    }while( $x < 1000 );   // reasonable probibility ( don't want to hang to long on the server )..
    
  return $color;
 }



 /*
     Desc: takes a number, retuns an image representing the bits of the number

     Pass: Number to be converted,
              width of a pixel box,
              height of a pixel box
         
     Returns: New pixel ( bit ) image 
 */
 function NumberToPixels( $number, $w, $h )
 {
    $str = sprintf( "%032b", $number );
    $len = strlen( $str );

    $imgw = ( $len * ( $w + 2 ) );
    $imgh = $h;
    $img  = imagecreatetruecolor( $imgw, $imgh );

    // set the background color
    $background = imagecolorallocate( $img, 255, 255, 255 );
    $oncolor    = imagecolorallocate( $img, 0, 0, 0 );
    $offcolor   = imagecolorallocate( $img, 195, 195, 195 );

    imagefilledrectangle( $img, 0, 0, $imgw, $imgh, $background );

    // ok, now make the rectangles spaced exactly 10 pixels apart,
    // filling the ones with 1 and NOT the ones with 0.
    $x = 0;
    for( $idx = 0; $idx < $len; $idx++ ) 
        {
           if( $str[ $idx ]  )
               $color = $oncolor;
              else
               $color = $offcolor;

          imagefilledrectangle( $img, $x, 0, $x + $w, $h, $color ); 

          $x += ( $w +2 );
        }

   return $img;
 }



 /*
     Desc: takes a number, draws a virticle row of thick and thin lines - like a bar code ( except verticle )

     Pass: image to draw on,
             Number to be converted,
             start x pos.,
             start y pos.
        
     Returns: Nothing.
 */
 function NumberToPixelLines( $image, $number, $x, $y )
 { 
    $str = sprintf( "%032b", $number );  // convert to binary number string
    $len = strlen( $str );

    for( $idx = 0; $idx < $len; $idx++ )  
        {
           if( $str[ $idx ]  ) 
            {
               // thick line
               imageline( $image, $x,   $y, $x+3,   $y, imagecolorresolve( $image, 0, 0, 0 ) ); 
               imageline( $image, $x, $y+1, $x+3, $y+1, imagecolorresolve( $image, 0, 0, 0 ) );           
            }
           else 
            {
              // thin line
              imageline( $image, $x,   $y, $x+3,   $y, imagecolorresolve( $image, 0, 0, 0 ) );   
            }

         $y += 9;
       }

 }



 /*
       Desc: takes a number, draws a square based on the bits of the passed number

       Pass: image to draw on,
                x pos. to draw at,
                y pos. to draw at,
                number to be converted,
                width of a pixel box,
                height of a pixel box,
                spacing between boxes,
                number of pixels in a row
         
       Returns: Nothing.
 */
 function NumberToSquare( $image, $x, $y, $number, $w, $h, $spacing, $rowlen )
 {
    $str = sprintf( "%032b", $number );  // convert to binary number string
    $len = strlen( $str );

    // set the background color
    $offcolor = imagecolorresolve( $image, 0, 0, 0 );
    $oncolor  = imagecolorresolve( $image, 190, 190, 190 );
  
    $numrows = ( $len / $rowlen ); 
    $rowpixels = ( $numrows * ( $w ) ) + $spacing;
    for( $yj = 0; $yj < $numrows; $yj++ ) 
        {  // down
           for( $xi = 0; $xi < $rowlen; $xi++ ) 
               {   // across
                   if( $str[ $xi + ( $yj * $rowlen ) ]  )
                       $color = $oncolor;
                      else
                       $color = $offcolor;                   

                   imagefilledrectangle(  $image,          
                                          $x + $xi + ( $xi * ( $w ) ) + $spacing,      
                                          $y + $yj,                                                       
                                          $x + $xi + ( $xi * ( $w ) ) + $w,            
                                          $y + $yj + $h,                                 
                                          $color );         
             }             
 
          $y += ( $h + 1 );
        }

}



 /*
        Desc: draws out text with random color lines throughout each character
              (to prevent easyily photoshopping out the characters)   

        Pass: image to draw on,
                 x,y pos. to draw at,
                 size of the font,
                 the font filename,
                 test string to draw,
                 brightness of the text.

      Returns: Nothing.
 */                           
 function DrawMultiColorText( $image, $x, $y, $size, $fnt, $text, $brightness )
 {
   $w = imagesx( $image ); 
   $h = imagesy( $image ); 

   // acllocate the lines image...
   $transimg = imagecreatetruecolor( $w, $h );
   $linesimg = imagecreatetruecolor( $w, $h ); 

   $mask    = imagecolorallocate( $transimg, 255,   0, 255 ); // color to draw on ( TEXT )
   $bgcolor = imagecolorallocate( $transimg,   0, 255, 0 );   // transparent bg color ( to NOT pick up )

   // setup the green screen...
   imagefill( $transimg, 0,0, $bgcolor );

   // draw the text on the trans image ( green screen )...
   //this ->ImageString( $transimg, $size, $x, $y, $text,  $mask );
   imagettftext( $transimg, $size, 0, $x, $y, $mask, $fnt, $text ); 

   // draw the bars image to merge over the text...
   $last10colors[] = "";
   for( $width = 0; $width < $w; $width++ ) 
       {
          $color = RandColor( $last10colors, $linesimg );

          if( count( $last10colors ) > 9 )
              $elm = array_shift($last10colors);  // shift off the oldest ( leftmost element )

          array_push($last10colors, $color);     // add the newest one...

          imageline( $linesimg, $width, 0, $width, $w, $color );

          imagecolordeallocate( $linesimg, $color );
      }
    
    
  if( $brightness )
      imagefilter( $linesimg, IMG_FILTER_BRIGHTNESS, $brightness); 

  // merge the striped colors onto any mask pixel color
  this ->MergeMasked( $transimg, $linesimg, $mask );

  // now we have a color text image with a green screen background...
  // set the background color of the new image as transparent.
  imagecolortransparent( $transimg, $bgcolor );
  imagecopymerge( $image, $transimg, 0, 0, 0, 0, $w, $h, 10 );

 }




 /*
       Desc: does the actual drawing, masking etc. for DrawMultiColorText()

       Pass: image to draw on,
                size of the font,
                angle of the font,
                x,y pos. of where to draw,
                the font file name,
                the text string to write,
                brightness of the text.
        
      returns: nothing.
 */
 function ImageTTFtextMulti( $image, $size, $angle, $x, $y, $fnt, $text, $brightness )
 {
   $w = imagesx( $image ); 
   $h = imagesy( $image ); 

   // get the fonts average character width and hight
   $len = strlen( $text );
   $fontwidth  = imagefontwidth( $size ) * $len;
   $fontheight = imagefontheight( $size );

   // allocate the new image and set up the bg transparent color
   $newimg = imagecreatetruecolor( $fontwidth, $fontheight );

   // draw the multi-colored text on the bg
   this ->DrawMultiColorText( $image, $x, $y, $size, $fnt, $text, $brightness );
 }

}


?>
