#include "StdAfx.h"
#include "Parser.h"
#include "SubscribeConnection.h"

extern "C"
{
#include "librtmp/rtmp.h"
}

#define MAX_PARSER_LEN 512*1024

Parser::Parser(SubscribeConnection* connection)
	: connection_(connection)
{
	buf_stream_ = new char[MAX_PARSER_LEN];
	buf_stream_len_ = 0;
}

Parser::~Parser(void)
{
	if (buf_stream_)
	{
		delete[] buf_stream_;
		buf_stream_ = NULL;
	}
}

void Parser::OnReceive(const char* data, int length)
{
	memcpy(buf_stream_ + buf_stream_len_, data, length);
	buf_stream_len_ += length;
	ParserStream(data, length);
}


// class RtmpParser
static const char span[4] = { 0, 0, 0, 1 };

RtmpParser::RtmpParser(SubscribeConnection* connection)
	: Parser(connection)
{
	find_flv_head_ = false;
	sps_buf_ = NULL;
	pps_buf_ = NULL;
	sps_len_ = 0;
	pps_len_ = 0;
	pps_count_ = 0;
}

RtmpParser::~RtmpParser(void)
{
	if (sps_buf_)
	{
		delete[] sps_buf_;
		sps_buf_ = NULL;
	}
	if (pps_buf_)
	{
		delete[] pps_buf_;
		pps_buf_ = NULL;
	}
}

void RtmpParser::ParserStream(const char* data, int length)
{
	uint8_t packetType;
	uint32_t nBodySize;
	uint32_t nTimeStamp;

	while (1) {
		char* ptr = buf_stream_;
		if (!find_flv_head_) {
			if (buf_stream_len_ < 13)
				break;
			if (ptr[0] == 'F' && ptr[1] == 'L' && ptr[2] == 'V') {
				find_flv_head_ = true;
				packetType = ptr[4];
				if (packetType & 0x01) {
					// has video
				}
				if(packetType & 0x04) {
					// has audio
				}
				buf_stream_len_ -= 13; // TODO hard coding
				memmove(buf_stream_, buf_stream_+13, buf_stream_len_);
				break;
			}
			ptr = buf_stream_;
		}

		if (buf_stream_len_ < 11) // TODO hard coding
			break;

		packetType = *ptr++;
		nBodySize = AMF_DecodeInt24((char*)ptr);
		ptr += 3;
		nTimeStamp = AMF_DecodeInt24((char*)ptr);
		ptr += 3;
		nTimeStamp |= *ptr++ << 24;
		ptr += 3;

		if (nBodySize + (ptr-buf_stream_) + 4 > buf_stream_len_)  // 数据不完整
			break;

		switch (packetType)
		{
		case RTMP_PACKET_TYPE_AUDIO:
			{

			}
			break;
		case RTMP_PACKET_TYPE_VIDEO:
			{
				char* ptr_frame = ptr;
				char* frame = NULL;
				if (ptr[1] == 0) { // MetaData
					ptr_frame += 11;
					sps_len_ = AMF_DecodeInt16((char*)ptr_frame);
					ptr_frame += 2;
					if (!sps_buf_) {
						sps_buf_ = (unsigned char*)(new char[sps_len_]);
					}
					memcpy(sps_buf_, ptr_frame, sps_len_);

					ptr_frame += sps_len_;
					pps_count_ = static_cast<int>(ptr_frame[0]);
					ptr_frame += 1;
					pps_len_ = AMF_DecodeInt16((char*)ptr_frame);
					ptr_frame += 2;
					if (!pps_buf_) {
						pps_buf_ = (unsigned char*)(new char[pps_len_]);
					}
					memcpy(pps_buf_, ptr_frame, pps_len_);

				} else if (ptr[1] == 1) { // Video (I/P) 
					int frame_len = 0;
					int frame_type = static_cast<int>(ptr[0]) >> 4&0xf;
					if (frame_type == 1) { // I Frame
						frame_len = 4+sps_len_+4+pps_len_+4+nBodySize-9;
						frame = new char[frame_len];
						memcpy(frame, span, 4);
						memcpy(frame+4, sps_buf_, sps_len_);
						memcpy(frame+4+sps_len_, span, 4);
						memcpy(frame+4+sps_len_+4, pps_buf_, pps_len_);
						memcpy(frame+4+sps_len_+4+pps_len_, span, 4);
						memcpy(frame+4+sps_len_+4+pps_len_+4, ptr+9, nBodySize-9);
					} else { // P Frame
						frame_len = 4+nBodySize-9;
						frame = new char[frame_len];
						memcpy(frame, span, 4);
						memcpy(frame+4, ptr+9, nBodySize-9);
					}

					connection_->OnAvStreamCallback(frame, frame_len);
				}

				if (frame) {
					delete []frame;
					frame = NULL;
				}
			}
			break;
		default:
			break;
		}
		buf_stream_len_ = buf_stream_len_ - ((ptr - buf_stream_) + nBodySize) - 4;
		memmove(buf_stream_, ptr + nBodySize + 4, buf_stream_len_);
	}
}


// class RtspParser
RtspParser::RtspParser(SubscribeConnection* connection)
	: Parser(connection)
{
}

RtspParser::~RtspParser(void)
{
}

void RtspParser::ParserStream(const char* data, int length)
{

}