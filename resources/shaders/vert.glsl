#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout(set = 0, binding = 0) uniform camera {
    mat4 view;
    mat4 proj;
    mat4 viewProj;
};

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = viewProj * vec4(inPos, 1.0);
    fragColor = inColor;
}
