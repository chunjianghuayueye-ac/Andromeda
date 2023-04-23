#ifndef ANM2DR_AUDIO_SAMPLE
#define ANM2DR_AUDIO_SAMPLE

namespace anm2dr{
    namespace audio{
        class Sample
        {
        public:
            signed short int l,r;

            Sample(signed short int lv=0,signed short int rv=0);

            bool operator==(Sample& s);
            bool operator!=(Sample& s);
        };
    }
}

using namespace anm2dr::audio;

inline bool Sample::operator==(Sample& s)
{
    return (l==s.l)&&(r==s.r);
}

inline bool Sample::operator!=(Sample& s)
{
    return (l!=s.l)||(r!=s.r);
}

#endif // ANM2DR_AUDIO_SAMPLE
