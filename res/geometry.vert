#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 model_view_matrix;
uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
uniform int anim_frame;

out vec2 tex_coord_circle;
out vec3 frag_position;
out vec3 frag_normal;

void main() {
	tex_coord_circle = texture_coord;
	frag_normal = normal_matrix * vertex_normal;
	frag_position = vec3(model_view_matrix * vec4(vertex_position, 1.0));
	gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
}