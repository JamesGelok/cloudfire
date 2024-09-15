#version 330 core

layout (location = 0) in vec3 aPos;   // Vertex position for 3D objects

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate the final position of the vertex on screen
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
