#version 330 core

layout (location = 0) in vec3 aPos;   // Vertex position for 3D objects
layout (location = 1) in vec3 aNormal; // Vertex normal for lighting calculations

out vec3 fragPos;        // Fragment position passed to the fragment shader
out vec3 vertexNormal;   // Normal passed to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate the fragment position in world space
    fragPos = vec3(model * vec4(aPos, 1.0));

    // Pass the vertex normal to the fragment shader
    vertexNormal = mat3(transpose(inverse(model))) * aNormal;

    // Calculate the final position of the vertex on screen
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
