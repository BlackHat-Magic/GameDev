#ifndef EVENTS_H
#define EVENTS_H

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL.h>

#include <stdbool.h>
#include <cglm/cglm.h>

enum PauseState {
    MAIN_MENU,
    PAUSED,
    PLAYING,
    GAME_OVER
};

typedef struct GameState {
    int mouseX;
    int mouseY;
    int last_mouseX;
    int last_mouseY;
    bool mouse_pressed;
    SDL_Point mouse_point;
    bool first_mouse;
    bool mouse_captured;

    enum PauseState state;
    bool* running;
    SDL_Window* window;

    vec3 camera_pos;
    vec3 camera_front;
    vec3 camera_up;
    float yaw;
    float pitch;
    float camera_speed;
    float mouse_sensitivity;
} GameState;

void init_camera (GameState* state);

void handle_event (SDL_Event* event, GameState* state);

void process_input (GameState* state, float dt);

#endif