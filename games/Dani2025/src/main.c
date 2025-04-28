// standard library
#include <SDL3/SDL_timer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// glad
#include <glad/glad.h>

// SDL
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL.h>

#define CHECK_SDL_ERROR(condition, message) \
    if (condition) { \
        fprintf (stderr, "%s: %s\n", message, SDL_GetError ()); \
        goto cleanup; \
    }

// cglm
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

// utils
#include "utils/events.h"
#include "utils/shaders.h"

const char* VERTEX_SHADER_PATH = "shaders/test.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/test.frag";

int main (int argc, char* argv[]) {
    // initialize SDL
    SDL_Window* window = NULL;
    SDL_GLContext context = NULL;
    GLuint shaderProgram = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;

    //
    GLint modelLoc = -1;
    GLint viewLoc = -1;
    GLint projectionLoc = -1;

    // shaders
    char* vertexShaderSource = NULL;
    char* fragmentShaderSource = NULL;
    int exit_code = EXIT_FAILURE;

    // timing
    Uint64 last_frame_time = 0;
    Uint64 current_frame_time = SDL_GetPerformanceCounter ();
    float dt = 0.0f;

    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        fprintf (stderr, "Could not initialize SD: %s\n", SDL_GetError ());
        return (EXIT_FAILURE);
    }
    printf ("SDL initialized successfully.\n");

    // Set OpenGL stuff
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
    printf ("Set OpenGL context to OpenGL ES 3.0.\n");

    // create window
    int window_width = 800;
    int window_height = 600;
    window = SDL_CreateWindow (
        "SDL3 OpenGL Window",
        window_width,
        window_height,
        SDL_WINDOW_OPENGL
    );
    CHECK_SDL_ERROR (window == NULL, "Failed to create window.");
    printf ("Window created successfully.\n");

    // create OpenGL context
    context = SDL_GL_CreateContext (window);
    CHECK_SDL_ERROR (context == NULL, "Failed to create OpenGL context.");
    printf("OpenGL context created successfully.\n");
    if (!gladLoadGLES2Loader ((GLADloadproc) SDL_GL_GetProcAddress)) {
        fprintf (stderr, "Failed to initialize GLAD\n");
        goto cleanup;
    }
    printf ("GLAD initialized successfully.\n");
    printf ("OpenGL Renderer: %s\n", glGetString (GL_RENDERER));
    printf ("OpenGL Version: %s\n", glGetString (GL_VERSION));
    printf ("GLSL Version: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

    // load shaders
    printf ("Loading shaders...\n");
    vertexShaderSource = load_shader_source (VERTEX_SHADER_PATH);
    if (!vertexShaderSource) goto cleanup;
    fragmentShaderSource = load_shader_source(FRAGMENT_SHADER_PATH);
    if (!fragmentShaderSource) goto cleanup;

    // compile vertex shader
    printf ("Copmpiling vertex shader...\n");
    GLuint vertexShader = compile_shader (vertexShaderSource, GL_VERTEX_SHADER);
    if (!vertexShader) goto cleanup;

    // compile fragment shader
    printf ("Compiling fragment shader...\n");
    GLuint fragmentShader = compile_shader (fragmentShaderSource, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        glDeleteShader (vertexShader);
        goto cleanup;
    }

    // we don't need source strings anymore
    free (vertexShaderSource); vertexShaderSource = NULL;
    free (fragmentShaderSource); fragmentShaderSource = NULL;

    // link shaders
    printf ("Linking shader program...\n");
    shaderProgram = create_shader_program (vertexShader, fragmentShader);
    if (!shaderProgram) goto cleanup;
    printf ("Shader program created successfully (ID: %u).\n", shaderProgram);

    modelLoc = glGetUniformLocation (shaderProgram, "model");
    viewLoc = glGetUniformLocation (shaderProgram, "view");
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    printf ("Uniform locations: model=%d, view=%d, projection=%d\n", modelLoc, viewLoc, projectionLoc);
    if (modelLoc == -1 || viewLoc == -1 || projectionLoc == -1) {
        fprintf (stderr, "Warning:\tCould not find uniform location for 'model'\n");
    }

    // vsync
    if (SDL_GL_SetSwapInterval (1) < 0) {
        fprintf (stderr, "Warning: Unable to set VSync. SDL Error: %s\n", SDL_GetError ());
    }

    // 3D depth testing
    glEnable (GL_DEPTH_TEST);
    glViewport (0, 0, window_width, window_height);

    // set some window stuffs
    SDL_GetWindowSize (window, &window_width, &window_height);
    glViewport (0, 0, window_width, window_height);

    // THE TRIANGLE \[T]/
    float vertices[] = {
        // Positions         // Colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f,   -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f 
    };
    // Vertex Buffer Object
    glGenBuffers (1, &VBO);
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    // Vertex Array Object
    glGenVertexArrays (1, &VAO);
    glBindVertexArray (VAO);
    // link vertex attributes
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    // configure attribute pointer
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)0);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)(3 * sizeof (float)));
    glEnableVertexAttribArray (1);
    // unbind VAO and VBO
    glBindVertexArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    // game loop
    bool running = true;

    GameState gameState;
    gameState.mouseX = 0;
    gameState.mouseY = 0;
    gameState.last_mouseX = window_width / 2;
    gameState.last_mouseY = window_height / 2;
    gameState.mouse_pressed = false;
    gameState.first_mouse = true;
    gameState.mouse_captured = false;
    gameState.state = PLAYING;
    gameState.running = &running;
    gameState.window = window;

    init_camera (&gameState);

    SDL_Event event;
    Uint64 start_time = SDL_GetTicks ();

    last_frame_time = current_frame_time;

    while (running) {
        current_frame_time = SDL_GetPerformanceCounter ();
        dt = (float) (((double) current_frame_time - (double) last_frame_time) / (double) SDL_GetPerformanceFrequency ());
        last_frame_time = current_frame_time;

        SDL_Event event;

        while (SDL_PollEvent (&event)) {
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                window_width = event.window.data1;
                window_height = event.window.data2;
                glViewport (0, 0, window_width, window_height);
                printf ("Window resized to %d x %d\n", window_width, window_height);
            }
            
            handle_event (&event, &gameState);
        }

        process_input (&gameState, dt);

        switch (gameState.state) {
            case MAIN_MENU:
                // main menu
                break;
            case PAUSED:
                // pause menu
                break;
            case PLAYING:
                // playing
                break;
            case GAME_OVER:
                // game over
                break;
        }

        mat4 model = GLM_MAT4_IDENTITY_INIT;
        float angle = (float) SDL_GetTicks () / 1000.0f * glm_rad (180.0f);
        glm_rotate (model, angle, (vec3) {0.0f, 1.0f, 0.0f});

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        vec3 center;
        glm_vec3_add (gameState.camera_pos, gameState.camera_front, center);
        glm_lookat (gameState.camera_pos, center, gameState.camera_up, view);

        mat4 projection = GLM_MAT4_IDENTITY_INIT;
        if (window_height > 0) {
            glm_perspective (
                glm_rad (45.0f),
                (float) window_width / (float) window_height,
                0.1f,
                100.0f,
                projection
            );
        }

        // rendering
        glClearColor (0.0f, 0.1f, 0.2f, 1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram (shaderProgram);

        // send matrices to shader uniforms
        if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);
        if (viewLoc != -1) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);
        if (projectionLoc != -1) glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const GLfloat*)projection);

        glBindVertexArray (VAO);
        glDrawArrays (GL_TRIANGLES, 0, 3);
        glBindVertexArray (0);

        SDL_GL_SwapWindow (window);
    }

    exit_code = EXIT_SUCCESS;

cleanup:
    printf ("Cleaning up...\n");
    
    free (vertexShaderSource);
    free (fragmentShaderSource);

    glDeleteVertexArrays (1, &VAO);
    glDeleteBuffers (1, &VBO);
    glDeleteProgram (shaderProgram);
    printf ("OpenGL objects deleted.\n");

    if (context) {
        SDL_GL_DestroyContext (context);
        printf ("OpenGL context destroyed.\n");
    }
    if (window) {
        SDL_DestroyWindow (window);
        printf ("Window destroyed.\n");
    }
    SDL_Quit ();
    printf ("SDL quit.\n");

    return exit_code;
}