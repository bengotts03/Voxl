#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 Position;
out vec3 Colour;

uniform mat4 cameraMatrix;

void main()
{
    Position = vec3(aPos);
    Colour = aColour;

    gl_Position = cameraMatrix * vec4(Position, 1.0);
}
