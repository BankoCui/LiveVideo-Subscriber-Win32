#pragma once

class SubscribeConnection;

class Parser
{
public:
	Parser(SubscribeConnection* connection);
	~Parser(void);
	// callback from SubscribeConnection
	void OnReceive(const char* data, int length);

protected:
	virtual void ParserStream(const char* data, int length) = 0;

	SubscribeConnection* connection_;
	char* buf_stream_;
	int buf_stream_len_;
};


class RtmpParser :
	public Parser
{
public:
	RtmpParser(SubscribeConnection* connection);
	~RtmpParser(void);

protected:
	virtual void ParserStream(const char* data, int length);

private:
	bool find_flv_head_;
	unsigned char* sps_buf_;
	unsigned char* pps_buf_;
	int sps_len_, pps_count_, pps_len_;
};


class RtspParser :
	public Parser
{
public:
	RtspParser(SubscribeConnection* connection);
	~RtspParser(void);

protected:
	virtual void ParserStream(const char* data, int length);
};