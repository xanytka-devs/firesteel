#ifndef FS_OAL_SOURCE
#define FS_OAL_SOURCE

#include "defenitions.hpp"
#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.hpp"
#define DR_MP3_IMPLEMENTATION
#include "dr_libs/dr_mp3.hpp"
#define DR_FLAC_IMPLEMENTATION
#include "dr_libs/dr_flac.hpp"

namespace FSOAL {

	struct Source {
	public:
		Source()
			: mGain(1), mPitch(1), mLooping(false) { }
		Source(float tGain, float tPitch=1, bool tLooping=false)
			: mGain(tGain), mPitch(tPitch), mLooping(tLooping) { }
		Source(glm::vec3 tPos, glm::vec3 tVelocity=glm::vec3(0))
			: mGain(1), mPitch(1), mLooping(false), mPosition(tPos), mVelocity(tVelocity) { }
		Source(float tGain, float tPitch = 1, bool tLooping = false, glm::vec3 tPos = glm::vec3(0), glm::vec3 tVelocity = glm::vec3(0))
			: mGain(tGain), mPitch(tPitch), mLooping(tLooping), mPosition(tPos), mVelocity(tVelocity) { }
		~Source() { remove(); }

		bool initialize() {
			if(!globalInitState) return false;
			alGetError(); // clear error code 
			alGenBuffers(1, &mBuffer);
			alGenSources(1, &mSource);
			return alGetError() != AL_NO_ERROR;
		}
		bool initialize(std::string tSrc, float tGain = 1.0f, bool tLooping = false) {
			if(!globalInitState) return false;
			if(mInited) remove();
			alGetError(); // clear error code 
			alGenBuffers(1, &mBuffer);
			alGenSources(1, &mSource);
			if(alGetError() != AL_NO_ERROR) return false;
			load(tSrc);
			setGain(tGain);
			setLooping(tLooping);
			return true;
		}
		Source* init(std::string tSrc, float tGain = 1.0f, bool tLooping = false) {
			if(!globalInitState) return nullptr;
			if(mInited) remove();
			alGetError(); // clear error code 
			alGenBuffers(1, &mBuffer);
			alGenSources(1, &mSource);
			if(alGetError() != AL_NO_ERROR) return nullptr;
			load(tSrc);
			setGain(tGain);
			setLooping(tLooping);
			mInited = true;
			return this;
		}
		void remove() {
			if(!globalInitState) return;
			stop();
			alDeleteSources(1, &mSource);
			alDeleteBuffers(1, &mBuffer);
			mInited = false;
		}

		bool load(std::string tSrc) {
			if(!globalInitState) return false;
			if(!_loadWav(tSrc.c_str()))
				if(!_loadMP3(tSrc.c_str())) 
					if (!_loadFLAC(tSrc.c_str())) {
						LOG_WARN("Couldn't load unsupported audio format at " + tSrc);
						return false;
					}
			alSourcef(mSource, AL_PITCH, mPitch);
			alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
			alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
			alSourcei(mSource, AL_LOOPING, mLooping ? AL_TRUE : AL_FALSE);
			alSourcei(mSource, AL_BUFFER, mBuffer);
			alSourcef(mSource, AL_MIN_GAIN, 0);
			alSourcef(mSource, AL_GAIN, mGain);
			return true;
		}

		void play() {
			if(!globalInitState || !mInited) return;
			mPlaying = true;
			alSourcePlay(mSource);
		}
		void stop() {
			if(!globalInitState || !mInited) return;
			mPlaying = false;
			alSourceStop(mSource);
		}
		void pause() {
			if(!globalInitState || !mInited) return;
			mPlaying = !mPlaying;
			alSourcePause(mSource);
		}
		void resume() {
			if(!globalInitState || !mInited) return;
			mPlaying = true;
			play();
		}

