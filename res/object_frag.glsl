#version 330 core
out vec4 fragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 frag_POS;
in vec2 frag_UV;
in vec3 frag_NORMAL;

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main() {
	//Ambient.
	vec3 ambient = light.ambient * material.ambient;
	//Diffuse.
	vec3 normal = normalize(frag_NORMAL);
	vec3 light_dir = normalize(light.pos - frag_POS);
	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	//Specular.
	vec3 view_dir = normalize(view_pos - frag_POS);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * material.specular);
	//Output.
	fragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
}