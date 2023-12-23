#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		mesh.render(t_shader);
	}

	/// <summary>
	/// Deletes model (cleanup).
	/// </summary>
	void Behaviour::remove() {
		mesh.remove();
	}

}