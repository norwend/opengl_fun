#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_col;
layout (location = 2) in vec2 vert_tex_pos;

out vec3 frag_col;
out vec2 frag_tex_pos;

uniform float sin0;
uniform float sin1;

out float sin_fr0;
out float sin_fr1;
	

void main() {
    gl_Position = vec4(vert_pos.x + sin0, vert_pos.y + sin1, vert_pos.z, 1.0f);
    frag_col = vert_col;
    frag_tex_pos = vert_tex_pos;
    sin_fr0 = sin0;
    sin_fr1 = sin1;
}
