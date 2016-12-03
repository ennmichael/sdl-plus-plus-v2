#include "flow.h"

#include "SDL2/SDL.h"

namespace Sdl
{
    Main_loop::Main_loop(const Function& f) noexcept
        : m_f { f }
    {
    }
    
    void Main_loop::start()
    {
        while (!SDL_QuitRequested())
        {
            m_f();
        }
    }
}
