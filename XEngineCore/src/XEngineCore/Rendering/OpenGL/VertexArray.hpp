#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace XEngine {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& vertexArray) noexcept;

		void addVertexBuffer(const VertexBuffer& vertexBuffer);
		void setIndexBuffer(const IndexBuffer& indexBuffer);
		void bind() const;
		static void unbind();
		size_t getIndicesCount() const { return indicesCount; }
	private:
		unsigned int curID = 0;
		unsigned int elementsCount = 0;
		size_t indicesCount = 0;
	};

}