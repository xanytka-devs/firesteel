#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

#include "xengine/rendering/shader.hpp"
#include "xengine/enviroment.hpp"

namespace XEngine {
	class Cubemap {
	public:
		Cubemap();

		/// <summary>
		/// Load cubemap manualy (from individual files).
		/// </summary>
		/// <param name="t_dir">Path to directory, where all cubemap images are.</param>
		/// <param name="t_right">Right side of cubemap.</param>
		/// <param name="t_left">Left side of cubemap.</param>
		/// <param name="t_top">Top side of cubemap.</param>
		/// <param name="t_bottom">Bottom side of cubemap.</param>
		/// <param name="t_front">Front side of cubemap.</param>
		/// <param name="t_back">Back side of cubemap.</param>
		void load_m(const char* t_dir,
			const char* t_right = "right.png",
			const char* t_left = "left.png",
			const char* t_top = "top.png",
			const char* t_bottom = "bottom.png",
			const char* t_front = "front.png",
			const char* t_back = "back.png");
		/// <summary>
		/// Load cubemap from special defenitions file (.cubemap).
		/// </summary>
		/// <param name="t_cb_file_path">Path to cubemap file.</param>
		void load(const char* t_cb_file_path);

		/// <summary>
		/// Initialize cubemap variables.
		/// </summary>
		/// <param name="t_size">Cubemap size.</param>
		void initialize(float t_size = 1.0f);
		/// <summary>
		/// Get texture ID of cubemap.
		/// </summary>
		/// <returns>Texture ID of cubemap.</returns>
		unsigned int get_id() const { return m_id; }
		/// <summary>
		/// Render cubemap.
		/// </summary>
		/// <param name="t_shader">Cubemap shader.</param>
		void render(Shader t_shader);
		/// <summary>
		/// Clear cubemap's textures.
		/// </summary>
		void clear();
		/// <summary>
		/// Delete cubemap (cleanup).
		/// </summary>
		void remove();
	private:
		unsigned int m_skybox_vao, m_skybox_vbo;
		unsigned int m_id;
		std::string m_dir;
		std::vector<const char*> m_faces;
		bool m_has_textures;
	};
}

#endif