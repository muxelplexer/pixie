#include "ui/widgets/fps_counter.hpp"

#include <SDL3/SDL_timer.h>
#include <format>

#include <imgui.h>
#include <print>

namespace ui::widgets
{
    void fps_counter::update()
    {
        float current = (float)SDL_GetTicks();
        delta = current - time;
        time = current;
    }

    void fps_counter::render() const
    {
        auto fps{std::format("FPS: {:.2f} : {:6f}ms/frame", 1000./(delta), delta)};
        ImGui::Text("%s", fps.c_str());
    }
}
