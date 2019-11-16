#pragma once
#include <queue>

class AvPlayer;
class H264decoder;

class BinaryData
{
public:
	BinaryData(const char* data, int len)
		: len_(len)
	{
		data_ = new char[len];
		memcpy(data_, data, len);
	}
	~BinaryData()
	{
		delete[] data_;
		data_ = NULL;
	}

	char* data_;
	int len_;
};

class AvDecoder
{
public:
	AvDecoder(AvPlayer* av_player);
	~AvDecoder(void);
	void DecodeVideoStream(const char* data, int length);
	void DecodeAudioStream(const char* data, int length);

private:
	static void DecodeStreamThreadFunction(LPVOID lpParameter);
	void DecodeStreamLooper();
	void DoDecodeVideoStream(const char* data, int length);
	void DoDecodeAudioStream(const char* data, int length);
	std::queue<BinaryData*> packet_queue_;
	HANDLE h_decode_event_;
	HANDLE h_decode_thread_;
	AvPlayer* av_player_;
	H264decoder* h264_decoder_;
};

