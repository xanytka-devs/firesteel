#include <iostream>
#include <vector>
#include <thread>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "xengine/log.hpp"
#include "xengine/audio.hpp"

namespace XEngine {

    constexpr auto MAX_DEVICES = 2;
    constexpr auto MAX_SOUNDS = 32;

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;
    bool AudioManager::m_is_active = false;

    /// <summary>
    /// Logs host device information.
    /// </summary>
    void AudioManager::print_host_info() {
        //deviceConfig = ma_device_config_init(ma_device_type_playback);
        ////Input debug info.
        //printf("Audio | MiniAudio\n");
        //printf("    Input device #%d:\n", *deviceConfig.capture.pDeviceID);
        //printf("        Channels: %g s\n", deviceConfig.capture.channels);
        //printf("        Format: %g s\n", deviceConfig.capture.format);
        ////Output debug info.
        //printf("    Output device #%d:\n", *deviceConfig.playback.pDeviceID);
        //printf("        Channels: %g s\n", deviceConfig.playback.channels);
        //printf("        Format: %g s\n", deviceConfig.playback.format);
    }

    /// <summary>
    /// Initializes MiniAudio.
    /// </summary>
    void AudioManager::initialize() {
        //Initialize library and report version.
        LOG_INFO("MiniAudio initialized.");
    }

    /// <summary>
    /// Is audio manager initialized.
    /// </summary>
    /// <returns>State.</returns>
    bool AudioManager::is_active() {
        return m_is_active;
    }

    /// <summary>
    /// Shutdown AudioManager (cleanup).
    /// </summary>
    void AudioManager::remove() {
        LOG_INFO("Shutting down MiniAudio.");
    }

    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if(pDecoder == NULL) return;
        ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
        (void)pInput;
    }


    Audio::Audio(std::string t_source, bool t_loop, const AudioLayer t_layer, bool t_play_on_awake)
        : m_source(t_source), m_loop(t_loop), m_layer(t_layer) {
        if(t_play_on_awake) play();
    }

    /// <summary>
    /// Is audio source currently playing.
    /// </summary>
    /// <returns>State.</returns>
    bool Audio::is_playing() const {
        return ma_device_get_state(&device) != ma_device_state_stopped 
           && ma_device_get_state(&device) != ma_device_state_uninitialized;
    }

    int Audio::get_length() {
        ma_uint64 length;
        ma_data_source_get_length_in_pcm_frames(decoder.pBackend, &length);
        return length;
    }

    /// <summary>
    /// Play audio source.
    /// </summary>
    void Audio::play() {
        if(is_playing()) stop();
        result = ma_decoder_init_file(m_source.c_str(), NULL, &decoder);
        if (result != MA_SUCCESS) {
            LOG_ERRR("Couldn't initialize decoder.");
            return;
        }
        ma_data_source_set_looping(&decoder, m_loop ? MA_TRUE : MA_FALSE);
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format = decoder.outputFormat;
        deviceConfig.playback.channels = decoder.outputChannels;
        deviceConfig.sampleRate = decoder.outputSampleRate;
        deviceConfig.dataCallback = data_callback;
        deviceConfig.pUserData = &decoder;
        if(ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
            LOG_ERRR("Failed to open playback device.");
            ma_decoder_uninit(&decoder);
            return;
        }
        ma_device_set_master_volume(&device, m_layer.volume / 100.f);
        if(ma_device_start(&device) != MA_SUCCESS) {
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
            LOG_ERRR("Failed to start playback device.");
            return;
        }
    }

    /// <summary>
    /// Stops audio source.
    /// </summary>
    void Audio::stop() {
        if(ma_device_get_state(&device) != ma_device_state_started) return;
        if(ma_device_stop(&device) != MA_SUCCESS) {
            LOG_ERRR("Failed to stop playback device.");
            return;
        }
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
    }

    /// <summary>
    /// Remove audio (cleanup).
    /// </summary>
    void Audio::remove() {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
    }

    void Audio::set_source(std::string t_source) {
        stop();
        m_source = t_source;
    }

}