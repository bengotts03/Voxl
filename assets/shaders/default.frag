#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Colour;
in vec2 TextureCoords;
in vec3 Normals;

uniform sampler2D diffuse0;
uniform vec3 cameraPosition;

vec4 DirectionalLighting(){
    float ambient = 0.5f;
    vec4 lightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Diffuse
    vec3 normal = normalize(Normals);
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(cameraPosition - Position);
    vec3 reflectionDirection = reflect(-lightDir, normal);
    float specularAmount = pow(max(dot(viewDir, reflectionDirection), 0.0f), 32.0f);
    float specular = specularStrength * specularAmount;

    return (texture(diffuse0, TextureCoords) * (diffuse + ambient + specular)) * vec4(Colour, 1.0f) * lightColour;
}

void main()
{
    FragColor = DirectionalLighting();
}