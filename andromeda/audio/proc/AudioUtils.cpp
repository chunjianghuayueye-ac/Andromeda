#include "AudioUtils.hpp"

#include <math.h>
#include <iostream>
#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"

#include "../macros/Codec.h"
#include "Sample.hpp"
#include "PCMAudio.hpp"
#include "../video/VideoUtils.hpp"
#include "../util/LinkedChunkBuffer.hpp"

using namespace anm2dr::audio;
using namespace anm2dr::video;
using namespace anm2dr::util;

PCMAudio* anm2dr::audio::readPCMAudio(const char* file)
{
    int ret=0;
    AVPacket* pkt=nullptr;
    AVFrame* frm=nullptr;
    AVCodec* c=nullptr;
    AVCodecContext* cc=nullptr;
    AVFormatContext* fc=nullptr;
    if((ret=avformat_open_input(&fc,file,nullptr,nullptr))<0)
    {
        std::cout<<"Open input file failed."<<getErrMsg(ret)<<std::endl;
        return nullptr;
    }
    if((ret=avformat_find_stream_info(fc,nullptr))<0)
    {
        std::cout<<"Find stream info failed."<<getErrMsg(ret)<<std::endl;
        return nullptr;
    }
    LinkedChunkBuffer<PCMAudio> audios(fc->nb_streams);
    frm=av_frame_alloc();
    if(!frm)
    {
        std::cout<<"Allocate frame failed."<<std::endl;
        return nullptr;
    }
    pkt=av_packet_alloc();
    if(!pkt)
    {
        std::cout<<"Allocate packet failed."<<std::endl;
        return nullptr;
    }
    for(int stream_index=0; stream_index<fc->nb_streams; ++stream_index)
    {
        if(fc->streams[stream_index]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
        {
            cc=fc->streams[stream_index]->codec;
            c=avcodec_find_decoder(cc->codec_id);
            if(!c)
            {
                std::cout<<"Find decoder failed."<<std::endl;
                return nullptr;
            }
            PCMAudio this_audio(cc->bit_rate,cc->sample_rate,getSampleSize(cc->sample_fmt),cc->channels);
            LinkedChunkBuffer<unsigned char> audio_data(2304);
            if((ret=avcodec_open2(cc,c,nullptr))<0)
            {
                std::cout<<"Open codec failed."<<getErrMsg(ret)<<std::endl;
                return nullptr;
            }
            ret=av_read_frame(fc,pkt);
            while(ret>=0)
            {
                startDecode(cc,pkt,frm)
                for(int cp=0; cp<cc->channels; ++cp)
                {
                    audio_data.put(frm->extended_data[cp],frm->linesize[cp]);
                    this_audio.data_size+=frm->linesize[cp];
                }
                endDecode(cc,pkt,frm)
                ret=av_read_frame(fc,pkt);
            }
            startDecodeFlush(cc,frm)
            for(int cp=0; cp<cc->channels; ++cp)
            {
                audio_data.put(frm->extended_data[cp],frm->linesize[cp]);
                this_audio.data_size+=frm->linesize[cp];
            }
            endDecodeFlush(cc,frm)
            this_audio.sample_count=this_audio.data_size/sizeof(Sample);
            this_audio.sample_data=RefcPointer(audio_data.toArray());
            audios.put(this_audio);
        }
    }
    avformat_close_input(&fc);
    av_free_packet(pkt);
    av_frame_free(&frm);
    avformat_free_context(fc);
    return audios.toArrayCpy();
}

void anm2dr::audio::writePCMAudio(const char* dest_file,PCMAudio& audio)
{
    int ret=0;
    AVPacket* pkt=nullptr;
    AVFrame* frm=nullptr;
    AVCodec* c=nullptr;
    AVCodecContext* cc=nullptr;
    AVFormatContext* format_context=avformat_alloc_context();
    if(!format_context)
    {
        std::cout<<"Allocate format context failed."<<std::endl;
        return;
    }
    //ret=avcodec_parameters_from_context((*(format_context->streams))->codecpar,vc->codec_context);
    if(ret<0)
        std::cout<<"Copy parameters from video encoder context failed."<<getErrMsg(ret)<<std::endl;
    strcpy(format_context->filename,dest_file);
    format_context->oformat=av_guess_format(nullptr,dest_file,nullptr);
    //format_context->audio_codec_id=ac->codec->id;
    //addStream(format_context,vc->codec_context,vc->codec);
    ret=avio_open(&format_context->pb,dest_file,AVIO_FLAG_WRITE);
    if(ret<0)
    {
        std::cout<<"Open the file failed."<<getErrMsg(ret)<<std::endl;
        return;
    }
    ret=avformat_write_header(format_context,nullptr);
    if(ret<0)
    {
        std::cout<<"Write file header failed."<<getErrMsg(ret)<<std::endl;
        return;
    }
    frm=av_frame_alloc();
    if(!frm)
    {
        std::cout<<"Allocate frame failed."<<std::endl;
        return;
    }
    pkt=av_packet_alloc();
    if(!pkt)
    {
        std::cout<<"Allocate packet failed."<<std::endl;
        return;
    }
}

void anm2dr::audio::resample(void* src,void* dest,int start_pos,int end_pos,int stride)
{
    Sample* this_src_sample=(Sample*)src+start_pos;
    Sample* this_dest_sample=(Sample*)dest;
    for(int pos=start_pos; pos<end_pos; ++pos+=stride,++this_dest_sample+=stride,++this_src_sample)
        *this_dest_sample=*this_src_sample;
}

Sample* anm2dr::audio::resample(void* src,int start_pos,int end_pos,int stride)
{
    Sample* sampled_data=(Sample*)malloc(sizeof(Sample)*ceil((double)(1+start_pos-end_pos)/(1+stride)));
    resample(src,sampled_data,start_pos,end_pos,stride);
    return sampled_data;
}

void resample(void* src,void* dest,int length,int stride)
{
    resample(src,dest,0,length,stride);
}

Sample* resample(void* src,int length,int stride)
{
    return resample(src,0,length,stride);
}
