#include <iostream>
#include <vector>
#include <portaudio/include/portaudio.h>

#include "XEngine/Log.hpp"
#include "XEngine/Sound.hpp"

namespace XEngine {

    typedef struct
    {
        float left_phase;
        float right_phase;
    }
    paTestData;

    /* This routine will be called by the PortAudio engine when audio is needed.
    ** It may called at interrupt level on some machines so don't do anything
    ** that could mess up the system like calling malloc() or free().
    */
    static int patestCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData)
    {
        /* Cast data passed through stream to our structure. */
        paTestData* data = (paTestData*)userData;
        float* out = (float*)outputBuffer;
        unsigned int i;
        (void)inputBuffer; /* Prevent unused variable warning. */

        for (i = 0; i < framesPerBuffer; i++)
        {
            *out++ = data->left_phase;  /* left */
            *out++ = data->right_phase;  /* right */
            /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
            data->left_phase += 0.01f;
            /* When signal reaches top, drop back down. */
            if (data->left_phase >= 1.0f) data->left_phase -= 2.0f;
            /* higher pitch so we can distinguish left and right. */
            data->right_phase += 0.03f;
            if (data->right_phase >= 1.0f) data->right_phase -= 2.0f;
        }
        return 0;
    }

    static paTestData data;
    void AudioManager::initialize() {
        PaStream* stream;
        PaError err;

        LOG_INFO("PortAudio (AudioManager::initialize()): Output sawtooth wave.");
        /* Initialize our data for use by callback. */
        data.left_phase = data.right_phase = 0.0;
        /* Initialize library before making any other calls. */
        err = Pa_Initialize();
        if (err != paNoError) goto error;

        /* Open an audio I/O stream. */
        err = Pa_OpenDefaultStream(&stream,
            0,          /* no input channels */
            2,          /* stereo output */
            paFloat32,  /* 32 bit floating point output */
            44100,
            256,        /* frames per buffer */
            patestCallback,
            &data);
        if (err != paNoError) goto error;

        err = Pa_StartStream(stream);
        if (err != paNoError) goto error;

        /* Sleep for several seconds. */
        Pa_Sleep(4 * 1000);

        err = Pa_StopStream(stream);
        if (err != paNoError) goto error;
        err = Pa_CloseStream(stream);
        if (err != paNoError) goto error;
        Pa_Terminate();
        LOG_INFO("PortAudio (AudioManager::initialize()): Test finished.");
    error:
        Pa_Terminate();
        std::string output = "PortAudio (AudioManager::initialize()): Error #" + std::to_string(err) + " : " + Pa_GetErrorText(err);
        LOG_ERRR(output.c_str());
    }

    bool AudioManager::is_active() {
        return true;
    }

    void AudioManager::remove() {
        
    }

    Audio::Audio(std::string t_source)
        : m_source(t_source) {

    }

    void Audio::play() {
        
    }

    void Audio::stop() {

    }

    void Audio::remove() {

    }

    void Audio::set_source(std::string t_source) {
        stop();
        m_source = t_source;
    }

}