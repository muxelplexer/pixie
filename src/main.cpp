#include <cstdint>
#include <memory>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#include "sdl/window.hpp"
#include "ui/ui.hpp"

namespace {
    std::unique_ptr<sdl::window> s_window{nullptr};
    enum fade_direction : int8_t
    {
        UP = 1,
        DOWN = -1
    };
    uint8_t s_red = 0;
    fade_direction s_fade_dir{fade_direction::UP};
    bool s_running{true};
}

SDL_AppResult SDL_AppInit(void **, int , char *[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }
    auto window_params{std::make_unique<sdl::window_params>(
        "pixie",
        1280,
        720,
        true
    )};
    s_window = std::make_unique<sdl::window>(std::move(window_params));
    if (s_window == nullptr)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    ui::init();
    s_window->init_imgui();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, const SDL_Event *event)
{
    ui::process_events(event);
    if (event->type == SDL_EVENT_QUIT) {
        s_running = false;
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch(event->key.scancode)
        {
        case SDL_SCANCODE_Q:
            s_running = false;
            break;
        default:
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *)
{
    ui::render_new_frame();

    /* since we're always fading red, we leave green and blue at zero.
       alpha doesn't mean much here, so leave it at full (255, no transparency). */
    s_window->set_draw_color(s_red, 0, 0, 255);

    /* clear the window to the draw color. */
    s_window->render_clear();

    s_window->render_present();
    /* update the color for the next frame we will draw. */
    if (s_fade_dir == fade_direction::UP)
    {
        if (s_red == 255)
        {
            s_fade_dir = fade_direction::DOWN;
        } else {
            ++s_red;
        }
    } else if (s_fade_dir == fade_direction::DOWN)
    {
        if (s_red == 0)
        {
            s_fade_dir = fade_direction::UP;
        } else {
            --s_red;
        }
    }

    return s_running ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

void SDL_AppQuit(void *)
{
    ui::shutdown();
}

