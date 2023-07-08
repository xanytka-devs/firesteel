#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "XEngineCore/Log.hpp"

namespace XEngine {

	/// <summary>
	/// Converts VertexBuffer::VBUsage to OpenGL's usage type.
	/// </summary>
	/// <param name="usage">Convertion value.</param>
	/// <returns>OpenGL's usage type.</returns>
	constexpr GLenum usageToGLenum(const VertexBuffer::VBUsage usage) {
		//Select from known usage types.
		switch (usage) {
			case VertexBuffer::VBUsage::Static:
				return GL_STATIC_DRAW;
			case VertexBuffer::VBUsage::Dynamic:
				return GL_DYNAMIC_DRAW;
			case VertexBuffer::VBUsage::Stream:
				return GL_STREAM_DRAW;
		}
		LOG_ERRR("Unknown VertexBuffer usage.");
		return GL_STATIC_DRAW;
	}

	VertexBuffer::VertexBuffer(const void* data, const size_t size, const VBUsage usage) {
		glGenBuffers(1, &curID);
		glBindBuffer(GL_ARRAY_BUFFER, curID);
		glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
	}
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &curID);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept {
		curID = vertexBuffer.curID;
		vertexBuffer.curID = 0;
		return *this;
	}
	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept {
		curID = vertexBuffer.curID;
		vertexBuffer.curID = 0;
	}
	/// <summary>
	/// Applies current vertex buffer to OpenGL.
	/// </summary>
	void VertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, curID);
	}
	/// <summary>
	/// Removes current vertex buffer from OpenGL.
	/// </summary>
	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}