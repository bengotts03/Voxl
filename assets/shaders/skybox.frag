#version 330 core

out vec4 FragColor;

in vec3 WorldPosition;

vec4 SkyboxGradient(){
    vec3 direction = normalize(WorldPosition);

    vec3 topColour = vec3(0.7, 0.9, 1.0);
    vec3 bottomColour = vec3(0.3, 0.6, 1.0);

    float height = (direction.y + 1.0f) * 0.5f;

    vec3 skyboxColour = mix(topColour, bottomColour, height);

    return vec4(skyboxColour, 1.0);
}

void main() {
    FragColor = SkyboxGradient();
}