#include "xengine/rendering/light.hpp"

namespace XEngine {

	void DirectionalLight::render(Shader t_shader) const {
		std::string name = "dir_light";
		t_shader.enable();
		//Set variables.
		t_shader.set_3_floats(name + ".direction", direction);
		t_shader.set_4_floats(name + ".ambient", ambient);
		t_shader.set_4_floats(name + ".diffuse", diffuse);
		t_shader.set_4_floats(name + ".specular", specular);
		t_shader.set_4_floats(name + ".color", color);
	}

	void PointLight::render(Shader t_shader, int idx) const {
		std::string name = "point_lights[" + std::to_string(idx) + "]";
		t_shader.enable();
		//Set variables.
		t_shader.set_3_floats(name + ".position", position);
		t_shader.set_4_floats(name + ".ambient", ambient);
		t_shader.set_4_floats(name + ".diffuse", diffuse);
		t_shader.set_4_floats(name + ".specular", specular);
		t_shader.set_4_floats(name + ".color", color);
		//Attenuation.
		t_shader.set_float(name + ".k0", k0);
		t_shader.set_float(name + ".k1", k1);
		t_shader.set_float(name + ".k2", k2);
	}

	void SpotLight::render(Shader t_shader, int idx) const {
		std::string name = "spot_lights[" + std::to_string(idx) + "]";
		t_shader.enable();
		//Set variables.
		t_shader.set_3_floats(name + ".position", position);
		t_shader.set_3_floats(name + ".direction", direction);
		t_shader.set_float(name + ".inner_cutoff", inner_cutoff);
		t_shader.set_float(name + ".outer_cutoff", outer_cutoff);
		t_shader.set_4_floats(name + ".ambient", ambient);
		t_shader.set_4_floats(name + ".diffuse", diffuse);
		t_shader.set_4_floats(name + ".specular", specular);
		t_shader.set_4_floats(name + ".color", color);
		//Attenuation.
		t_shader.set_float(name + ".k0", k0);
		t_shader.set_float(name + ".k1", k1);
		t_shader.set_float(name + ".k2", k2);
	}

}