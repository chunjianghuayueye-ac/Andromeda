#ifndef ANM2DR_VIDEO_VIDEOUTILS
#define ANM2DR_VIDEO_VIDEOUTILS

#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"
#include <iostream>

namespace anm2dr {
	namespace video {
		enum output_mode
		{
			OUTPUT_NONE=-1, OUTPUT_VIDEO_BUFFER, OUTPUT_VIDEO_FILE, OUTPUT_BITSTREAM_BUFFER, OUTPUT_BITSTREAM_FILE
		};

		typedef void (*packet_proc)(AVCodecContext*,AVFormatContext*,AVPacket*);
		typedef void (*frame_proc)(AVCodecContext*,AVFormatContext*,AVFrame*);

		void encode(AVCodecContext *cc,AVFrame *frm,AVFormatContext* fc,AVStream *st,AVPacket *pkt,long int* count,packet_proc p);
		void encode_flush(AVCodecContext *cc,AVFormatContext* fc,AVStream *st,AVPacket *pkt,packet_proc p);
		void decode(AVCodecContext *cc,AVPacket* pkt,AVFormatContext* fc,AVFrame *frm,frame_proc p);
		void decode_flush(AVCodecContext *cc,AVFormatContext* fc,AVFrame *frm,frame_proc p);

		inline char* getErrMsg(int errnum)
		{
			char msg[AV_ERROR_MAX_STRING_SIZE]={};
			av_make_error_string(msg,AV_ERROR_MAX_STRING_SIZE,errnum);
			return msg;
		}

		inline void logEncodingStatus(AVFrame* frm,AVPacket* pkt)
		{
			std::cout<<"Writing Frame "<<frm->pts<<" : pts = "<<(pkt->pts)<<" , dts = "<<(pkt->dts)<<" ."<<std::endl;
		}

		void addStream(AVFormatContext *fc,AVCodec* c,int num);
		int addStreamUpTo(AVFormatContext *fc,AVCodec* c,int lim);
		void initVideoStream(AVFormatContext *fc,AVCodecContext *cc,AVCodec *c,int pos);

		void writeBytesToFile(const char* filename,void *data,long int length);
		void appendBytesToFile(const char* filename,void *data,long int length);
		void packBitsToBytes(unsigned char* bits,int bits_start_pos,unsigned char* bytes,int bytes_start_pos,long int bits_length);
	}
}
#endif // ANM2DR_VIDEO_VIDEOUTILS
