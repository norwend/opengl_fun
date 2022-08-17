#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_tex_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 frag_tex_pos;

void main() {
    gl_Position = projection * view * model * vec4(vert_pos.xyz, 1.0f);
    // gl_Position = vec4(vert_pos.xyz, 1.0f);
    frag_tex_pos = vert_tex_pos;
}
