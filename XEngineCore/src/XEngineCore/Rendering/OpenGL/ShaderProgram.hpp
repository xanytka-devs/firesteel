#pragma once

namespace XEngine {

	class ShaderProgram {

	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		~ShaderProgram();
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;

		void bind() const;
		static void unbind();
		bool isCompilied() const { return pIsCompiled; }

	private:
		bool pIsCompiled = false;
		unsigned int curID = 0;

	};

}