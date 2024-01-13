#version 330 core

out vec4 fragColor;

in vec3 frag_POS;
in vec2 frag_UV;
in vec3 frag_NORMAL;

uniform int render_mode;
uniform sampler2D diffuse0;
uniform vec4 albedo_color;

void main() {
	if(render_mode == 0) fragColor = texture2D(diffuse0, frag_UV) * albedo_color, 1.0;
	else if(render_mode == 1) fragColor = vec4(frag_UV, 1.0, 1.0);
	else if(render_mode == 2) fragColor = vec4(frag_NORMAL, 1.0);
}