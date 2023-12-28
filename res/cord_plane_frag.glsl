#version 330 core

uniform mat4 view;
uniform mat4 proj;

out mat4 frag_view;
out mat4 frag_proj;
out vec3 near_point;
out vec3 far_point;

// Grid position are in clipped space
vec3 gridPlane[6] = vec3[] (
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = gridPlane[gl_VertexID].xyz;
    near_point = UnprojectPoint(p.x, p.y, 0.0, view, proj).xyz; // unprojecting on the near plane
    far_point = UnprojectPoint(p.x, p.y, 1.0, view, proj).xyz; // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
    frag_view = view;
    frag_proj = proj;
}
