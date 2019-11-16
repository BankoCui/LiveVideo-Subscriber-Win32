#pragma once
#include <string>

class AvPlayer;
class Parser;
struct RTMP;

class SubscribeConnection
{
public:
	SubscribeConnection(AvPlayer* av_player);
	~SubscribeConnection(void);
	void SetParser(Parser* parser);
	virtual void Init(const char* url) = 0;
	virtual void Connect() = 0;
	// callback from parser
	void OnAvStreamCallback(const char* data, int length);

protected:
	virtual void ReceiveStream() = 0;
	void NotifyObserver(const char* data, int length);
	bool b_connected;

	char* buf_read_;
	int rsize_;

private:
	static void ReceiveStreamThreadFunction(LPVOID lpParameter);
	void ReceiveStreamLooper();
	HANDLE h_stream_thread_;
	AvPlayer* av_player_;
	Parser* parser_;
};

class RtmpSubscribeConnection :
	public SubscribeConnection
{
public:
	RtmpSubscribeConnection(AvPlayer* av_player);
	~RtmpSubscribeConnection(void);
	virtual void Init(const char* url);
	virtual void Connect();

protected:
	virtual void ReceiveStream();

private:
	RTMP* rtmp_;
	std::string rtmp_url_;
};

class RtspSubscribeConnection :
	public SubscribeConnection
{
public:
	RtspSubscribeConnection(AvPlayer* av_player);
	~RtspSubscribeConnection(void);
	virtual void Init(const char* url);
	virtual void Connect();

protected:
	virtual void ReceiveStream();

private:
	std::string rtsp_url;
};