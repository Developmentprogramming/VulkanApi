#version 430 core

layout (location = 0) in vec3 o_FragColor;
layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(o_FragColor, 1.0);
}
