#include <iostream>
#include <vector>
#include <portaudio/include/portaudio.h>
#include <libsndfile/include/sndfile.h>
#include <AudioFile/AudioFile.h>

#include "XEngine/Log.hpp"
#include "XEngine/Audio/Sound.hpp"

namespace XEngine {

    int SAMPLE_RATE = 44100;

    typedef struct {
        SNDFILE* file;
        SF_INFO	info;
    } pa_callback_data_t;

    /* This routine will be called by the PortAudio engine when audio is needed.
    ** It may called at interrupt level on some machines so don't do anything
    ** that could mess up the system like calling malloc() or free().
    */
    static int pa_stream_callback(const void* input, void* output, unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
        pa_callback_data_t* data = (pa_callback_data_t*)userData;
        sf_count_t framesRead = 0;
        sf_count_t frames = frameCount * (sf_count_t)data->info.channels;

        memset(output, 0, sizeof(float) * frameCount * data->info.channels); 	/* clear output buffer */

        framesRead = sf_readf_float(data->file, (float*)output, frameCount);

        if (framesRead < frameCount) {
            return paComplete;
        }

        return paContinue;
    }

    static void report_error(PaError t_err) {
        Pa_Terminate();
        std::string output = "PortAudio (AudioManager::initialize()): Error #" + std::to_string(t_err) + " : " + Pa_GetErrorText(t_err);
        LOG_ERRR(output.c_str());
    }

    PaError err;
    PaStreamParameters inputParameters, outputParameters;
    const PaDeviceInfo* inputInfo;
    const PaDeviceInfo* outputInfo;
    int numChannels;
    PaStream* stream = NULL;
    static void get_device_info() {
        //Input debug info.
        inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
        printf("Input device #%d:\n", inputParameters.device);
        inputInfo = Pa_GetDeviceInfo(inputParameters.device);
        printf("    Name: %s\n", inputInfo->name);
        printf("      LL: %g s\n", inputInfo->defaultLowInputLatency);
        printf("      HL: %g s\n", inputInfo->defaultHighInputLatency);
        //Output debug info.
        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        printf("Output device #%d:\n", outputParameters.device);
        outputInfo = Pa_GetDeviceInfo(outputParameters.device);
        printf("   Name: %s\n", outputInfo->name);
        printf("     LL: %g s\n", outputInfo->defaultLowOutputLatency);
        printf("     HL: %g s\n", outputInfo->defaultHighOutputLatency);
        numChannels = inputInfo->maxInputChannels < outputInfo->maxOutputChannels
            ? inputInfo->maxInputChannels : outputInfo->maxOutputChannels;
        printf("Num channels: %d\n", numChannels);
        //Input params.
        inputParameters.channelCount = numChannels;
        inputParameters.sampleFormat = paInt32;
        inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;
        //Output params.
        outputParameters.channelCount = numChannels;
        outputParameters.sampleFormat = paInt32;
        outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;
    }

    void AudioManager::initialize() {
        //Initialize library and report version.
        err = Pa_Initialize();
        if(err != paNoError) goto report;
        LOG_INFO("PortAudio initialized.");
        LOG_INFO("PortAudio version: " + std::to_string(Pa_GetVersion()));
        //Get devices.
        int numDevices;
        const PaDeviceInfo* deviceInfo;
        numDevices = Pa_GetDeviceCount();
        if(numDevices < 0) {
            LOG_ERRR("PortAudio (AudioManager::initialize()): Pa_GetDeviceCount returned " + numDevices);
            err = numDevices;
            goto report;
        }
        LOG_INFO("Number of devices: " + std::to_string(numDevices));
        get_device_info();    
        return;
    report:
        if(stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        report_error(err);
        return;
    }


    bool AudioManager::is_active() {
        return true;
    }

    void AudioManager::remove() {
        Pa_Terminate();
        LOG_INFO("Shutting down PortAudio.");
    }

    Audio::Audio(std::string t_source, bool t_loop, bool t_play_on_awake)
        : m_source(t_source), m_loop(t_loop) {
        play();
    }

    static pa_callback_data_t audio_data;
    void Audio::play() {
        audio_data.file = sf_open(m_source.c_str(), SFM_READ, &audio_data.info);
        AudioFile<int> audioFile;
        audioFile.shouldLogErrorsToConsole(true);
        audioFile.load(m_source);
        //Start stream.
        err = Pa_OpenDefaultStream(&stream, 0,
            audioFile.getNumChannels(), paFloat32, audioFile.getSampleRate(), 
            paFramesPerBufferUnspecified, pa_stream_callback, &audio_data);
        if (err != paNoError) goto report;
        err = Pa_StartStream(stream);
        if (err != paNoError) goto report;
        // Do some other tasks or wait for user interaction
        audioFile.printSummary();
        while (Pa_IsStreamActive(stream)) {
            Pa_Sleep(100);
        }
        //Stop stream.
        err = Pa_StopStream(stream);
        if (err != paNoError) goto report;
        //Close stream.
        Pa_CloseStream(stream);
        if (err != paNoError) goto report;
        return;
    report:
        if (stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        report_error(err);
        return;
    }

    void Audio::stop() {

    }

    void Audio::remove() {
        if (sf_close(audio_data.file) != 0)
            LOG_INFO("PortAudio (Audio::remove()): Could not close Audio File!");
    }

    void Audio::set_source(std::string t_source) {
        stop();
        m_source = t_source;
    }

}