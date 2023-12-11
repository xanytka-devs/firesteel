#version 330 core
out vec4 fragColor;

in vec3 baseColor;
in vec2 UV;

uniform sampler2D albedo;

void main() {
	//fragColor = vec4(baseColor, 1.0f);
	fragColor = texture(albedo, UV);
}