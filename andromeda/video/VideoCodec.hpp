#ifndef ANM2DR_VIDEO_VIDEOCODEC
#define ANM2DR_VIDEO_VIDEOCODEC

#include <fstream>
#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"

#include "../declares/dec_image/RasterImage.h"
#include "../declares/dec_video/Muxer.h"

using namespace anm2dr::image;

namespace anm2dr {
	namespace video {
		class VideoProperties
		{
			public:
				int width,height;
				int frame_rate_num,frame_rate_den;
				int timebase_num=0,timebase_den=0;
				int bit_rate;
				int gop_size;
				int max_b_frames;
				int nb_video_stream,nb_audio_stream,nb_subtitle_stream;
		};

		class VideoFrameInfo
		{
			public:
				RasterImage *img;
				int last_frame_count;

				VideoFrameInfo(RasterImage& image);
				VideoFrameInfo(RasterImage& image,int lfc);
		};

		class VideoCodec
		{
				friend class Muxer;

			protected:
				AVCodec *codec;
				AVCodecContext *codec_context;
				AVFrame *frame;
				AVPacket *packet;
				std::ofstream* output_file;
				VideoProperties *properties;
				bool is_buf_alloc;
				long int encoded_frame_count,packet_count;

				bool init_codec(AVCodecID &id);
				bool construct(AVCodecID &id);
				void deconstruct();
			public:
				VideoCodec(AVCodecID &id);
				VideoCodec(AVCodecID &&id);
				~VideoCodec();

				void inputFrame(RasterImage& img,int last_frame_count);
				void inputFrames(VideoFrameInfo* infos,int length);

				void applySettings(void);

				void setFrameSize(int width,int height);
				void setFrameRate(int numerator,int denominator);
				void setBitRate(int rate);
				void setGopSize(int gopsize);
				void setTimeBase(int numerator,int denominator);
				void setMaxBFrames(int max);
				void setProperties(VideoProperties *pro);
				void openOutputBitstreamFile(const char* file);
				void closeOutputBitstreamFile(void);

				static void loadYUV444PDataFromRasterImageToFrame(RasterImage &img,AVFrame *frame_);

		};
	}
}

#include "../image/RasterImage.hpp"

using namespace anm2dr::video;
using namespace anm2dr::image;

inline void VideoCodec::setFrameSize(int width,int height)
{
	properties->width=width;
	properties->height=height;
}

inline void VideoCodec::setFrameRate(int numerator,int denominator)
{
	properties->frame_rate_num=numerator;
	properties->frame_rate_den=denominator;
}

inline void VideoCodec::setTimeBase(int numerator,int denominator)
{
	properties->timebase_num=numerator;
	properties->timebase_den=denominator;
}

inline void VideoCodec::setBitRate(int rate)
{
	properties->bit_rate=rate;
}

inline void VideoCodec::setGopSize(int gopsize)
{
	properties->gop_size=gopsize;
}

inline void VideoCodec::setMaxBFrames(int max)
{
	properties->max_b_frames=max;
}

inline void VideoCodec::setProperties(VideoProperties *pro)
{
	properties=pro;
}

#endif // ANM2DR_VIDEO_VIDEOCODEC
