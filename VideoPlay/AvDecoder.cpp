#include "StdAfx.h"
#include "AvDecoder.h"
#include "H264decoder.h"
#include "AvPlayer.h"

AvDecoder::AvDecoder(AvPlayer* av_player)
	: av_player_(av_player),
	  h_decode_thread_(NULL)
{
	h264_decoder_ = new H264decoder();
	h264_decoder_->Create();
	h_decode_thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DecodeStreamThreadFunction, this, 0, NULL);
	// lpEventAttributes
	// bManualReset, FALSE: 事件有信号状态在WaitForSingleObject自动重置为无信号状态，然后再手动设置为有信号状态
	// bInitialState, TRUE: 初始状态为有信号状态
	// lpName
	h_decode_event_ = ::CreateEvent(NULL, FALSE, TRUE, NULL);
}

AvDecoder::~AvDecoder(void)
{
	if (h264_decoder_)
	{
		delete h264_decoder_;
		h264_decoder_ = NULL;
	}
	::ResetEvent(h_decode_event_);
	if (h_decode_thread_)
	{
		CloseHandle(h_decode_thread_);
		h_decode_thread_ = NULL;
	}
}

void AvDecoder::DecodeStreamThreadFunction(LPVOID lpParameter)
{
	AvDecoder* av_decoder = (AvDecoder*)lpParameter;
	av_decoder->DecodeStreamLooper();
}

void AvDecoder::DecodeStreamLooper()
{
	while(true)
	{
		BinaryData* data = NULL;
		if (WaitForSingleObject(h_decode_event_, INFINITE) == WAIT_OBJECT_0)
		{
		}
		if (!packet_queue_.empty())
		{
			data = packet_queue_.front();
			packet_queue_.pop();
		}
		::SetEvent(h_decode_event_);
		if (data)
		{
			DoDecodeVideoStream(data->data_, data->len_);
			delete data;
			data = NULL;
		}
		Sleep(10);
	}
}

void AvDecoder::DecodeVideoStream(const char* data, int length)
{
	WaitForSingleObject(h_decode_event_, INFINITE);
	packet_queue_.push(new BinaryData(data, length));
	::SetEvent(h_decode_event_);
}

void AvDecoder::DecodeAudioStream(const char* data, int length)
{

}

void AvDecoder::DoDecodeVideoStream(const char* data, int length)
{
	H264_DEC_FRAME_S decframe;
	int result = -1;
	if (h264_decoder_)
	{
		result = h264_decoder_->Decode((unsigned char*)data, length, &decframe);
		if (result != 1)
		{
			result = h264_decoder_->Decode(NULL, 0, &decframe);
		}
	}

	if (result == 1)
	{
		if (av_player_)
		{
			av_player_->OnVideoData(decframe.pY, decframe.pU, decframe.pV, decframe.uWidth, decframe.uHeight);
		}
	}
}

void AvDecoder::DoDecodeAudioStream(const char* data, int length)
{

}
