#include <memory>

#include <SDL3/SDL_render.h>
namespace sdl
{
    template<class T>
    struct deleter_of;

    template<>
    struct deleter_of<SDL_Window>{ void operator()(SDL_Window* p) { SDL_DestroyWindow(p); }};

    template<>
    struct deleter_of<SDL_Renderer>{ void operator()(SDL_Renderer* p) { SDL_DestroyRenderer(p); }};

    template<class T>
    using unique_ptr = std::unique_ptr<T, deleter_of<T>>;
}