		glm::vec3 getPostion() const { return mPosition; }
		glm::vec3 getVelocity() const { return mVelocity; }
		float getGain() const { return mGain; }
		float getPitch() const { return mPitch; }
		float getOffsetInSamples() const {
			float offset = 0;
			alGetSourcef(mSource, AL_SAMPLE_OFFSET, &offset);
			return offset;
		}
		float getOffset() const {
			return getOffsetInSamples() / mSampleRate;
		}
		float getDurationInSamples() const {
			if(!globalInitState || !mInited) return 0;
			ALint sizeInBytes;
			ALint channels;
			ALint bits;

			alGetBufferi(mBuffer, AL_SIZE, &sizeInBytes);
			alGetBufferi(mBuffer, AL_CHANNELS, &channels);
			alGetBufferi(mBuffer, AL_BITS, &bits);

			return sizeInBytes * 8.f / (channels * bits);
		}
		float getDuration() const {
			if(!globalInitState || !mInited) return 0;

			ALint frequency;
			alGetBufferi(mBuffer, AL_FREQUENCY, &frequency);

			return (float)getDurationInSamples() / (float)frequency;
		}
		bool isLooping() const { return mLooping; }
		bool isPlaying() const { return mPlaying; }
		bool isInitialized() const { return mInited; }

		Source* setPostion(glm::vec3 tPos) {
			if(!globalInitState) return this;
			mPosition = tPos;
			alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
			return this;
		}
		Source* setPostion(float tX, float tY, float tZ) {
			if(!globalInitState) return this;
			mPosition = glm::vec3(tX,tY,tZ);
			alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
			return this;
		}
		Source* setVelocity(glm::vec3 tVel) {
			if(!globalInitState) return this;
			mVelocity = tVel;
			alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
			return this;
		}
		Source* setVelocity(float tX, float tY, float tZ) {
			if(!globalInitState) return this;
			mVelocity = glm::vec3(tX, tY, tZ);
			alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
			return this;
		}
		Source* setGain(float tGain) {
			if(!globalInitState) return this;
			mGain = tGain;
			alSourcef(mSource, AL_GAIN, mGain);
			return this;
		}
		Source* setTempGain(float tGain) {
			if(!globalInitState) return this;
			alSourcef(mSource, AL_GAIN, tGain);
			return this;
		}
		Source* setPitch(float tPitch) {
			if(!globalInitState) return this;
			mPitch = tPitch;
			alSourcef(mSource, AL_PITCH, mPitch);
			return this;
		}
		Source* setTempPitch(float tPitch) {
			if(!globalInitState) return this;
			alSourcef(mSource, AL_PITCH, tPitch);
			return this;
		}
		Source* setLooping(bool tLoop) {
			if(!globalInitState) return this;
			mLooping = tLoop;
			alSourcei(mSource, AL_LOOPING, mLooping ? AL_TRUE : AL_FALSE);
			return this;
		}
		Source* setOffset(float tSec) {
			if(!globalInitState) return this;
			alSourcef(mSource, AL_SEC_OFFSET, tSec);
			return this;
		}

	private:
		/* Source info */
		glm::vec3 mPosition = glm::vec3(0);
		glm::vec3 mVelocity = glm::vec3(0);
		float mGain;
		float mPitch;
		bool mLooping, mInited = false, mPlaying = false;

		/* AL data */
		ALuint mSource = 0;
		ALuint mBuffer = 0;
		ALenum mFormat = 0;

		/* File info */
		unsigned short mChannels = 0;
		unsigned int mSampleRate = 0;
		unsigned short mBitsPerSample = 0;

	private:
		bool _loadWav(const char* tSrc) {
			drwav wav;
			if(!drwav_init_file(&wav, tSrc, NULL)) {
				return false;
			}

			mChannels = wav.channels;
			mSampleRate = wav.sampleRate;
			mBitsPerSample = wav.bitsPerSample;

			size_t totalFrameCount = wav.totalPCMFrameCount * wav.channels;
			std::vector<int16_t> soundData = std::vector<int16_t>(totalFrameCount);

			drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, soundData.data());
			drwav_uninit(&wav);

