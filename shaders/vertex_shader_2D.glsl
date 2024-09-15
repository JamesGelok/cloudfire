#version 330 core

layout (location = 0) in vec3 aPos;  // Use vec3 for 3D positions
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;        // New uniform for the view matrix
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0); // Apply view matrix
    vertexColor = aColor;
}
