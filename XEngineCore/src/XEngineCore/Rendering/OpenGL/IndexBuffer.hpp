#pragma once
#include "VertexBuffer.hpp"

namespace XEngine {

	class IndexBuffer {
	public:
		IndexBuffer(const void* data, const size_t count, const VertexBuffer::VBUsage usage = VertexBuffer::VBUsage::Static);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& indexArray) noexcept;
		IndexBuffer(IndexBuffer&& indexArray) noexcept;

		void bind() const;
		static void unbind();
		size_t getCount() const { return curCount; }
	private:
		unsigned int curID = 0;
		size_t curCount = 0;
	};

}