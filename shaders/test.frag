#version 330 core

in vec2 frag_tex_pos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float offset;

out vec4 color;

void main() {
    color = texture(texture0, frag_tex_pos);
}
