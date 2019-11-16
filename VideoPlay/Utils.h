
//Change endian of a picture   
void change_endian_picture_rgb(unsigned char *image,int w,int h,int bpp);

//change endian of a pixel (32bit)   
void change_endian_picture_rgb_32(unsigned char *data);

//change endian of a pixel (24bit)   
void change_endian_picture_rgb_24(unsigned char *data);

//change endian of a pixel (16bit) 
void change_endian_picture_rgb_16_5551(unsigned char *data);

//change endian of a pixel (16bit) 
void change_endian_picture_rgb_16_565(unsigned char *data);

void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height);