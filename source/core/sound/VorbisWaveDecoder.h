#pragma once
#include "WaveIntf.h"

#ifdef TVP_AUDIO_ENABLE_VORBIS
class VorbisWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    //VorbisWaveDecoderCreator() { TVPRegisterWaveDecoderCreator(this); }
    tTVPWaveDecoder * Create(const ttstr & storagename, const ttstr & extension);
};
#endif

#ifdef TVP_AUDIO_ENABLE_OPUS
class OpusWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    //VorbisWaveDecoderCreator() { TVPRegisterWaveDecoderCreator(this); }
    tTVPWaveDecoder * Create(const ttstr & storagename, const ttstr & extension);
};
#endif
