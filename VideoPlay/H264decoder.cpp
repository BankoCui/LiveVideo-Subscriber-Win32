#include "H264decoder.h"


H264decoder::H264decoder(void)
	:  codec_id_(AV_CODEC_ID_H264),
	codec_(NULL),
	avctx_(NULL),
	src_frame_(NULL)
{

}

H264decoder::~H264decoder(void)
{
	Destroy();
}

bool H264decoder::Create()
{
	bool ret = false;
	do {
		avcodec_register_all();
		codec_ = avcodec_find_decoder(AV_CODEC_ID_H264);
		if (!codec_)
			break;

		avctx_ = avcodec_alloc_context3(codec_);
		if (!avctx_)
			break;

		src_frame_ = av_frame_alloc();
		if (!src_frame_)
			break;

		if (codec_->capabilities & CODEC_CAP_TRUNCATED)
			avctx_->flags |= CODEC_FLAG_TRUNCATED;

		if (avcodec_open2(avctx_, codec_, NULL) != 0)
			break;

		av_init_packet(&pkt_);

		ret = true;
	} while (0);

	if (!ret) {
		Destroy();
	}
	return ret;
}

void H264decoder::Destroy() {
	if (avctx_ != NULL) {
		if (avctx_->extradata != NULL) {
			free(avctx_->extradata);
			avctx_->extradata = NULL;
		}
		avcodec_close(avctx_);
		av_free(avctx_);
		avctx_ = NULL;
	}

	if (src_frame_) {
		av_frame_free(&src_frame_);
	}
}

int H264decoder::Decode(unsigned char* h264packet, int pkgLen, H264_DEC_FRAME_S* pDecFrame) {
	int got_picture = 0;

	pkt_.data = h264packet;
	pkt_.size = pkgLen;
	if (avcodec_decode_video2(avctx_, src_frame_, &got_picture, &pkt_) < 0) {
		return 0;
	}

	if (!got_picture) {
		return 2;
	}

	pDecFrame->pY = src_frame_->data[0];
	pDecFrame->pU = src_frame_->data[1];
	pDecFrame->pV = src_frame_->data[2];
	pDecFrame->uWidth = avctx_->width;
	pDecFrame->uHeight = avctx_->height;
	pDecFrame->uYStride = avctx_->width;
	pDecFrame->uUVStride = avctx_->width/2;

	return 1;
}