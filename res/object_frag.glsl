#version 330 core
out vec4 fragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 frag_POS;
in vec2 frag_UV;
in vec3 frag_NORMAL;

uniform int render_mode;
uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main() {
	//Ambient.
	vec3 ambient = light.ambient * material.ambient;
	//Diffuse.
	vec3 norm = normalize(frag_NORMAL);
	vec3 lightDir = normalize(light.position - frag_POS);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	//Specular.
	vec3 viewDir = normalize(view_pos - frag_POS);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * material.specular);

	if(render_mode == 0) fragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
	else if(render_mode == 1) fragColor = vec4(frag_UV, 1.0, 1.0);
	else if(render_mode == 2) fragColor = vec4(frag_NORMAL, 1.0);
}