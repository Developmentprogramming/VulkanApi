#version 430 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 o_FragColor;

vec3 colors[] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(a_Position, 1.0);
    o_FragColor = colors[gl_VertexIndex];
}
