#include "XEngine/Rendering/Behaviour.hpp"

namespace XEngine {

	Behaviour::Behaviour() { }

	/// <summary>
	/// Initializes model.
	/// </summary>
	void Behaviour::initialize() { }

	/// <summary>
	/// Render model.
	/// </summary>
	/// <param name="t_shader">Shader for meshes.</param>
	void Behaviour::render(Shader t_shader) {
		for (Mesh mesh : meshes)
			mesh.render(t_shader);
	}

	/// <summary>
	/// Deletes model (cleanup).
	/// </summary>
	void Behaviour::remove() {
		for (Mesh mesh : meshes)
			mesh.remove();
	}

}