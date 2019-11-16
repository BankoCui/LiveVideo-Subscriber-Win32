#pragma once

#include "stdint.h"
#include "inttypes.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#ifdef _WIN32
#pragma comment(lib, "libgcc.a")
#pragma comment(lib, "libmingwex.a")
#pragma comment(lib, "libavcodec.a")
#pragma comment(lib, "libavutil.a")
#pragma comment(lib, "libswscale.a")
#endif

struct H264_DEC_FRAME_S {
	unsigned char* pY;                  //Y plane base address of the picture
	unsigned char* pU;                  //U plane base address of the picture
	unsigned char* pV;                  //V plane base address of the picture
	unsigned int uWidth;                //The width of output picture in pixel
	unsigned int uHeight;               //The height of output picture in pixel
	unsigned int uYStride;              //Luma plane stride in pixel
	unsigned int uUVStride;             //Chroma plane stride in pixel
};

class H264decoder
{
public:
	H264decoder(void);
	~H264decoder(void);
	bool Create();
	void Destroy();

	int Decode(unsigned char* h264packet, int pkgLen, H264_DEC_FRAME_S* pDecFrame);

private:
	AVCodecID codec_id_;
	AVPixelFormat dst_fmt_;
	AVCodec* codec_;
	AVCodecContext* avctx_;
	AVFrame* src_frame_;
	AVPacket pkt_;
};

