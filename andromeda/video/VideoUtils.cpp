#include "VideoUtils.hpp"

#include <iostream>
#include "../macros/Codec.h"

using namespace anm2dr::video;

void anm2dr::video::initVideoStream(AVFormatContext *fc,AVCodecContext *cc,AVCodec *c,int pos)
{
    AVStream* stream=*(fc->streams)+pos;
    stream->codecpar->codec_type=AVMEDIA_TYPE_VIDEO;
    stream->codecpar->codec_id=c->id;
    stream->codecpar->width=cc->width;
    stream->codecpar->height=cc->height;
    stream->sample_aspect_ratio=(AVRational){cc->width,cc->height};
    stream->codecpar->bit_rate=cc->bit_rate;
    stream->codecpar->format=AV_PIX_FMT_YUV444P;
    stream->time_base=cc->time_base;
    stream->avg_frame_rate=cc->framerate;
    stream->id=pos;
    stream->index=pos;
    int ret=0;
    ret=avcodec_parameters_from_context((*(fc->streams)+pos)->codecpar,cc);
    if(ret<0)
    	std::cout<<"Copy parameters from video encoder context failed."<<getErrMsg(ret)<<std::endl;
}

void anm2dr::video::addStream(AVFormatContext *fc,AVCodec* c,int num)
{
	for(;num;--num)
		avformat_new_stream(fc,c);
}

int anm2dr::video::addStreamUpTo(AVFormatContext *fc,AVCodec* c,int lim)
{
	int num=0;
	for(;fc->nb_streams<lim;++num)
		avformat_new_stream(fc,c);
    return num;
}

void anm2dr::video::encode_flush(AVCodecContext *cc,AVFormatContext* fc,AVStream *st,AVPacket *pkt,packet_proc p)
{
    avcodec_send_frame(cc,nullptr);
    int ret=0;
    while(ret>=0)
    {
        ret=avcodec_receive_packet(cc,pkt);
        if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
            break;
        else if(ret<0)
        {
            std::cout<<"Receive the packet failed."<<std::endl;
            break;
        }
        if(st)
        {
            av_packet_rescale_ts(pkt,cc->time_base,st->time_base);
            pkt->stream_index=st->index;
        }
        p(cc,fc,pkt);
        av_packet_unref(pkt);
    }
}

void anm2dr::video::encode(AVCodecContext *cc,AVFrame *frm,AVFormatContext* fc,AVStream *st,AVPacket *pkt,long int* count,packet_proc p)
{
    frm->pts=*count;
    if(avcodec_send_frame(cc,frm)<0)
    {
        std::cout<<"Send the frame failed."<<std::endl;
        return;
    }
    int ret=0;
    while(ret>=0)
    {
        ret=avcodec_receive_packet(cc,pkt);
        if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
            break;
        else if(ret<0)
        {
            std::cout<<"Receive the packet failed."<<std::endl;
            break;
        }
        p(cc,fc,pkt);
    }
    ++*count;
}

void anm2dr::video::decode(AVCodecContext *cc,AVPacket* pkt,AVFormatContext* fc,AVFrame *frm,frame_proc p)
{
    if(avcodec_send_packet(cc,pkt)<0)
    {
        std::cout<<"Send the packet failed."<<std::endl;
        return;
    }
    int ret=0;
    while(ret>=0)
    {
        ret=avcodec_receive_frame(cc,frm);
        if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
            break;
        else if(ret<0)
        {
            std::cout<<"Receive the frame failed."<<std::endl;
            break;
        }
        p(cc,fc,frm);
    }
}

void anm2dr::video::decode_flush(AVCodecContext *cc,AVFormatContext* fc,AVFrame *frm,frame_proc p)
{
    avcodec_send_packet(cc,nullptr);
    int ret=0;
    while(ret>=0)
    {
        ret=avcodec_receive_frame(cc,frm);
        if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)
            break;
        else if(ret<0)
        {
            std::cout<<"Receive the frame failed."<<std::endl;
            break;
        }
        p(cc,fc,frm);
    }
}
