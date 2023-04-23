#ifndef ANM2DR_MACROS_AUDIOPROCESS
#define ANM2DR_MACROS_AUDIOPROCESS

#define getS16SampleInt(i) (i<-32768?-32768:(i>32767?32767:i))
#define sampleLinear(s,e,t,i) (getS16SampleInt((s+(double)(e-s)/t*i)))
#define getMixedAudioSize(s1,s2,off) (s2+off>s1?s2+off:s1)

#endif // ANM2DR_MACROS_AUDIOPROCESS
