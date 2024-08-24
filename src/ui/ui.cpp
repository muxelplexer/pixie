#include "ui/ui.hpp"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <memory>

#include "ui/widgets/fps_counter.hpp"

namespace ui
{
    std::unique_ptr<widgets::fps_counter> counter{nullptr};
    void init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
        counter = std::make_unique<widgets::fps_counter>();
    }

    void process_events(const SDL_Event* e)
    {
        ImGui_ImplSDL3_ProcessEvent(e);
    }

    void render_new_frame()
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            ImGui::Text("This is a tooltip text. I am mean! >:(");
            counter->render();
            ImGui::EndMainMenuBar();
        }

        ImGui::Render();
        counter->update();
    }

    void shutdown()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}

