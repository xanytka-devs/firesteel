#include <glad/glad.h>

#include "XEngine/Rendering/VertexArray.hpp"

namespace XEngine::Rendering {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &curID);
	}
	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &curID);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		curID = vertexArray.curID;
		elementsCount = vertexArray.elementsCount;
		vertexArray.curID = 0;
		vertexArray.elementsCount = 0;
		return *this;
	}
	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
		:curID(vertexArray.curID), elementsCount(vertexArray.elementsCount) {
		vertexArray.curID = 0;
		vertexArray.elementsCount = 0;
	}

	/// <summary>
	/// Applies vertex array to OpenGL.
	/// </summary>
	void VertexArray::bind() const {
		glBindVertexArray(curID);
	}
	/// <summary>
	/// Removes vertex array from OpenGL.
	/// </summary>
	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

	/// <summary>
	/// Adds vertex buffer to array.
	/// </summary>
	/// <param name="vertexBuffer">New element.</param>
	void VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer) {
		//Apply vertex array and buffer to OpenGL.
		bind();
#if GL_MAJOR_VERSION >= 3 && GL_MINOR_VERSION >= 4
		//Add buffer to array and count.
		for (const BufferElement& curElement : vertexBuffer.getLayout().getElements()) {
			//Do some wizardry!
			glEnableVertexAttribArray(elementsCount);
			glBindVertexBuffer(elementsCount, vertexBuffer.getHandle(),
				curElement.offset, static_cast<GLsizei>(vertexBuffer.getLayout().getStride()));
			glVertexAttribFormat(elementsCount, static_cast<GLuint>(curElement.componentsCount),
				curElement.componentType, GL_FALSE, 0);
			glVertexAttribBinding(elementsCount, elementsCount);
			//Add 1 to elements count.
			elementsCount++;
		}
#else
		vertexBuffer.bind();
		//Add buffer to array and count.
		for (const BufferElement& curElement : vertexBuffer.getLayout().getElements()) {
			//Do some wizardry!
			glEnableVertexAttribArray(elementsCount);
			glVertexAttribPointer(elementsCount,
				static_cast<GLuint>(curElement.componentsCount),
				curElement.componentType,
				GL_FALSE,
				static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
				reinterpret_cast<const void*>(curElement.offset));
			//Add 1 to elements count.
			elementsCount++;
		}
#endif
	}

	/// <summary>
	/// Sets index buffer to array.
	/// </summary>
	/// <param name="indexBuffer">New element.</param>
	void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer) {
		//Apply index array and buffer to OpenGL.
		bind();
		indexBuffer.bind();
		indicesCount = indexBuffer.getCount();
	}

}