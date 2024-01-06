#include <iostream>
#include <vector>
#include <thread>
#include <portaudio/include/portaudio.h>
#include <libsndfile/include/sndfile.h>

#include "XEngine/Log.hpp"
#include "XEngine/Audio.hpp"

namespace XEngine {

    typedef struct {
        SNDFILE* file;
        SF_INFO	info;
        float volume;
    } pa_callback_data_t;

    static int pa_stream_callback(const void* input, void* output, unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
        pa_callback_data_t* data = (pa_callback_data_t*)userData;
        sf_count_t framesRead = 0;
        sf_count_t frames = frameCount * (sf_count_t)data->info.channels;

        memset(output, 0, sizeof(float) * frameCount * data->info.channels); 	/* clear output buffer */

        float* out = (float*)output;
        framesRead = sf_readf_float(data->file, out, frameCount);

        for (int i = 0; i < frameCount * data->info.channels; ++i) {
            out[i] *= (data->volume/100.f);
        }

        if (framesRead < frameCount) {
            return paComplete;
        }

        return paContinue;
    }

    static void report_error(PaError t_err) {
        Pa_Terminate();
        std::string output = "PortAudio: Error #" + std::to_string(t_err) + " : " + Pa_GetErrorText(t_err);
        LOG_ERRR(output.c_str());
    }

    PaError err;
    PaStreamParameters inputParameters, outputParameters;
    const PaDeviceInfo* inputInfo;
    const PaDeviceInfo* outputInfo;
    int numChannels;
    PaStream* stream = NULL;
    void AudioManager::print_host_info() {
        //Input debug info.
        printf("Audio | PortAudio (GLFW & GLAD)\n");
        printf("    Input device #%d:\n", inputParameters.device);
        printf("        Name: %s\n", inputInfo->name);
        printf("        LL: %g s\n", inputInfo->defaultLowInputLatency);
        printf("        HL: %g s\n", inputInfo->defaultHighInputLatency);
        //Output debug info.
        printf("    Output device #%d:\n", outputParameters.device);
        printf("        Name: %s\n", outputInfo->name);
        printf("        LL: %g s\n", outputInfo->defaultLowOutputLatency);
        printf("        HL: %g s\n", outputInfo->defaultHighOutputLatency);
        printf("    Num channels: %d\n", numChannels);
    }

    void AudioManager::initialize() {
        //Initialize library and report version.
        err = Pa_Initialize();
        if(err != paNoError) goto report;
        LOG_INFO("PortAudio initialized.");
        LOG_INFO("PortAudio version: " + std::to_string(Pa_GetVersion()));
        //Get devices.
        int numDevices = Pa_GetDeviceCount();
        if(numDevices < 0) {
            LOG_ERRR("PortAudio (AudioManager::initialize()): Pa_GetDeviceCount returned " + numDevices);
            err = numDevices;
            goto report;
        }
        LOG_INFO("Number of devices: " + std::to_string(numDevices));
        //Input info.
        inputParameters.device = Pa_GetDefaultInputDevice();
        inputInfo = Pa_GetDeviceInfo(inputParameters.device);
        //Output info.
        outputParameters.device = Pa_GetDefaultOutputDevice();
        outputInfo = Pa_GetDeviceInfo(outputParameters.device);
        numChannels = inputInfo->maxInputChannels < outputInfo->maxOutputChannels
            ? inputInfo->maxInputChannels : outputInfo->maxOutputChannels;
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

    Audio::Audio(std::string t_source, bool t_loop, AudioLayer t_layer, bool t_play_on_awake)
        : m_source(t_source), m_loop(t_loop), m_layer(t_layer) {
        if(t_play_on_awake) play();
    }

    static pa_callback_data_t audio_data;
    void play_stream(pa_callback_data_t* data, AudioLayer m_layer) {
        // Start stream.
        err = Pa_OpenDefaultStream(&stream, 0,
            data->info.channels, paFloat32, data->info.samplerate * m_layer.pitch,
            paFramesPerBufferUnspecified, pa_stream_callback, data);
        if (err != paNoError) {
            report_error(err);
            return;
        }
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            Pa_CloseStream(stream);
            report_error(err);
            return;
        }
        // Sleep for a while or perform other tasks while audio is playing asynchronously.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // Stop stream.
        err = Pa_StopStream(stream);
        if (err != paNoError) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
            report_error(err);
            return;
        }
        // Close stream.
        Pa_CloseStream(stream);
        if (err != paNoError) {
            report_error(err);
            return;
        }
    }

    void Audio::play() {
        audio_data.file = sf_open(m_source.c_str(), SFM_READ, &audio_data.info);
        if (!audio_data.file) {
            LOG_INFO("PortAudio (Audio::playAsync()): Couldn't open audio file.");
            return;
        }
        //Set volume.
        audio_data.volume = m_layer.volume;
        while (m_loop) {
            //Detach the thread, allowing it to run independently.
            std::thread async_thread(play_stream, &audio_data, m_layer);
            async_thread.detach();
            //Reset position in file to play once again.
            sf_seek(audio_data.file, 0, SEEK_SET);
            //Sleep for a short time before starting a new thread.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        //If not in loop - just play audio once.
        play_stream(&audio_data, m_layer);
        sf_close(audio_data.file);
    }

    void Audio::stop() {

    }

    void Audio::remove() {
        if (sf_close(audio_data.file) != 0)
            LOG_INFO("PortAudio (Audio::remove()): Couldn't close audio file.");
    }

    void Audio::set_source(std::string t_source) {
        stop();
        m_source = t_source;
    }

}