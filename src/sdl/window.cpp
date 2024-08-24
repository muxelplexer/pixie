#include "sdl/window.hpp"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace sdl
{

    void window::init_imgui()
    {
        ImGui_ImplSDL3_InitForSDLRenderer(_window.get(), _renderer.get());
        ImGui_ImplSDLRenderer3_Init(_renderer.get());
    }

    void window::set_draw_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    {
        SDL_SetRenderDrawColor(_renderer.get(), r, g, b, a);
    }
    void window::render_clear()
    {
        SDL_RenderClear(_renderer.get());
    }
    void window::render_present()
    {
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer.get());
        SDL_RenderPresent(_renderer.get());
    }
}
