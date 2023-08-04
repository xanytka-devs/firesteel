#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "XEngine/Log.hpp"

namespace XEngine::Rendering {

	/// <summary>
	/// Converts ShaderDataType to amount of components.
	/// </summary>
	/// <param name="type">Type of shader.</param>
	/// <returns>Amount of components.</returns>
	constexpr unsigned int shaderDataType_to_ComponentsCount(const ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;
		}
		LOG_ERRR("shaderData_to_ComponentsCount: Unknown ShaderDataType");
		return 0;
	}
	/// <summary>
	/// Converts ShaderDataType to its binary size.
	/// </summary>
	/// <param name="type">Type of shader.</param>
	/// <returns>Binary size of ShaderDataType.</returns>
	constexpr size_t shaderDataType_typeSize(const ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return sizeof(GLfloat) * shaderDataType_to_ComponentsCount(type);
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return sizeof(GLint) * shaderDataType_to_ComponentsCount(type);
		}
		LOG_ERRR("shaderData_typeSize: Unknown ShaderDataType");
		return 0;
	}
	/// <summary>
	/// Converts ShaderDataType to OpenGL's unit.
	/// </summary>
	/// <param name="type">Type of shader.</param>
	/// <returns>OpenGL's unit.</returns>
	constexpr unsigned int shaderDataType_to_componentType(const ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		}
		LOG_ERRR("shaderDataType_to_componentType: unknown ShaderDataType");
		return GL_FLOAT;
	}
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

	BufferElement::BufferElement(const ShaderDataType _type)
		: type(_type)
		, componentType(shaderDataType_to_componentType(_type))
		, componentsCount(shaderDataType_to_ComponentsCount(_type))
		, size(shaderDataType_typeSize(_type))
		, offset(0) { }
	/// <summary>
	/// Creates new vertex buffer ang applies to OpenGL.
	/// </summary>
	/// <param name="data">Data of buffer.</param>
	/// <param name="size">Size in binary.</param>
	/// <param name="usage">Usage.</param>
	VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout bufferLayout, const VBUsage usage)
		: layout(std::move(bufferLayout)) {
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
	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
		: curID(vertexBuffer.curID), layout(std::move(vertexBuffer.layout)) {
		vertexBuffer.curID = 0;
	}
	/// <summary>
	/// Applies current vertex buffer to OpenGL.
	/// </summary>
	void VertexBuffer::bind() const {
#if GL_MAJOR_VERSION < 3 && GL_MINOR_VERSION < 4
		glBindBuffer(GL_ARRAY_BUFFER, curID);
#endif
	}
	/// <summary>
	/// Removes current vertex buffer from OpenGL.
	/// </summary>
	void VertexBuffer::unbind() {
#if GL_MAJOR_VERSION < 3 && GL_MINOR_VERSION < 4
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}

}