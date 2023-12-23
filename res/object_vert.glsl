#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 frag_POS;
out vec2 frag_UV;
out vec3 frag_NORMAL;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	frag_POS = vec3(model * vec4(pos, 1.0));
	frag_NORMAL = mat3(transpose(inverse(model))) * normal;
	frag_UV = uv;

	gl_Position = projection * view * vec4(frag_POS, 1.0);
}