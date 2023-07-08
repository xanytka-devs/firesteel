#pragma once

namespace XEngine {

	class VertexBuffer {
	public:
		enum class VBUsage {
			Static,
			Dynamic,
			Stream
		};
		VertexBuffer(const void* data, const size_t, const VBUsage usage = VertexBuffer::VBUsage::Static);
		~VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&&) noexcept;

		void bind() const;
		static void unbind();
	private:
		unsigned int curID = 0;
	};

}