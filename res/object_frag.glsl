#version 330 core
out vec4 fragColor;

struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float emission_factor;
	vec3 emission_color;
	float shininess;
};

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D emission0;

struct DirectionalLight {
	//Location.
	vec3 direction;
	//Base variables.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;
};
struct PointLight {
	//Location.
	vec3 position;
	//Attenuation.
	float k0;
	float k1;
	float k2;
	//Base variables.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;
};
struct SpotLight {
	//Location.
	vec3 position;
	vec3 direction;
	//Attenuation.
	float k0;
	float k1;
	float k2;
	//Cutoffs.
	float inner_cutoff;
	float outer_cutoff;
	//Base variables.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;
};

in vec3 frag_POS;
in vec2 frag_UV;
in vec3 frag_NORMAL;

uniform int render_mode;
uniform int no_textures;
uniform Material material;
uniform vec3 view_pos;

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 8

uniform DirectionalLight dir_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int num_point_lights;
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];
uniform int num_spot_lights;

vec3 calc_dir_light(vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map);
vec3 calc_point_light(int idx, vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map);
vec3 calc_spot_light(int idx, vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map);

void main() {
	vec3 norm = normalize(frag_NORMAL);
	vec3 view_dir = normalize(view_pos - frag_POS);
	vec3 diff_map = vec3(texture(diffuse0, frag_UV));
	vec3 spec_map = vec3(texture(specular0, frag_UV));
	vec3 emis_map = vec3(texture(emission0, frag_UV));
	if(no_textures == 1) {
		diff_map = material.diffuse;
		spec_map = material.specular;
		emis_map = material.emission;
	}
	vec3 result;
	//Directional light.
	result = calc_dir_light(norm, view_dir, diff_map, spec_map, emis_map);
	//Point lights.
	for(int p = 0; p < num_point_lights; p++)
		result += calc_point_light(p, norm, view_dir, diff_map, spec_map, emis_map);
	//Spot lights.
	for(int s = 0; s < num_spot_lights; s++)
		result += calc_spot_light(s, norm, view_dir, diff_map, spec_map, emis_map);
	//Output.
	fragColor = vec4(result, 1.0);
}

vec3 calc_dir_light(vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map) {
	//Ambient.
	vec3 ambient = dir_light.ambient * diff_map;
	//Diffuse.
	vec3 light_dir = normalize(-dir_light.direction);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = dir_light.diffuse * (diff * diff_map);
	//Specular.
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 128);
	vec3 specular = dir_light.specular * (spec * spec_map);
	//Emission
    vec3 glow = emission_map * material.emission_factor * material.emission_color;
	//Output.
	if(render_mode == 0) return (ambient + diffuse + specular + glow) * dir_light.color;
	else if(render_mode == 1) return vec3(frag_UV, 1.0);
	else if(render_mode == 2) return frag_NORMAL * spec_map;
}

vec3 calc_point_light(int idx, vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map) {
	//Ambient.
	vec3 ambient = point_lights[idx].ambient * diff_map;
	//Diffuse.
	vec3 light_dir = normalize(point_lights[idx].position - frag_POS);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = point_lights[idx].diffuse * (diff * diff_map);
	//Specular.
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 128);
	vec3 specular = point_lights[idx].specular * (spec * spec_map);
	//Emission
    vec3 glow = emission_map * material.emission_factor * material.emission_color;
	//Attenuation.
	float dist = length(point_lights[idx].position - frag_POS);
	float attenuation = 1.0 / (point_lights[idx].k0 + point_lights[idx].k1 * dist + point_lights[idx].k2 * (dist * dist));
	//Output.
	if(render_mode == 0) return ((ambient + diffuse + specular + glow) * point_lights[idx].color) * attenuation;
	else if(render_mode == 1) return vec3(frag_UV, 1.0);
	else if(render_mode == 2) return frag_NORMAL * spec_map;
}

vec3 calc_spot_light(int idx, vec3 norm, vec3 view_dir, vec3 diff_map, vec3 spec_map, vec3 emission_map) {
	//Ambient.
	vec3 ambient = spot_lights[idx].ambient * diff_map;
	vec3 light_dir = normalize(spot_lights[idx].position - frag_POS);
	float theta = dot(light_dir, normalize(-spot_lights[idx].direction));
	if(theta > spot_lights[idx].outer_cutoff) {
		//Diffuse.		
		float diff = max(dot(norm, light_dir), 0.0);
		vec3 diffuse = spot_lights[idx].diffuse * (diff * diff_map);
		//Specular.
		vec3 reflect_dir = reflect(-light_dir, norm);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 128);
		vec3 specular = spot_lights[idx].specular * (spec * spec_map);
		//Intensity.
		float intensity = clamp((theta - spot_lights[idx].outer_cutoff)/
		(spot_lights[idx].inner_cutoff - spot_lights[idx].outer_cutoff),
		0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;
		//Emission
		vec3 glow = emission_map * material.emission_factor * material.emission_color;
		//Attenuation.
		float dist = length(spot_lights[idx].position - frag_POS);
		float attenuation = 1.0 / (spot_lights[idx].k0 + spot_lights[idx].k1 * dist + spot_lights[idx].k2 * (dist * dist));
		//Output.
		if(render_mode == 0) return ((ambient + diffuse + specular + glow) * spot_lights[idx].color) * attenuation;
		else if(render_mode == 1) return vec3(frag_UV, 1.0);
		else if(render_mode == 2) return frag_NORMAL * spec_map;
	}
	//Output.
	if(render_mode == 0) return ambient * spot_lights[idx].color;
	else if(render_mode == 1) return vec3(frag_UV, 1.0);
	else if(render_mode == 2) return frag_NORMAL * spec_map;
}