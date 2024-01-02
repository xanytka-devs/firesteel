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
		Audio(std::string t_source, bool t_loop = false, bool t_play_on_awake = false);

		void play();
		void stop();
		void set_source(std::string t_source);
		void remove();
	private:
		std::string m_source;
		bool m_loop;
	};
}

#endif // AUDIO_H