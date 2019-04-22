/*
 *
 * Copyright 2013-2014 by MotionPortrait, Inc.
 *
 * All rights reserved.
 *
 */

#ifndef MPCTLSPEECH_H_
#define MPCTLSPEECH_H_

#include "mptypes.h"

#ifdef WEBGL
#include <string>
#endif

namespace motionportrait {


class MpCtlSpeechImpl;

/**
 * \class MpCtlSpeech
 *
 * MpCtlSpeech class controls lip synch of an avatar
 */
class MpCtlSpeech {

  public:

    /** voice data id */
#ifdef WEBGL
    typedef intptr_t VoiceId;
#else
    typedef void* VoiceId;
#endif

    /**
     * create voice data
     *
     * @param pathVoice : path to voice file.
     *                   voice file must be WAV or ENV format.
     *                   ENV file can be converted from WAV by MPAnavoice tool.
     * @return voice data id
     */
    VoiceId CreateVoice(const char *pathVoice);

    /**
     * voice buffer data structure
     */
    typedef struct {
        int nChannel;           /**< number of channel */
        int bytesPerSample;     /**< bytes per sample */
        int sampleRate;         /**< samples per second */
        int nbyte;              /**< byte size of the buffer */
        void *buffer;           /**< buffer data */
    } VoiceBufferData;

    /**
     * create voice data from buffer
     *
     * @param  data : voice buffer data
     * @return voice data id
     */
    VoiceId CreateVoice(VoiceBufferData &data);

    /**
     * destroy voice data
     *
     * @param voice : voice data id
     */
    mpResult DestroyVoice(VoiceId voice);

    /**
     * start lip sync
     *
     * @param voice : voice data id
     */
    mpResult Speak(VoiceId voice);

    /**
     * stop lip sync
     */
    mpResult SpeakStop();

    /**
     * resume lip sync
     */
    mpResult SpeakResume();

    /**
     * seek lip sync
     *
     * @param msec : milli second seek time
     */
    mpResult SpeakSeek(int msec);

    /**
     * set lip sync gain value
     *
     * @param gain : lip sync gain as [0.0, 1.0]
     */
    mpResult SetGain(float gain);

    /**
     * get lip sync gain value
     *
     * @return current lip sync gain value
     */
    float GetGain();

    /**
     * get voice duration
     *
     * @return voice duration in milli second
     */
    float GetDuration(VoiceId voice);

    /**
     * data structure for realtime lip sync set up
     */
    typedef struct {
        int sampleRate;         /**< samples per second */
        int bytesPerSample;     /**< bytes per sample */
        bool isSigned;          /**< true=signed false=unsigned */
    } RtSpeakInfo;

    /**
     * start realtime lip sync
     *
     * @param info : real time lip sync information
     */
    mpResult RtSpeakStart(RtSpeakInfo *info);

    /**
     * stop realtime lip sync
     */
    mpResult RtSpeakStop();

    /**
     * queue realtime lip sync buffer
     *
     * @param buffer : buffer of voice data.
                       The data must be PCM linear format with
                       sample rate specified by RtSpeakStart()
     * @param nbyte  : byte size of the buffer
     */
    mpResult RtSpeakQueue(void *buffer, int nbyte);

    /**
     * set realtime lip sync gain
     *
     * @param gain : realtime lip sync gain as [0.0, 1.0]
     */
    mpResult RtSpeakGain(float gain);

#ifdef WEBGL
    VoiceId CreateVoice(std::string pathVoice) {
        return this->CreateVoice(pathVoice.c_str());
    }
#endif

    /**
     */
    MpCtlSpeech();
    virtual ~MpCtlSpeech();

    MpCtlSpeechImpl *mp_;
};


} // namespace motionportrait

#endif /* MPCTLSPEECH_H_ */
