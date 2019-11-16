#pragma once

class AvPlayerObserver
{
public:
	virtual void OnRawData(const char* data, int length) = 0;
	virtual void OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height) = 0;
	virtual void OnAudioData(const char* data, int length) = 0;
	virtual void OnPlayerEvent(int code, const char* content) = 0;
};