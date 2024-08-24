#include "ui/ui.hpp"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>

namespace ui
{
    void init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
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
            ImGui::EndMainMenuBar();
        }

        ImGui::Render();
    }

    void shutdown()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}

