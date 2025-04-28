#version 300 es
precision mediump float;

// Input from vertex shader (interpolated)
in vec3 vertexColor; // Receive interpolated color (NEW)

// Output fragment color
out vec4 FragColor;

void main () {
    // Use the interpolated color received from the vertex shader
    FragColor = vec4 (vertexColor, 1.0f); // (NEW)
}
