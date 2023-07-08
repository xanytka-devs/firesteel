#pragma once

#include "VertexBuffer.hpp"

namespace XEngine {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& vertexArray) noexcept;

		void addBuffer(const VertexBuffer& vertexBuffer);
		void bind() const;
		static void unbind();
	private:
		unsigned int curID = 0;
		unsigned int elementsCount = 0;
	};

}