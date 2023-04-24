#ifndef ANM2DR_VIDEO_AUDIOCODEC
#define ANM2DR_VIDEO_AUDIOCODEC

#include <fstream>
#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"
#include "../audio/PCMAudio.hpp"

namespace anm2dr{
    namespace video{
        class AudioCodec
        {
        friend class Muxer;

        public:
            typedef struct AudioProperties{
                int bit_rate;
                int sample_rate;
                int sample_bit;
            } AudioProperties;

            typedef struct AudioFrameInfo{
                int bit_rate;
            } AudioFrameInfo;

            AudioCodec();
            AudioCodec(AVCodecID &id);
            AudioCodec(AVCodecID &&id);
            ~AudioCodec();

            bool construct(AVCodecID &id);
            void deconstruct();

            void openOutputAudioFile(const char* name);
            void closeOutputAudioFile();
            void encodePCMStreamToAudioFile(const char* name,PCMAudio pcm);

        protected:
            AVCodec *codec;
            AVCodecContext *codec_context;
            AVFrame *frame;
            AVPacket *packet;
            std::ofstream* output_file;
            AudioProperties *properties;
            bool is_buf_alloc;
            long int encoded_frame_count;
        };
    }
}

#endif // ANM2DR_VIDEO_AUDIOCODEC
