#include "StdAfx.h"
#include "SubscribeConnection.h"
#include "Parser.h"
#include "AvPlayer.h"

extern "C"
{
#include "librtmp/rtmp.h"
}
static const int kBufReadLen = 100*1024; //100K

SubscribeConnection::SubscribeConnection(AvPlayer* av_player)
	: av_player_(av_player),
	  parser_(NULL),
	  buf_read_(NULL),
	  rsize_(0),
	  b_connected(false),
	  h_stream_thread_(NULL)
{
	buf_read_ = new char[kBufReadLen];
	h_stream_thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveStreamThreadFunction, this, 0, NULL);
}


SubscribeConnection::~SubscribeConnection(void)
{
	if (buf_read_)
	{
		delete[] buf_read_;
		buf_read_ = NULL;
	}
	if (h_stream_thread_)
	{
		CloseHandle(h_stream_thread_);
		h_stream_thread_ = NULL;
	}
	b_connected = false;
	if (parser_)
	{
		delete parser_;
		parser_ = NULL;
	}
}

void SubscribeConnection::ReceiveStreamThreadFunction(LPVOID lpParameter)
{
	SubscribeConnection* connection = (SubscribeConnection*) lpParameter;
	connection->ReceiveStreamLooper();
}

void SubscribeConnection::ReceiveStreamLooper()
{
	while(true)
	{
		ReceiveStream();
		Sleep(10);
	}
}

void SubscribeConnection::SetParser(Parser* parser)
{
	parser_ = parser;
}

void SubscribeConnection::NotifyObserver(const char* data, int length)
{
	if (parser_)
	{
		parser_->OnReceive(data, length);
	}
}

void SubscribeConnection::OnAvStreamCallback(const char* data, int length)
{
	if (av_player_)
	{
		av_player_->OnAvStreamCallback(data, length);
	}
}

// class RtmpSubscribeConnection
RtmpSubscribeConnection::RtmpSubscribeConnection(AvPlayer* av_player)
	: SubscribeConnection(av_player)
{
	rtmp_ = NULL;
}

RtmpSubscribeConnection::~RtmpSubscribeConnection(void)
{
}

void RtmpSubscribeConnection::Init(const char* url)
{
#ifdef WIN32
	WSADATA wsaData;
	int nRet;
	if ((nRet = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return;
	}
#endif

	rtmp_ = RTMP_Alloc();
	RTMP_Init(rtmp_);
	rtmp_->Link.lFlags |= RTMP_LF_LIVE;
	rtmp_->Link.timeout = 10;

	rtmp_url_ = url;
	BOOL result = RTMP_SetupURL(rtmp_, (char*)rtmp_url_.c_str());
}

void RtmpSubscribeConnection::Connect()
{
	if (!RTMP_Connect(rtmp_, NULL)) {
		return;
	}
	// connect steam
	if(!RTMP_ConnectStream(rtmp_, 0)) {
		return;
	}
	b_connected = true;
}

void RtmpSubscribeConnection::ReceiveStream()
{
	if (b_connected)
	{
		rsize_ = RTMP_Read(rtmp_, buf_read_, kBufReadLen);
		NotifyObserver(buf_read_, rsize_);
	}
}

// class RtspSubscribeConnection
RtspSubscribeConnection::RtspSubscribeConnection(AvPlayer* av_player)
	: SubscribeConnection(av_player)
{

}


RtspSubscribeConnection::~RtspSubscribeConnection(void)
{

}

void RtspSubscribeConnection::Init(const char* url)
{

}

void RtspSubscribeConnection::Connect()
{

}

void RtspSubscribeConnection::ReceiveStream()
{

}