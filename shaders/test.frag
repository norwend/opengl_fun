#version 330 core

in vec3 mColor; 
out vec4 color;

void main() {
    color = vec4(mColor.xyz, 1.0f);
}
