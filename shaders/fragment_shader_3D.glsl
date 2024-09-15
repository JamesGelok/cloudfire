#version 330 core

out vec4 FragColor;

uniform vec3 objectColor; // Uniform for object color

void main() {
    FragColor = vec4(objectColor, 1.0); // Use uniform color for 3D objects
}
