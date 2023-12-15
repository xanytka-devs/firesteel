#version 330 core
out vec4 fragColor;

in vec2 UV;

uniform sampler2D albedo;

void main() {
	fragColor = texture(albedo, UV);
}