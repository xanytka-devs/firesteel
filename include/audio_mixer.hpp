#ifndef FSE_AUDIO_MIXER
#define FSE_AUDIO_MIXER

#include "../engine/include/audio/source.hpp"

namespace FSOAL {
	struct AudioLayer {
	public:
		float gain = 1;
		float pitch = 1;
	};

	Source* setGain(Source* src, AudioLayer tAl) {
		src->setTempGain(src->getGain() * tAl.gain);
		return src;
	}
	Source* setPitch(Source* src, AudioLayer tAl) {
		src->setTempPitch(src->getPitch() * tAl.pitch);
		return src;
	}
}

#endif // !FSE_AUDIO_MIXER
