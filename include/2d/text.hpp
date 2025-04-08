#ifndef FS_TEXT
#define FS_TEXT

#include "../common.hpp"
#include <filesystem>
#include <string>
#include <map>
#include "../shader.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <../external/freetype/ft2build.h>
#include <../external/freetype/include/freetype.h>

FT_Library ft;
bool initialized;
bool done;

namespace Firesteel {
	typedef struct {
		unsigned int texture_id;	// Texture ID which is storing character.
		glm::ivec2 size;			// Size of character.
		glm::ivec2 bearing;			// Distance from origin to top left of character.
		unsigned int advance;		// Distance from origin to next origin (1/64th pixels).
	} Character;

	class TextRenderer {
	public:
		static void initialize() {
			if (FT_Init_FreeType(&ft)) {
				LOG_ERRR("Couldn't load FreeType library.");
				return;
			}
			initialized = true;
		}
	};

	class Text {
	public:
		bool loadFont(std::string path, int t_height) {
			if(!initialized) return false;
			if(!std::filesystem::exists(path)) {
				LOG_ERRR("Font file \"" + path + "\" doesn't exist.");
				return false;
			}
			//Assign variables and load font.
			m_height = t_height;
			FT_Face font;
			if (FT_New_Face(ft, path.c_str(), 0, &font)) {
				LOG_ERRR("Couldn't load font file \"" + path + "\".");
				return false;
			}
			//Set height and dynamic width.
			FT_Set_Pixel_Sizes(font, 0, m_height);
			//Load texture.
			//Disables the byte-alignment restriction so can use 1 byte for each pixel.
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			//Load first 128 characters of ASCII set.
			for (unsigned char c = 1; c < 129; c++) {
				//Load glyph.
				if (FT_Load_Char(font, c-1, FT_LOAD_RENDER)) {
					LOG_ERRR(std::string("Couldn't load glyph #") + (const char *)c + ".");
					continue;
				}
				//Generate texture.
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					font->glyph->bitmap.width,
					font->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					font->glyph->bitmap.buffer
				);
				//Set texture parameters.
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//Store character texture for use.
				Character character = {
					texture,
					glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
					glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
					static_cast<unsigned int>(font->glyph->advance.x)
				};
				m_chars.insert(std::pair<char, Character>(c-1, character));
			}
			//Signal that font was loaded.
			glBindTexture(GL_TEXTURE_2D, 0);
			FT_Done_Face(font);
			//Config VAO and VBO.
			glGenVertexArrays(1, &m_text_vao);
			glGenBuffers(1, &m_text_vbo);
			glBindVertexArray(m_text_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return true;
		}

		void draw(Shader* shader, std::string text, glm::vec2 t_proj_size, glm::vec2 t_pos, glm::vec2 scale, glm::vec3 color) {
			if (!initialized) return;
			if (!done) {
				FT_Done_FreeType(ft);
				done = true;
			}
			shader->enable();
			shader->setVec3("textColor", color);
			shader->setMat4("projection", glm::ortho(0.0f, t_proj_size.x, 0.0f, t_proj_size.y));
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(m_text_vao);
			//Go through all characters of string.
			for (size_t i = 0, len = text.size(); i < len; i++) {
				Character c = m_chars[text[i]];
				float xpos = t_pos.x + c.bearing.x * scale.x;
				float ypos = t_pos.y - (c.size.y - c.bearing.y) * scale.y; // characters might need to be shifted below baseline
				float w = c.size.x * scale.x;
				float h = c.size.y * scale.y;
				//New VBO data.
				float vertices[6][4] = {
					//			X		  Y			UV
						{ xpos,     ypos + h,   0.0f, 0.0f },
						{ xpos,     ypos,       0.0f, 1.0f },
						{ xpos + w, ypos,       1.0f, 1.0f },
			
						{ xpos,     ypos + h,   0.0f, 0.0f },
						{ xpos + w, ypos,       1.0f, 1.0f },
						{ xpos + w, ypos + h,   1.0f, 0.0f }
				};
				//Setup texture.
				glBindTexture(GL_TEXTURE_2D, c.texture_id);
				//Update content of VBO memory.
				glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//Render quad.
				glDrawArrays(GL_TRIANGLES, 0, 6);
				//Advance cursor.
				t_pos.x += (c.advance >> 6) * scale.x; // multiply by 64 
			}
			//Clear bindings.
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void remove() {
			if(!initialized) return;
			glDeleteVertexArrays(1, &m_text_vao);
			glDeleteBuffers(1, &m_text_vbo);
			m_chars.clear();
		}

		~Text() { remove(); }
	private:
		int m_height = 0;
		std::map<char, Character> m_chars;
		unsigned int m_text_vao = 0, m_text_vbo = 0;
	};

}

#endif // !FS_TEXT