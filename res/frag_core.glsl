#version 330 core
out vec4 fragColor;

in vec3 baseColor;

void main() {
	fragColor = vec4(baseColor, 1.0f);
}