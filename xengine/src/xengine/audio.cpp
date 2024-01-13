#include <iostream>
#include <vector>
#include <thread>
#include <portaudio/include/portaudio.h>
#include <libsndfile/include/sndfile.h>

#include "xengine/log.hpp"
#include "xengine/audio.hpp"

namespace XEngine {

    bool AudioManager::m_is_active = false;

    /// <summary>
    /// PortAudio callback data.
    /// </summary>
    typedef struct {
        SNDFILE* file;
        SF_INFO	info;
        float volume;
    } pa_callback_data_t;

    /// <summary>
    /// PortAudio callback. Used to play audio from file.
    /// </summary>
    /// <param name="input">Input buffer.</param>
    /// <param name="output">Output buffer.</param>
    /// <param name="frame_count">Amount of frames.</param>
    /// <param name="time_info">Time.</param>
    /// <param name="status_flags">Flags.</param>
    /// <param name="user_data">Callback data.</param>
    /// <returns>Callback status.</returns>
    static int pa_stream_callback(const void* input, void* output, unsigned long frame_count,
        const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* user_data) {
        pa_callback_data_t* data = (pa_callback_data_t*)user_data;
        sf_count_t framesRead = 0;
        sf_count_t frames = frame_count * (sf_count_t)data->info.channels;

        memset(output, 0, sizeof(float) * frame_count * data->info.channels); 	/* clear output buffer */

        float* out = (float*)output;
        framesRead = sf_readf_float(data->file, out, frame_count);

        for (int i = 0; i < frame_count * data->info.channels; ++i) {
            out[i] *= (data->volume/100.f);
        }

        if (framesRead < frame_count) {
            return paComplete;
        }

        return paContinue;
    }

    /// <summary>
    /// Logs given error.
    /// </summary>
    /// <param name="t_err">Error.</param>
    void AudioManager::report_error(PaError t_err) {
        //Terminate library.
        Pa_Terminate();
        m_is_active = false;
        //Log error.
        LOG_ERRR("PortAudio: Error #" + std::to_string(t_err) + " : " + Pa_GetErrorText(t_err));
    }

    PaError err;
    PaStreamParameters inputParameters, outputParameters;
    const PaDeviceInfo* inputInfo;
    const PaDeviceInfo* outputInfo;
    int numChannels;
    PaStream* stream = NULL;

    /// <summary>
    /// Logs host device information.
    /// </summary>
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

    /// <summary>
    /// Initializes PortAudio.
    /// </summary>
    void AudioManager::initialize() {
        //Initialize library and report version.
        err = Pa_Initialize();
        if(err != paNoError) goto error;
        LOG_INFO("PortAudio initialized.");
        LOG_INFO("PortAudio version: " + std::to_string(Pa_GetVersion()));
        //Get devices.
        int numDevices = Pa_GetDeviceCount();
        if(numDevices < 0) {
            LOG_ERRR("PortAudio (AudioManager::initialize()): Pa_GetDeviceCount returned " + numDevices);
            err = numDevices;
            goto error;
        }
        LOG_INFO("Number of devices: " + std::to_string(numDevices));
        m_is_active = true;
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
    error:
        //Report error.
        report_error(err);
    }


    bool AudioManager::is_active() {
        return m_is_active;
    }

    void AudioManager::remove() {
        Pa_Terminate();
        LOG_INFO("Shutting down PortAudio.");
    }

    Audio::Audio(std::string t_source, bool t_loop, const AudioLayer t_layer, bool t_play_on_awake)
        : m_source(t_source), m_loop(t_loop), m_layer(t_layer) {
        if(t_play_on_awake) play();
    }

    bool Audio::is_playing() {
        return m_is_playing;
    }

    /// <summary>
    /// Logs given error.
    /// </summary>
    /// <param name="t_err">Error.</param>
    void audio_report_error(PaError t_err) {
        //Log error.
        LOG_ERRR("PortAudio: Error #" + std::to_string(t_err) + " : " + Pa_GetErrorText(t_err));
    }

    static pa_callback_data_t audio_data;
    void play_stream(pa_callback_data_t* data, AudioLayer m_layer) {
        // Start stream.
        err = Pa_OpenDefaultStream(&stream, 0,
            data->info.channels, paFloat32, data->info.samplerate * m_layer.pitch,
            paFramesPerBufferUnspecified, pa_stream_callback, data);
        if(err != paNoError) {
            goto error;
            return;
        }
        err = Pa_StartStream(stream);
        if(err != paNoError) {
            goto error;
            return;
        }
        // Sleep for a while or perform other tasks while audio is playing asynchronously.
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // Stop stream.
        err = Pa_StopStream(stream);
        if(err != paNoError) {
            goto error;
            return;
        }
        // Close stream.
        Pa_CloseStream(stream);
        if(err != paNoError) {
            goto error;
            return;
        }
        return;
    error:
        //Close stream and report error.
        if (stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        audio_report_error(err);
    }

    void Audio::play() {
        if(!AudioManager::is_active()) {
            LOG_WARN("PortAudio (Audio::play()): Couldn't play sound because AudioManager isn't active.")
            return;
        }
        audio_data.file = sf_open(m_source.c_str(), SFM_READ, &audio_data.info);
        if (!audio_data.file) {
            LOG_WARN("PortAudio (Audio::play()): Couldn't open audio file.");
            return;
        }
        //Set volume.
        audio_data.volume = m_layer.volume;
        m_is_playing = true;
        //Detach the thread, allowing it to run independently.
        std::thread async_thread(play_stream, &audio_data, m_layer);
        while (m_loop) {
            async_thread.detach();
            //Reset position in file to play once again.
            sf_seek(audio_data.file, 0, SEEK_SET);
            //Sleep for a short time before starting a new thread.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        //If not in loop - just play audio once.
        async_thread.detach();
        sf_close(audio_data.file);
        m_is_playing = false;
    }

    void Audio::stop() {
        sf_close(audio_data.file);
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