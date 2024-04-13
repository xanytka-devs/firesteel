#include "shader.hpp"
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

namespace firesteel {
	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(glm::vec2 t_scale, bool t_drawer);

		/// <summary>
		/// Resize frame buffer.
		/// </summary>
		/// <param name="t_scale">New size.</param>
		void scale(glm::vec2 t_scale);
		/// <summary>
		/// Enable frame buffer.
		/// </summary>
		void begin();
		/// <summary>
		/// Disable frame buffer.
		/// </summary>
		void end();
		/// <summary>
		/// Delete frame buffer (cleanup).
		/// </summary>
		void remove() const;

		/// <summary>
		/// Get ID of buffer's texture.
		/// </summary>
		/// <returns>ID of buffer's texture.</returns>
		unsigned int get_id() const { return m_texture; }
		/// <summary>
		/// Draw texture to object.
		/// </summary>
		/// <param name="t_object">Texture reciever.</param>
		void draw_to(unsigned int t_object);
		/// <summary>
		/// Display frame buffer to screen.
		/// </summary>
		/// <param name="t_screen_shader">Draw shader.</param>
		void draw(Shader t_screen_shader);
	private:
		unsigned int m_fbo = 0, m_texture = 0, m_rbo = 0, m_vao = 0, m_vbo = 0;
	};
}

#endif // FRAME_BUFFER_H