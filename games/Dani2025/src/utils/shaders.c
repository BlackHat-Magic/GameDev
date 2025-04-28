#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>

char* load_shader_source (const char* filepath) {
    FILE* file = fopen (filepath, "rb");
    if (!file) {
        fprintf (stderr, "ERROR:\tCould not open shader file: %s\n", filepath);
        return (NULL);
    }

    fseek (file, 0, SEEK_END);
    long length = ftell (file);
    fseek (file, 0, SEEK_SET);

    if (length <=0) {
        fprintf (stderr, "ERROR:\tInvalid or empty shader file: %s\n", filepath);
        fclose (file);
        return (NULL);
    }

    char* buffer = (char*) calloc (length + 1, sizeof (char));
    if (!buffer) {
        fprintf (stderr, "ERROR:\tCould not read entire shader file: %s\n", filepath);
        free (buffer);
        return (NULL);
    }

    size_t bytes_read = fread (buffer, 1, length, file);
    fclose (file);
    if (bytes_read != (size_t) length) {
        fprintf (stderr, "ERROR:\tCould not read entire shader file: %s (read %zu bytes, expected %ld)\n", filepath, bytes_read, length);
        free (buffer);
        return (NULL);
    }

    buffer[length] = '\0';

    return (buffer);
}

GLuint compile_shader (const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader (shaderType);
    if (!shader) {
        fprintf (stderr, "ERROR:\tCould not create shader object (type: %d)\n", shaderType);
        return (0);
    }
    glShaderSource (shader, 1, &source, NULL);
    glCompileShader (shader);

    GLint success;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = (char*) calloc (logLength, sizeof (char));
        if (infoLog) {
            glGetShaderInfoLog (shader, logLength, NULL, infoLog);
            fprintf (stderr, "ERROR:\tShader compilation failed (type: %d):\n%s\n", shaderType, infoLog);
            free (infoLog);
        } else {
            fprintf (stderr, "ERROR:\tShader compilation failed (type: %d) and could not allocate memory for log.\n", shaderType);
        }
        glDeleteShader (shader);
        return (0);
    }

    return (shader);
}

GLuint create_shader_program (GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram ();
    if (!program) {
        fprintf (stderr, "ERROR:\tCould not create shader program object.\n");
        return (0);
    }
    glAttachShader (program, vertexShader);
    glAttachShader (program, fragmentShader);
    glLinkProgram (program);

    GLint success;
    glGetProgramiv (program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv (program, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = (char*) calloc (logLength, sizeof (char));
        if (infoLog) {
            glGetProgramInfoLog (program, logLength, NULL, infoLog);
            fprintf (stderr, "ERROR:\tShader program linking failed:\n%s\n", infoLog);
            free (infoLog);
        } else {
            fprintf (stderr, "ERROR:\tShader program linking failed and could not allocate memory for log.\n");
        }
        glDeleteProgram (program);
        program = 0;
    }

    glDetachShader (program, vertexShader);
    glDetachShader (program, fragmentShader);
    glDeleteShader (vertexShader);
    glDeleteShader (fragmentShader);

    return (program);
}