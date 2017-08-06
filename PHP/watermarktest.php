<?php

 /*
     Test using the watermark class.
 */

 require "watermarkclass.php";

 $image = imagecreatefromgif('graph2.gif');  // load in the image to watermark

 $n = rand( 1111111111, 9999999999 );  // generate a random number

 $str = sprintf( "%010d", $n);  // conver the number into hex

 $wm = new WaterMark();

 // draw in the multi-colored watermark number
 $wm ->ImageTTFtextMulti( $image, 80, 0, 104, 160, "arial.ttf", $str, 0 );
 $wm ->ImageTTFtextMulti( $image, 80, 0, 104, 260, "arial.ttf", $str, 0 );

 // put in the bar code lines...
 $wm ->NumberToPixelLines( $image, $n, 87, 42 );

 // draw in the top squares...
 $wm ->NumberToSquare( $image, 250, 4, $n, 2, 2, 1, 8 );
 $wm ->NumberToSquare( $image, 515, 4, $n, 2, 2, 1, 8 );
 
 // show the watermarked image... 
 header("Content-type: image/png");
 imagepng( $image );
 imagedestroy( $image );

?>