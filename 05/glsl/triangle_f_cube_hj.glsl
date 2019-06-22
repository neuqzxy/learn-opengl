#version 330 core
out vec4 FragColor;

uniform vec3 aColor;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.1;
    FragColor = ambientStrength * vec4(aColor, 1.0f) * vec4(lightColor, 1.0f);
}