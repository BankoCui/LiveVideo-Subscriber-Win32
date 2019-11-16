#pragma once

class SubscribeConnection;
class AvDecoder;
class AvPlayerObserver;

class AvPlayer
{
public:
	AvPlayer(void);
	~AvPlayer(void);
	void InitObserver(AvPlayerObserver* observer);
	void UninitObserver();
	void StartPlayRtmp(const char* url);
	// callback from SubscribeConnection
	void OnAvStreamCallback(const char* data, int length);
	// callback from AvDecoder
	void OnRawData(const char* data, int length);
	void OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height);
	void OnAudioData(const char* data, int length);

private:
	SubscribeConnection* subscribe_connection_;
	AvDecoder* av_decoder_;
	AvPlayerObserver* av_player_observer_;
};