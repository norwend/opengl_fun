#version 330 core
layout (location = 0) in vec3 mPos;
layout (location = 1) in vec3 mCol;
out vec3 mColor;

void main() {
    gl_Position = vec4(mPos.xyz, 1.0f);
    mColor = mCol;
}
