#include "StdAfx.h"
#include "AvPlayer.h"
#include "AvDecoder.h"
#include "AvPlayerObserver.h"
#include "SubscribeConnection.h"
#include "Parser.h"

AvPlayer::AvPlayer(void)
{
	subscribe_connection_ = NULL;
	av_decoder_ = NULL;
	av_player_observer_ = NULL;
}

AvPlayer::~AvPlayer(void)
{
	if (subscribe_connection_)
	{
		delete subscribe_connection_;
		subscribe_connection_ = NULL;
	}
	if (av_decoder_)
	{
		delete av_decoder_;
		av_decoder_ = NULL;
	}
}

void AvPlayer::InitObserver(AvPlayerObserver* observer)
{
	av_player_observer_ = observer;
}

void AvPlayer::UninitObserver()
{
	av_player_observer_ = NULL;
}

void AvPlayer::StartPlayRtmp(const char* url)
{
	subscribe_connection_ = new RtmpSubscribeConnection(this);
	Parser* parser = new RtmpParser(subscribe_connection_);
	subscribe_connection_->SetParser(parser);
	av_decoder_ = new AvDecoder(this);

	subscribe_connection_->Init(url);
	subscribe_connection_->Connect();
}

void AvPlayer::OnAvStreamCallback(const char* data, int length)
{
	if (av_decoder_)
	{
		av_decoder_->DecodeVideoStream(data, length);
	}
}

void AvPlayer::OnRawData(const char* data, int length)
{
	if (av_player_observer_)
	{
		av_player_observer_->OnRawData(data, length);
	}
}

void AvPlayer::OnVideoData(const unsigned char* y, const unsigned char* u, const unsigned char* v, int width, int height)
{
	if (av_player_observer_)
	{
		av_player_observer_->OnVideoData(y, u, v, width, height);
	}
}

void AvPlayer::OnAudioData(const char* data, int length)
{
	if (av_player_observer_)
	{
		av_player_observer_->OnAudioData(data, length);
	}
}
