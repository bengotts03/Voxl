#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTextureCoords;
layout (location = 3) in vec3 aNormals;

out vec3 Position;
out vec3 Colour;
out vec2 TextureCoords;
out vec3 Normals;

uniform mat4 cameraMatrix;
uniform mat4 model;
uniform float scale;

void main()
{
    Position = vec3(model * vec4(aPos * scale, 1));
    Colour = aColour;
    TextureCoords = aTextureCoords;
    Normals = aNormals;

    gl_Position = cameraMatrix * vec4(Position, 1.0);
}
