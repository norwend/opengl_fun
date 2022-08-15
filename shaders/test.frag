#version 330 core

in vec3 frag_col;
in vec2 frag_tex_pos;
in float sin_fr0;
in float sin_fr1;

uniform sampler2D kag_texture;

out vec4 color;

void main() {
//    color = vec4(0.7f, 0.3f, 0.3f, 1.0f);
    color = texture(kag_texture, frag_tex_pos) * vec4(frag_col.x + sin_fr0 - sin_fr1, frag_col.y + abs(sin_fr0), frag_col.z + abs(sin_fr1), 1.0);
}
