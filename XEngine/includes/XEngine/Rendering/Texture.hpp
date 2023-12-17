#ifndef TEXTURE_H
#define TEXTURE_H

namespace XEngine {
	class Texture {
	public:
		Texture();
		Texture(const char* t_path, const char* t_name, bool t_load_now = true, bool t_use_defalt_params = true);

		void generate();
		void load(bool t_flip = false);
		void enable();
		void remove();

		void set_filters(GLenum t_all);
		void set_filters(GLenum t_mag, GLenum t_min);
		void set_wrap(GLenum t_all);
		void set_wrap(GLenum t_s, GLenum t_t);

		unsigned int id;
		const char* name;
	private:
		const char* m_path;
		int m_width;
		int m_height;
		int m_channels;
	};
}

#endif // TEXTURE_H