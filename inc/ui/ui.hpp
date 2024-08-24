#pragma once

union SDL_Event;
namespace ui
{
    void init();
    void process_events(const SDL_Event* e);
    void render_new_frame();
    void shutdown();
}
