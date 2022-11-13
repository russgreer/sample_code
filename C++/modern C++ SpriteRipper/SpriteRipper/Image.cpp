#include "stdafx.h"
#include "Image.h"


Image::Image( unsigned char *data, int width, int height, int bytes_per_pixel )
{
	this ->width = width;
	this ->height = height;
	bytesPerPixel = bytes_per_pixel;

	image_data = new unsigned char[(width*height) * bytes_per_pixel];
    memcpy(image_data, data, (width*height) * bytes_per_pixel );
}


unsigned char *Image::getImageData(void)
{
    return (unsigned char *) image_data;
}

int Image::getWidth(void)
{
    return width;
}

int Image::getHeight(void)
{
    return height;
}


int Image::getBytesPerPixel(void)
{
	return bytesPerPixel;
}



Image::~Image(void)
{
	if( image_data )
		delete image_data;


}
