#version 460
in vec2 tex_coords;
in vec3 frag_position;
in vec3 frag_normal;

layout(binding = 0) uniform sampler2D InTexture_Diffuse;
layout(binding = 1) uniform sampler2D InTexture_Specular;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float ambient_factor;
uniform float diffuse_factor;
uniform float specular_factor;
uniform float shininess;

out vec4 frag_color;

void main() {
	//Ambient
	vec3 ambient = ambient_factor * light_color
	* vec3(texture(InTexture_Diffuse, tex_coords));
	//Diffuse
	vec3 normal = normalize(frag_normal);
	vec3 light_dir = normalize(light_position - frag_position);
	vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0)
	* vec3(texture(InTexture_Diffuse, tex_coords));
	//Specular
	vec3 camera_dir = normalize(-frag_position);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_value = pow(max(dot(camera_dir, reflect_dir), 0.0), shininess);
	vec3 specular = specular_factor * specular_value * light_color
	* vec3(texture(InTexture_Specular, tex_coords));
	//Final
	frag_color = vec4(ambient + diffuse + specular, 1.f);
}