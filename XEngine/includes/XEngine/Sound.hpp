#ifndef AUDIO_H
#define AUDIO_H

#include <string>

namespace XEngine {
	class AudioManager {
	public:
		static void initialize();
		static bool is_active();
		static void remove();
	private:
	};

	class Audio {
	public:
		Audio(std::string t_source);

		void play();
		void stop();
		void set_source(std::string t_source);
		void remove();
	private:
		std::string m_source;
	};
}

#endif // AUDIO_H