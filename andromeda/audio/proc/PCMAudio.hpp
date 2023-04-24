#ifndef ANM2DR_AUDIO_PCMAUDIO
#define ANM2DR_AUDIO_PCMAUDIO

#include "../declares/dec_audio/Sample.h"
#include "../util/RefcPointer.hpp"

using namespace anm2dr::util;

namespace anm2dr{
    namespace audio{
        class PCMAudio
        {
        friend PCMAudio* anm2dr::audio::readPCMAudio(const char* file);
        friend void anm2dr::audio::writePCMAudio(const char* dest_file,PCMAudio& audio);
        protected:
            RefcPointer sample_data;

        public:
            int bit_rate;
            int sample_rate;
            int sample_size;
            int nb_channels;
            int sample_count;
            long int data_size;

            PCMAudio(int br=320000,int sr=0,int sb=0,int nbc=0,long int data_size_=0,RefcPointer data=0);

            RefcPointer getSampleDataMem();
            unsigned char* getSampleDataPtr();
            void setSampleData(void* data);
            Sample& getSample(int sp_idx);

            void mulVolumeFactor(float factor);
            short int** departChannels();

            static PCMAudio& mix(PCMAudio& aud1,PCMAudio& aud2,long int pos_off);
            static PCMAudio& mulSpeedFactor(PCMAudio& audio,float factor);
        };
    }
}

#include "Sample.hpp"

using namespace anm2dr::audio;

inline RefcPointer PCMAudio::getSampleDataMem()
{
    return sample_data;
}

inline unsigned char* PCMAudio::getSampleDataPtr()
{
    return (unsigned char*)(sample_data.mem);
}

inline void PCMAudio::setSampleData(void* data)
{
    sample_data=RefcPointer(data);
}

inline Sample& PCMAudio::getSample(int sp_idx)
{
    return *((Sample*)(sample_data.mem)+sp_idx);
}

#endif // ANM2DR_AUDIO_PCMAUDIO
