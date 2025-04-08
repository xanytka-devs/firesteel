#ifndef FS_OAL_DEF
#define FS_OAL_DEF

#include "../common.hpp"
#include <utility>
#include <vector>
#include <array>
#include <al.h>
#include <alc.h>
#include <alext.h>

namespace FSOAL {

    static ALCdevice* ALCDEVICE;
    static ALCcontext* ALCONTEXT;
    static bool globalInitState;

	static bool initialize() {
        char const* device_name = nullptr;
        device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
        ALCDEVICE = alcOpenDevice(device_name);
        if (!ALCDEVICE) {
            LOG_CRIT("OpenAL default device couldn't be open (How?).");
            return false;
        }

        ALCONTEXT = alcCreateContext(ALCDEVICE, (ALCint*)nullptr);
        ALCboolean contextMadeCurrent = false;
        alcMakeContextCurrent(ALCONTEXT);
        globalInitState = true;
        return true;
	}

    void deinitialize() {
        if(!globalInitState) return;
        alcMakeContextCurrent(ALCONTEXT);
        alcDestroyContext(ALCONTEXT);
        alcCloseDevice(ALCDEVICE);
        globalInitState = false;
    }

}

#endif // !FS_OAL_DEF