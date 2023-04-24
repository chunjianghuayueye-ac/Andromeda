#include "PCMAudio.hpp"

#include <malloc.h>
#include <string.h>
#include <math.h>

#include "PCMAudio.hpp"

#include "../macros/Math.h"
#include "AudioUtils.hpp"
#include "../util/LinkedChunkBuffer.hpp"
#include "../util/RefcPointer.hpp"
#include "Sample.hpp"

using namespace anm2dr::audio;
using namespace anm2dr::util;

PCMAudio::PCMAudio(int br,int sr,int sb,int nbc,long int data_size_,RefcPointer data):
    bit_rate(br),sample_rate(sr),sample_size(sb),nb_channels(nbc),data_size(data_size_),sample_data(data),sample_count(data_size_/sample_size)
{}

PCMAudio& PCMAudio::mix(PCMAudio& aud1,PCMAudio& aud2,long int pos_off)
{
    PCMAudio *mixed_audio=new PCMAudio(aud1.bit_rate,aud1.sample_rate,aud1.sample_size,getBiggerNumber(aud1.nb_channels,aud2.nb_channels),getMixedAudioSize(aud1.sample_count,aud2.sample_count,pos_off));
    memcpy(mixed_audio->sample_data.mem,aud1.sample_data.mem,getSmallerNumber(sizeof(Sample)*pos_off,aud1.data_size));
    if(pos_off>aud1.sample_count)
    {
        memset(mixed_audio->sample_data.mem+aud1.data_size,0,sizeof(Sample)*pos_off-aud1.data_size);
        memcpy(mixed_audio->sample_data.mem+sizeof(Sample)*pos_off,aud2.sample_data.mem,aud2.data_size);
        return *mixed_audio;
    }
    Sample* samples=(Sample*)(mixed_audio->sample_data.mem),*samples1=(Sample*)(aud1.sample_data.mem),*samples2=(Sample*)(aud2.sample_data.mem);
    if(pos_off+aud2.sample_count>aud1.sample_count)
    {
        for(int pos=pos_off,pos2=0; pos<aud1.sample_count; ++pos,++pos2)
            *(samples+pos)=*(samples1+pos)+(*(samples2+pos2));
        memcpy(mixed_audio->sample_data.mem+aud1.data_size,aud2.sample_data.mem+aud1.data_size-sizeof(Sample)*pos_off,aud2.data_size-aud1.data_size+sizeof(Sample)*pos_off);
        return *mixed_audio;
    }
    for(int pos=pos_off,pos2=0; pos2<aud2.sample_count; ++pos,++pos2)
        *(samples+pos)=*(samples1+pos)+(*(samples2+pos2));
    memcpy(mixed_audio->sample_data.mem+sizeof(Sample)*pos_off+aud2.data_size,aud1.sample_data.mem+sizeof(Sample)*pos_off+aud2.data_size,aud1.data_size-sizeof(Sample)*pos_off-aud2.data_size);
    return *mixed_audio;
}

void PCMAudio::mulVolumeFactor(float factor)
{
    Sample* samples=(Sample*)(sample_data.mem);
    for(int pos=0; pos<sample_count; ++pos)
    {
        Sample &this_sample=*(samples+pos);
        this_sample.l=getS16SampleInt(this_sample.l*factor);
        this_sample.r=getS16SampleInt(this_sample.r*factor);
    }
}

short int** PCMAudio::departChannels()
{
    short int* left=new short int[data_size/2];
    short int* right=new short int[data_size/2];
    short int** channels=new short int*[2] {left,right};
    for(int pos=0; pos<sample_count; ++pos)
    {
        *(left+pos)=getSample(pos).l;
        *(right+pos)=getSample(pos).r;
    }
    return channels;
}

