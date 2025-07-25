#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPosition;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

void main() {
    WorldPosition = aPos;
    mat4 rotView = mat4(mat3(view));
    vec4 pos = projection * rotView * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}