#ifndef ANM2DR_AUDIO_AUDIOUTILS
#define ANM2DR_AUDIO_AUDIOUTILS

#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"

#include "../macros/AudioProcess.h"
#include "Sample.hpp"

namespace anm2dr{
    namespace audio{
        class PCMAudio;
        class Sample;

        enum audio_channel
        {
            AUDIO_CHANNEL_LEFT=0,AUDIO_CHANNEL_RIGHT=1
        };

        enum audio_operation
        {
            AUDIO_OPERATION_NONE=-1,AUDIO_ORDER_RESERVE,AUDIO_ORDER_REGULAR
        };

        extern PCMAudio* readPCMAudio(const char* file);
        extern void writePCMAudio(const char* dest_file,PCMAudio& audio);
        extern void resample(void* src,void* dest,int start_pos,int end_pos,int stride);
        extern Sample* resample(void* src,int start_pos,int end_pos,int stride);
        extern void resample(void* src,void* dest,int length,int stride);
        extern Sample* resample(void* src,int length,int stride);

        inline int getSampleSize(AVSampleFormat fmt)
        {
            switch(fmt)
            {
            case AV_SAMPLE_FMT_S16:
            case AV_SAMPLE_FMT_S16P:
                return 16;
            case AV_SAMPLE_FMT_S32:
            case AV_SAMPLE_FMT_S32P:
                return 32;
            case AV_SAMPLE_FMT_S64:
            case AV_SAMPLE_FMT_S64P:
                return 64;
            case AV_SAMPLE_FMT_U8:
            case AV_SAMPLE_FMT_U8P:
                return 8;
            }
            return -1;
        }

        inline Sample operator+(Sample& s1,Sample& s2)
        {
            return Sample(getS16SampleInt(s1.l+s2.l),getS16SampleInt(s1.r+s2.r));
        }
    }
}
#endif // ANM2DR_AUDIO_AUDIOUTILS