PCMAudio& PCMAudio::mulSpeedFactor(PCMAudio& audio,float factor)
{
    PCMAudio *speeded_audio=new PCMAudio(audio.bit_rate,audio.sample_rate,audio.sample_size,audio.nb_channels);
    audio_operation opt=AUDIO_ORDER_REGULAR;
    if(factor==0)
    {
        return *speeded_audio;
    }
    if(factor<0)
    {
        opt=AUDIO_ORDER_RESERVE;
        factor=-factor;
    }
    double dest_relative_sample_size=1.0/factor;
    int dest_data_sample_count=ceil(audio.sample_count*dest_relative_sample_size);
    Sample *audio_data=(Sample*)malloc(sizeof(Sample)*dest_data_sample_count);
    speeded_audio->sample_count=dest_data_sample_count;
    speeded_audio->data_size=sizeof(Sample)*dest_data_sample_count;
    double relative_pos=0;
    long long last_relative_pos_q=0;
    double dest_this_sample_l_sum=0;
    double dest_this_sample_r_sum=0;
    double cross_ratio=0;
    double last_rest_cross_ratio=1;
    int crossed_raw_sample_count=0;
    int dest_pos=0;
    int raw_pos=0;
    switch(opt)
    {
    case AUDIO_ORDER_REGULAR:
        if(dest_relative_sample_size==1)
        {
            for(raw_pos=0,dest_pos=0; raw_pos<audio.sample_count; ++raw_pos,++dest_pos)
            {
                Sample &this_raw_sample=audio.getSample(raw_pos);
                Sample &this_dest_sample=*(audio_data+dest_pos);
                this_dest_sample.l=this_raw_sample.l;
                this_dest_sample.r=this_raw_sample.r;
            }
            break;
        }
        raw_pos=0;
        relative_pos+=dest_relative_sample_size;
        do
        {
            Sample &this_raw_sample=audio.getSample(raw_pos);
            cross_ratio=relative_pos-(long int)relative_pos;
            if(relative_pos-last_relative_pos_q>1)
            {
                if(dest_relative_sample_size<1)
                {
                    dest_this_sample_l_sum+=this_raw_sample.l*(dest_relative_sample_size-cross_ratio);
                    dest_this_sample_r_sum+=this_raw_sample.r*(dest_relative_sample_size-cross_ratio);
                    ++crossed_raw_sample_count;
                    Sample &this_dest_sample=*(audio_data+dest_pos);
                    ++dest_pos;
                    this_dest_sample.l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
                    this_dest_sample.r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
                    if(cross_ratio>0)
                    {
                        crossed_raw_sample_count=1;
                        dest_this_sample_l_sum=cross_ratio*this_raw_sample.l;
                        dest_this_sample_r_sum=cross_ratio*this_raw_sample.r;
                    }
                    else
                    {
                        crossed_raw_sample_count=0;
                        dest_this_sample_l_sum=0;
                        dest_this_sample_r_sum=0;
                    }
                }
                else if(dest_relative_sample_size>1)
                {
                    dest_this_sample_l_sum+=this_raw_sample.l*last_rest_cross_ratio;
                    dest_this_sample_r_sum+=this_raw_sample.r*last_rest_cross_ratio;
                    ++crossed_raw_sample_count;
                    Sample &first_dest_sample=*(audio_data+dest_pos);
                    ++dest_pos;
                    first_dest_sample.l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
                    first_dest_sample.r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
                    for(int i=2; i<ceil(relative_pos)-last_relative_pos_q; ++i,++dest_pos)
                    {
                        Sample &same_dest_sample=*(audio_data+dest_pos);
                        same_dest_sample.l=this_raw_sample.l;
                        same_dest_sample.r=this_raw_sample.r;
                    }
                    last_rest_cross_ratio=1-cross_ratio;
                    dest_this_sample_l_sum=this_raw_sample.l*cross_ratio;
                    dest_this_sample_r_sum=this_raw_sample.r*cross_ratio;
                    if(cross_ratio==0)
                    {
                        crossed_raw_sample_count=0;
                        Sample &last_dest_sample=*(audio_data+dest_pos);
                        last_dest_sample.l=this_raw_sample.l;
                        last_dest_sample.r=this_raw_sample.r;
                        ++dest_pos;
                    }
                    else
                    {
                        crossed_raw_sample_count=1;
                    }
                }
                goto CONTINUE_REG;
            }
            else
            {
                dest_this_sample_l_sum+=this_raw_sample.l;
                dest_this_sample_r_sum+=this_raw_sample.r;
                ++crossed_raw_sample_count;
            }
CONTINUE_REG:
            ++raw_pos;
            last_relative_pos_q=relative_pos;
            relative_pos+=dest_relative_sample_size;
        }
        while(raw_pos<audio.sample_count);
        (*(audio_data+dest_data_sample_count-1)).l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
        (*(audio_data+dest_data_sample_count-1)).r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
        break;
    case AUDIO_ORDER_RESERVE:
        if(dest_relative_sample_size==1)
        {
            for(raw_pos=audio.sample_count-1,dest_pos=0; raw_pos>=0; --raw_pos,++dest_pos)
            {
                Sample &this_raw_sample=audio.getSample(raw_pos);
                Sample &this_dest_sample=*(audio_data+dest_pos);
                this_dest_sample.l=this_raw_sample.l;
                this_dest_sample.r=this_raw_sample.r;
            }
            break;
        }
        raw_pos=audio.sample_count-1;
        relative_pos+=dest_relative_sample_size;
        do
        {
            Sample &this_raw_sample=audio.getSample(raw_pos);
            cross_ratio=relative_pos-(long int)relative_pos;
            if(relative_pos-last_relative_pos_q>1)
            {
                if(dest_relative_sample_size<1)
                {
                    dest_this_sample_l_sum+=this_raw_sample.l*(dest_relative_sample_size-cross_ratio);
                    dest_this_sample_r_sum+=this_raw_sample.r*(dest_relative_sample_size-cross_ratio);
                    ++crossed_raw_sample_count;
                    Sample &this_dest_sample=*(audio_data+dest_pos);
                    ++dest_pos;
                    this_dest_sample.l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
                    this_dest_sample.r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
                    if(cross_ratio>0)
                    {
                        crossed_raw_sample_count=1;
                        dest_this_sample_l_sum=cross_ratio*this_raw_sample.l;
                        dest_this_sample_r_sum=cross_ratio*this_raw_sample.r;
                    }
                    else
                    {
                        crossed_raw_sample_count=0;
                        dest_this_sample_l_sum=0;
                        dest_this_sample_r_sum=0;
                    }
                }
                else if(dest_relative_sample_size>1)
                {
                    dest_this_sample_l_sum+=this_raw_sample.l*last_rest_cross_ratio;
                    dest_this_sample_r_sum+=this_raw_sample.r*last_rest_cross_ratio;
                    ++crossed_raw_sample_count;
                    Sample &first_dest_sample=*(audio_data+dest_pos);
                    ++dest_pos;
                    first_dest_sample.l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
                    first_dest_sample.r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
                    for(int i=2; i<ceil(relative_pos)-last_relative_pos_q; ++i,++dest_pos)
                    {
                        Sample &same_dest_sample=*(audio_data+dest_pos);
                        same_dest_sample.l=this_raw_sample.l;
                        same_dest_sample.r=this_raw_sample.r;
                    }
                    last_rest_cross_ratio=1-cross_ratio;
                    dest_this_sample_l_sum=this_raw_sample.l*cross_ratio;
                    dest_this_sample_r_sum=this_raw_sample.r*cross_ratio;
                    if(cross_ratio==0)
                    {
                        crossed_raw_sample_count=0;
                        Sample &last_dest_sample=*(audio_data+dest_pos);
                        last_dest_sample.l=this_raw_sample.l;
                        last_dest_sample.r=this_raw_sample.r;
                        ++dest_pos;
                    }
                    else
                    {
                        crossed_raw_sample_count=1;
                    }
                }
                goto CONTINUE_RES;
            }
            else
            {
                dest_this_sample_l_sum+=this_raw_sample.l;
                dest_this_sample_r_sum+=this_raw_sample.r;
                ++crossed_raw_sample_count;
            }
CONTINUE_RES:
            --raw_pos;
            last_relative_pos_q=relative_pos;
            relative_pos+=dest_relative_sample_size;
        }
        while(raw_pos>=0);
        (*(audio_data+dest_data_sample_count-1)).l=getS16SampleInt(dest_this_sample_l_sum/crossed_raw_sample_count);
        (*(audio_data+dest_data_sample_count-1)).r=getS16SampleInt(dest_this_sample_r_sum/crossed_raw_sample_count);
        break;
    }
    speeded_audio->setSampleData(audio_data);
    return *speeded_audio;
}
