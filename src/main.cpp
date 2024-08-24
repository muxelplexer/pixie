#include <cstdint>
#include <memory>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace {
    template<class T>
    struct deleter_of;

    template<>
    struct deleter_of<SDL_Window>{ void operator()(SDL_Window* p) { SDL_DestroyWindow(p); }};

    template<>
    struct deleter_of<SDL_Renderer>{ void operator()(SDL_Renderer* p) { SDL_DestroyRenderer(p); }};

    template<class T>
    using unique_ptr = std::unique_ptr<T, deleter_of<T>>;

    unique_ptr<SDL_Window> s_window{nullptr};
    unique_ptr<SDL_Renderer> s_renderer{nullptr};

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

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0, &window, &renderer) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }
    s_window.reset(window);
    s_renderer.reset(renderer);
    SDL_SetRenderVSync(renderer, 1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(s_window.get(), s_renderer.get());
    ImGui_ImplSDLRenderer3_Init(s_renderer.get());

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, const SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
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

    if (SDL_GetWindowFlags(s_window.get()) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return s_running ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("This is a tooltip text. I am mean! >:(");
        ImGui::EndMainMenuBar();
    }

    ImGui::Render();

    /* since we're always fading red, we leave green and blue at zero.
       alpha doesn't mean much here, so leave it at full (255, no transparency). */
    SDL_SetRenderDrawColor(s_renderer.get(), s_red, 0, 0, 255);

    /* clear the window to the draw color. */
    SDL_RenderClear(s_renderer.get());

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), s_renderer.get());

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(s_renderer.get());

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
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

