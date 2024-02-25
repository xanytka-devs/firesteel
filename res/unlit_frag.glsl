#version 330 core
out vec4 fragColor;

uniform sampler2D diffuse0;

in vec3 frag_POS;
in vec2 frag_UV;
in vec3 frag_NORMAL;

uniform vec4 color;
uniform int render_mode;
uniform int no_textures;

void main() {
	vec4 diff_map = color;
	if(no_textures != 1) {
		diff_map = texture(diffuse0, frag_UV);
	}
	//Output.
	if(render_mode == 0) fragColor = diff_map;
	else if(render_mode == 1) fragColor = vec4(frag_UV, 1.0, 1.0);
	else if(render_mode == 2) fragColor = vec4(frag_NORMAL, 1.0);
}