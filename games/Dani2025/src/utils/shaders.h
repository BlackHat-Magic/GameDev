#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>

char* load_shader_source (const char* filepath);

GLuint compile_shader (const char* source, GLenum shaderType);

GLuint create_shader_program (GLuint vertexShader, GLuint fragmentShader);

#endif