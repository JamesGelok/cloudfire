#version 330 core

in vec3 vertexNormal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 objectColor;
uniform float specularStrength;
uniform float shininess;

void main() {
    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(vertexNormal);
    float diff = max(dot(norm, normalize(-lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    vec3 viewDir = normalize(-fragPos);  // Assumes the camera is at (0,0,0)
    vec3 reflectDir = reflect(normalize(lightDir), norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
