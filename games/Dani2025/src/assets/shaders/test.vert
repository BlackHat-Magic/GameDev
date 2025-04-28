#version 300 es
precision mediump float;

// Input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// transformation matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;

void main () {
    gl_Position = projection * view * model * vec4 (aPos, 1.0);
    vertexColor = aColor;
}
