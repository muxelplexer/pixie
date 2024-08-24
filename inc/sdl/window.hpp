#pragma once
#include "sdl/types.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <utility>

namespace sdl
{
    struct window_params
    {
        std::string title{""};
        int16_t     width{0};
        int16_t     height{0};
        bool        vsync{false};
    };

    class window
    {
    public:
        /*
         * TODO: Fix pointers potentially being uninitialized?
         *
         * When a nullptr is moved into the constructor it would continue trying
         * to initialize _window with potentiall uninitialized values.
         */

        window(std::unique_ptr<window_params>&& params)
            : _params{std::move(params)},
              _window{sdl::unique_ptr<SDL_Window>(
                  SDL_CreateWindow(_params->title.c_str(), _params->width, _params->height, 0))
              },
              _renderer{sdl::unique_ptr<SDL_Renderer>(
                  SDL_CreateRenderer(_window.get(), nullptr)
              )}
        {
            SDL_SetRenderVSync(_renderer.get(), _params->vsync ? 1 : 0);
        }

        void init_imgui();
        void set_draw_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        void render_clear();
        void render_present();

    private:
        std::unique_ptr<window_params> _params{nullptr};
        sdl::unique_ptr<SDL_Window>    _window{nullptr};
        sdl::unique_ptr<SDL_Renderer>  _renderer{nullptr};
    };
}
