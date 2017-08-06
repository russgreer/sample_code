#pragma once

class Image
{
private:
	int width;
	int height;
	int bytesPerPixel;
	unsigned char *image_data;

public:
	Image( unsigned char *data, int width, int height, int bytes_per_pixel );
	unsigned char *getImageData(void);
	int getWidth(void);
	int getHeight(void);
	int getBytesPerPixel(void);
	~Image(void);
};

