#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

out vec2 UV;
uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(pos, 1.0);
	UV = uv;
}