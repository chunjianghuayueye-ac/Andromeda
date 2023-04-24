#ifndef ANM2DR_VIDEO_MUXER
#define ANM2DR_VIDEO_MUXER

#include "../../lib/av_codec/avcodec/include/libavcodec/avcodec.h"
#include "../../lib/av_codec/avcodec/include/libavformat/avformat.h"

namespace anm2dr {
	namespace image {
		class RasterImage;
	}

	using namespace anm2dr::image;

	namespace video {
		class VideoCodec;
		class AudioCodec;
		class VideoFrameInfo;
		class VideoProperties;

		class Muxer
		{
			public:
				Muxer(VideoCodec* vc,AudioCodec* ac);
				~Muxer();

				bool construct(VideoCodec* vc,AudioCodec* ac);
				void deconstruct();

				void applySettings(void);

				void openOutputVideoFile(const char* file);
				void closeOutputVideoFile(void);

				void inputVideoFrame(RasterImage& img,int last_frame_count);
				void inputVideoFrames(VideoFrameInfo* infos,int length);

				int getMaxNumberOfStreams();
				void setVideoStreamNumber(int nb);
				void setAudioStreamNumber(int nb);
				void setSubtileStreamNumber(int nb);
				void selectVideoStreamIndex(int new_idx);
				void selectAudioStreamIndex(int new_idx);
				void selectSubtileStreamIndex(int new_idx);

			protected:
				int thisVideoStreamIdx,thisAudioStreamIdx,thisSubtitleStreamIdx;
				VideoCodec* video_codec;
				AudioCodec* audio_codec;
				AVPacket* packet;
				AVFormatContext *format_context;
		};
	}
}

#include  "VideoCodec.hpp"

using namespace anm2dr::video;


inline int Muxer::getMaxNumberOfStreams(){
	return video_codec->properties->nb_video_stream+video_codec->properties->nb_audio_stream+video_codec->properties->nb_subtitle_stream;
}
inline void Muxer::setVideoStreamNumber(int nb)
{
	video_codec->properties->nb_video_stream=nb;
}
inline void Muxer::setAudioStreamNumber(int nb)
{
	video_codec->properties->nb_audio_stream=nb;
}
inline void Muxer::setSubtileStreamNumber(int nb)
{
	video_codec->properties->nb_subtitle_stream=nb;
}
inline void Muxer::selectVideoStreamIndex(int new_idx)
{
	thisVideoStreamIdx=new_idx;
}
inline void Muxer::selectAudioStreamIndex(int new_idx)
{
	thisAudioStreamIdx=video_codec->properties->nb_video_stream+new_idx;
}
inline void Muxer::selectSubtileStreamIndex(int new_idx)
{
	thisSubtitleStreamIdx=video_codec->properties->nb_video_stream+video_codec->properties->nb_audio_stream+new_idx;
}

#endif // ANM2DR_VIDEO_MUXER
