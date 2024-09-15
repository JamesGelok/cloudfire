#version 330 core

in vec3 vertexNormal;  // Normal passed from the vertex shader
in vec3 fragPos;       // Fragment position for lighting calculations

out vec4 FragColor;

uniform vec3 lightDir;  // Light direction (assumed to be normalized)
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 objectColor; // Uniform for object color

void main() {
    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(vertexNormal);
    float diff = max(dot(norm, normalize(-lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine ambient and diffuse components
    vec3 result = (ambient + diffuse) * objectColor;

    // Final color output
    FragColor = vec4(result, 1.0);
}
