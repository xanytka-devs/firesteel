#ifndef AUDIO_H
#define AUDIO_H

#include <string>

namespace XEngine {
	typedef int PaError;
	class AudioManager {
	public:
		static void initialize();
		static void print_host_info();
		static bool is_active();
		static void remove();
	private:
		static bool m_is_active;
		static void report_error(PaError t_err);
	};

	struct AudioLayer {
		std::string name;
		float volume = 100.f;
		float pitch = 1.f;
	};

	static const AudioLayer master_layer{"master"};
	class Audio {
	public:
		Audio(std::string t_source, bool t_loop = false, const AudioLayer t_layer = master_layer, bool t_play_on_awake = false);
		bool is_playing();

		void play();
		void stop();
		void set_source(std::string t_source);
		void remove();
	private:
		std::string m_source;
		AudioLayer m_layer;
		bool m_loop;
		bool m_is_playing;
	};
}

#endif // AUDIO_H