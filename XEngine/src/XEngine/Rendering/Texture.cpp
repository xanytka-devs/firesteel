#include <iostream>

#include "XEngine/Rendering/Texture.hpp"
#include "XEngine/ResManager.hpp"

namespace XEngine {

	Texture::Texture() { }

	/// <summary>
	/// Creates new texture instance.
	/// </summary>
	/// <param name="t_path">File path.</param>
	/// <param name="t_name">Name for texture.</param>
	/// <param name="t_load_now">Load texture autmatically?</param>
	/// <param name="t_use_defalt_params">Set parameters to default.</param>
	Texture::Texture(const char* t_path, const char* t_name, bool t_load_now, bool t_use_defalt_params)
	: m_path(t_path), name(t_name) {
		generate();
		//Set parameters.
		if(t_use_defalt_params) { 
			set_filters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
			set_wrap(GL_REPEAT);
		}
		//Load texture.
		if(t_load_now) load();
	}

	/// <summary>
	/// Generate texture.
	/// </summary>
	void Texture::generate() {
		glGenTextures(1, &id);
	}

	/// <summary>
	/// Load texture file.
	/// </summary>
	/// <param name="t_flip">Flip texture by Y?</param>
	void Texture::load(bool t_flip) {
		//Load texture
		unsigned char* data = ResManager::load_image(m_path, &m_width, &m_height, &m_channels, t_flip);
		//Set texture mode.
		GLenum color_m = GL_RGB;
		GLenum color_m_sup = GL_RGB;
		switch (m_channels) {
		case 1:
			color_m = GL_RED;
			color_m_sup = GL_RED;
			break;
		case 4:
			color_m = GL_RGBA;
			color_m_sup = GL_RGB;
			break;
		}
		//Generate mipmaps.
		if(data) {
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, color_m, m_width, m_height, 0, color_m_sup, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		//Free data.
		ResManager::free_image(data);
	}

	/// <summary>
	/// Set texture to current.
	/// </summary>
	void Texture::enable() {
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	/// <summary>
	/// Delete texture (cleanup).
	/// </summary>
	void Texture::remove() {
		glDeleteTextures(1, &id);
	}

	/// <summary>
	/// Set mipmap filters.
	/// </summary>
	/// <param name="t_all">Filter.</param>
	void Texture::set_filters(GLenum t_all) {
		set_filters(t_all, t_all);
	}

	/// <summary>
	/// Set mipmap filters.
	/// </summary>
	/// <param name="t_mag">Filter for far.</param>
	/// <param name="t_min">Filter for near.</param>
	void Texture::set_filters(GLenum t_mag, GLenum t_min) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, t_mag);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, t_min);
	}

	/// <summary>
	/// Set texture wrapping.
	/// </summary>
	/// <param name="t_all">Wrap state.</param>
	void Texture::set_wrap(GLenum t_all) {
		set_wrap(t_all, t_all);
	}

	/// <summary>
	/// Set texture wrapping.
	/// </summary>
	/// <param name="t_s">Horizontal wrap.</param>
	/// <param name="t_t">Vertical wrap.</param>
	void Texture::set_wrap(GLenum t_s, GLenum t_t) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, t_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_t);
	}
}