			if(mChannels == 1) {
				if(mBitsPerSample == 8) mFormat = AL_FORMAT_MONO8;
				else if(mBitsPerSample == 16) mFormat = AL_FORMAT_MONO16;
			}
			else if(mChannels == 2) {
				if(mBitsPerSample == 8) mFormat = AL_FORMAT_STEREO8;
				else if(mBitsPerSample == 16) mFormat = AL_FORMAT_STEREO16;
			}
			else {
				LOG_WARN("Unsupported audio: " + std::to_string(static_cast<int>(mChannels)) +
					" channels | " + std::to_string(static_cast<int>(mSampleRate)).c_str() + " sample rate");
				mFormat = AL_FORMAT_STEREO16;
			}

			alGetError(); // clear error code 
			alBufferData(mBuffer, mFormat, soundData.data(), static_cast<ALsizei>(soundData.size() * sizeof(int16_t)), mSampleRate);
			soundData.clear(); // erase the sound in RAM
			return alGetError() == AL_NO_ERROR;
		}
		bool _loadMP3(const char* tSrc) {
			drmp3 mp3;
			if (!drmp3_init_file(&mp3, tSrc, NULL)) {
				return false;
			}

			mChannels = mp3.channels;
			mSampleRate = mp3.sampleRate;
			mBitsPerSample = 16;

			size_t totalFrameCount = drmp3_get_pcm_frame_count(&mp3);
			std::vector<int16_t> soundData = std::vector<int16_t>(totalFrameCount * mp3.channels);

			drmp3_read_pcm_frames_s16(&mp3, totalFrameCount, soundData.data());
			drmp3_uninit(&mp3);

			if(mChannels == 1)
				mFormat = AL_FORMAT_MONO16;
			else if(mChannels == 2)
				mFormat = AL_FORMAT_STEREO16;
			else {
				LOG_WARN("Unsupported audio: " + std::to_string(static_cast<int>(mChannels)) +
					" channels | " + std::to_string(static_cast<int>(mSampleRate)) + " sample rate");
				mFormat = AL_FORMAT_STEREO16;
			}

			alGetError(); // clear error code 
			alBufferData(mBuffer, mFormat, soundData.data(), static_cast<ALsizei>(soundData.size() * sizeof(int16_t)), mSampleRate);
			soundData.clear(); // erase the sound in RAM

			return alGetError() == AL_NO_ERROR;
		}
		bool _loadFLAC(const char* tSrc) {
			drflac* flac = drflac_open_file(tSrc, NULL);
			if (!flac) {
				return false;
			}

			mChannels = flac->channels;
			mSampleRate = flac->sampleRate;
			mBitsPerSample = 16;

			size_t totalFrameCount = flac->totalPCMFrameCount * flac->channels;
			std::vector<int16_t> soundData = std::vector<int16_t>(totalFrameCount);

			drflac_read_pcm_frames_s16(flac, flac->totalPCMFrameCount, soundData.data());
			drflac_close(flac);

			if (mChannels == 1)
				mFormat = AL_FORMAT_MONO16;
			else if (mChannels == 2)
				mFormat = AL_FORMAT_STEREO16;
			else {
				LOG_WARN("Unsupported audio: " + std::to_string(static_cast<int>(mChannels)) +
					" channels | " + std::to_string(static_cast<int>(mSampleRate)) + " sample rate");
				mFormat = AL_FORMAT_STEREO16;
			}

			alGetError(); // clear error code 
			alBufferData(mBuffer, mFormat, soundData.data(), static_cast<ALsizei>(soundData.size() * sizeof(int16_t)), mSampleRate);
			soundData.clear(); // erase the sound in RAM
			return alGetError() == AL_NO_ERROR;
		}
		float samplesToSeconds(size_t samples, int sampleRate) {
			float seconds = samples / (float)sampleRate;
			return seconds;
		}
	};

}

#endif // !FS_OAL_SOURCE
