#version 330 core
layout (location = 0) in vec3 mPos;

void main() {
    gl_Position = vec4(mPos.xyz, 1.0f);
}
