#include "XEngine/Rendering/Material.hpp"
#include "XEngine/Log.hpp"

namespace XEngine::Rendering {

	Material::Material(const char* name,
		const char* vertShader, const char* fragShader) {
		program = std::make_unique<ShaderProgram>(vertShader, fragShader);
		if (!program->isCompilied())
			LOG_CRIT("Error while compiling shader '{0}'.", name);
	}
	Material::~Material() {
		curID = 0;
		program.release();
	}

	Material& Material::operator=(Material&& mat) noexcept {
		curID = mat.curID;
		name = mat.name;
		assignedShaders = mat.assignedShaders;
		assignedTextures = mat.assignedTextures;
		mat.curID = 0;
		return *this;
	}
	Material::Material(Material&& mat) noexcept
	: curID(mat.curID), name(mat.name), assignedShaders(mat.assignedShaders),
	assignedTextures(mat.assignedTextures) {
		mat.curID = 0;
	}

	void Material::addShader(const ShaderType param, const char* value) {
		assignedShaders++;
	}
	void Material::addTexture(const Texture2D* value) {
		value->bind(static_cast<const unsigned int>(assignedTextures));
		assignedTextures++;
	}
}