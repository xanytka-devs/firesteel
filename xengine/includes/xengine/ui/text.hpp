#include "xengine/common.hpp"
#ifdef XENGINE_UI

#ifndef UI_TEXT_H
#define UI_TEXT_H

#include <string>
#include <map>
#include "xengine/math.hpp"
#include "xengine/rendering/shader.hpp"

namespace XEngine {
	typedef struct {
		unsigned int texture_id;	// Texture ID which is storing character.
		glm::ivec2 size;			// Size of character.
		glm::ivec2 bearing;			// Distance from origin to top left of character.
		unsigned int advance;		// Distance from origin to next origin (1/64th pixels).
	} Character;

	class TextRenderer {
	public:
		/// <summary>
		/// Initialize module.
		/// </summary>
		void initialize();
	};

	class Text {
	public:
		Text();
		/// <summary>
		/// Load font from file.
		/// </summary>
		/// <param name="t_path">Path to font file.</param>
		/// <param name="t_height">Font size.</param>
		/// <returns>Sucess.</returns>
		bool load_font(std::string t_path, int t_height = 32);
		/// <summary>
		/// Render font by shader.
		/// </summary>
		/// <param name="t_shader">Font shader.</param>
		/// <param name="t_text">String of text.</param>
		/// <param name="t_proj_size">Size of projection (window size).</param>
		/// <param name="t_pos">Position of text.</param>
		/// <param name="t_scale">Scale of text.</param>
		/// <param name="t_color">Color of text.</param>
		void render(Shader t_shader, std::string t_text, glm::vec2 t_proj_size,
			glm::vec2 t_pos = glm::vec3(0), glm::vec2 t_scale = glm::vec3(1), glm::vec3 t_color = glm::vec3(1));
		/// <summary>
		/// Delete font renderer (cleanup).
		/// </summary>
		void remove();
	private:
		int m_height;
		std::map<char, Character> m_chars;
		unsigned int m_text_vao, m_text_vbo;
	};

}

#endif // UI_TEXT_H

#endif // XENGINE_UI