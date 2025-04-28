#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_stdinc.h>
#include <math.h>

#include "events.h"

void update_camera_vectors (GameState* state) {
    vec3 front; 
    front[0] = cosf (glm_rad (state->yaw)) * cosf (glm_rad (state->pitch));
    front[1] = sinf (glm_rad (state->pitch));
    front[2] = sinf (glm_rad (state->yaw)) * cosf (glm_rad (state->pitch));

    glm_vec3_normalize_to (front, state->camera_front);
}

void init_camera (GameState* state) {
    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, state->camera_pos);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, state->camera_front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, state->camera_up);
    state->yaw = -90.0f;
    state->pitch = 0.0f;
    state->camera_speed = 2.5f;
    state->mouse_sensitivity = 0.1f;
    state->first_mouse = true;
    state->mouse_captured = false;

    update_camera_vectors(state);
}

void handle_event (SDL_Event* event, GameState* state) {
    SDL_EventType event_type = (SDL_EventType) event->type;

    if (event_type == SDL_EVENT_QUIT) {
        *(state->running) = false;
        return;
    }

    // escape key; currently enable/disable mouse capture
    if (event_type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        if (state->mouse_captured) {
            SDL_SetWindowRelativeMouseMode (state->window, false);
            state->mouse_captured = false;
            state->first_mouse = true;
        } else {
            SDL_SetWindowRelativeMouseMode (state->window, true);
            state->mouse_captured = true;
            state->first_mouse = true;
        }
        return;
    }

    // mouse buttons; currently only left click
    if (event_type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            state->mouse_pressed = true;
        }
    } else if (event_type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            state->mouse_pressed = false;
        }
    }

    if (event_type == SDL_EVENT_MOUSE_MOTION) {
        state->last_mouseX = state->mouseX;
        state->last_mouseY = state->mouseY;
        state->mouseX = event->motion.x;
        state->mouseY = event->motion.y;
        state->mouse_point.x = event->motion.x;
        state->mouse_point.y = event->motion.y;
    }
}

void process_input (GameState* state, float dt) {
    // don't process input if not focused
    if  (!state->mouse_captured) {
        return;
    }

    const bool* keyboard_state = SDL_GetKeyboardState (NULL);
    float velocity = state->camera_speed * dt;

    vec3 right;
    glm_vec3_crossn (state->camera_front, state->camera_up, right);

    if (keyboard_state[SDL_SCANCODE_W]) {
        glm_vec3_muladds (state->camera_front, velocity, state->camera_pos);
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        glm_vec3_muladds (state->camera_front, -velocity, state->camera_pos);
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        glm_vec3_muladds (right, -velocity, state->camera_pos);
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        glm_vec3_muladds (right, velocity, state->camera_pos);
    }
}