#include <glad/glad.h>

#include "IndexBuffer.hpp"
#include "XEngine/Log.hpp"

namespace XEngine::OpenGL {

	/// <summary>
	/// Converts VertexBuffer::VBUsage to OpenGL's usage type.
	/// </summary>
	/// <param name="usage">Convertion value.</param>
	/// <returns>OpenGL's usage type.</returns>
	constexpr GLenum usageToGLenum(const VertexBuffer::VBUsage usage) {
		switch (usage) {
		case VertexBuffer::VBUsage::Static:
			return GL_STATIC_DRAW;
		case VertexBuffer::VBUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
		case VertexBuffer::VBUsage::Stream:
			return GL_STREAM_DRAW;
		}
		LOG_ERRR("Unknown VertexBuffer usage.");
		return GL_STREAM_DRAW;
	}

	IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::VBUsage usage)
		: curCount(count) {
		glGenBuffers(1, &curID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usageToGLenum(usage));
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &curID);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept {
		curID = indexBuffer.curID;
		indexBuffer.curID = 0;
		return *this;
	}
	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
		: curID(indexBuffer.curID) {
		indexBuffer.curID = 0;
	}
	/// <summary>
	/// Applies current index buffer to OpenGL.
	/// </summary>
	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curID);
	}
	/// <summary>
	/// Removes current index buffer from OpenGL.
	/// </summary>
	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}