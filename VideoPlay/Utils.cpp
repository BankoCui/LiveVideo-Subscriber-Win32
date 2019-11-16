#include <stdio.h>
#include "Utils.h"


//Change endian of a picture   
void change_endian_picture_rgb(unsigned char *image,int w,int h,int bpp)
{
	unsigned char *pixeldata = NULL;  
	for(int i =0;i<h;i++)  
		for(int j=0;j<w;j++)
		{  
			pixeldata=image+(i*w+j)*bpp/8;  
			if(bpp == 32)
			{
				change_endian_picture_rgb_32(pixeldata);  
			} else if(bpp == 24)
			{  
				change_endian_picture_rgb_24(pixeldata);  
			} else if (bpp == 16)
			{
				change_endian_picture_rgb_16_5551(pixeldata);
			}
		}  
}

//change endian of a pixel (32bit)   
void change_endian_picture_rgb_32(unsigned char *data)
{  
	char temp3,temp2;  
	temp3 = data[3];  
	temp2 = data[2];  
	data[3] = data[0];  
	data[2] = data[1];  
	data[0] = temp3;  
	data[1] = temp2;  
} 

//change endian of a pixel (24bit)   
void change_endian_picture_rgb_24(unsigned char *data)
{
	char temp2 = data[2];
	data[2] = data[0];
	data[0] = temp2;
}

//change endian of a pixel (16bit)   
void change_endian_picture_rgb_16_5551(unsigned char *data)
{
	short* rgb_16 = (short*)data;

	short R = (rgb_16[0] & 0X7C00) >> 10;
	short B = rgb_16[0] & 0x001F;

	rgb_16[0] = rgb_16[0] & (0XFFFF - 0X7C1F);

	rgb_16[0] = rgb_16[0] | (B << 10);
	rgb_16[0] = rgb_16[0] | R;
}

//change endian of a pixel (16bit)   
void change_endian_picture_rgb_16_565(unsigned char *data)
{
	short* rgb_16 = (short*)data;

	short R = (rgb_16[0] & 0xF800) >> 11;
	short B = rgb_16[0] & 0x001F;

	rgb_16[0] = rgb_16[0] & (0XFFFF - 0XF81F);

	rgb_16[0] = rgb_16[0] | (B << 11);
	rgb_16[0] = rgb_16[0] | R;
}

void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height)
{
	//  int begin = GetTickCount();
	int R,G,B,Y,U,V;
	int x,y;
	int nWidth = width>>1; //色度信号宽度
	for (y=0;y<height;y++)
	{
		for (x=0;x<width;x++)
		{
			Y = *(yuv420[0] + y*width + x);
			U = *(yuv420[1] + ((y>>1)*nWidth) + (x>>1));
			V = *(yuv420[2] + ((y>>1)*nWidth) + (x>>1));
			R = Y + 1.402*(V-128);
			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
			B = Y + 1.772*(U-128);

			//防止越界
			if (R>255)R=255;
			if (R<0)R=0;
			if (G>255)G=255;
			if (G<0)G=0;
			if (B>255)B=255;
			if (B<0)B=0;

			// bmp(rgb)在y方向正向存储
			//*(rgb24 + ((height-y-1)*width + x)*3) = B;
			//*(rgb24 + ((height-y-1)*width + x)*3 + 1) = G;
			//*(rgb24 + ((height-y-1)*width + x)*3 + 2) = R;

			// bmp(rgb)在y方向反向存储
			*(rgb24 + (y*width + x)*3) = B;
			*(rgb24 + (y*width + x)*3 + 1) = G;
			*(rgb24 + (y*width + x)*3 + 2) = R;   
		}
	}
}