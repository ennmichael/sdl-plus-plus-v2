#include "flow.h"
#include "SDL2/SDL.h"

namespace Sdl
{
    Basic_loop::Basic_loop(Function f) noexcept
        : m_f { f }
    {
    }
    
    void Basic_loop::start()
    {
        while (!SDL_QuitRequested())
        {
            m_f();
        }
    }
    
    Event_loop::Event_loop(Event_function event_f, 
                           Unconditional_function unconditional_f) noexcept
        : m_event_f { event_f }
        , m_unconditional_f { unconditional_f }
    { }
    
    void Event_loop::start()
    {
        while (true)
        {
            while (SDL_PollEvent(&m_event))
            {
                if (m_event_f(m_event) == Event_result::Quit)
                    return; // Quit if the user says so
            }
            
            m_unconditional_f();
        }
    }
    
    Timer::Timer(std::chrono::milliseconds delay) noexcept
    {
        set_delay(delay);
    }
    
    bool Timer::ready() noexcept
    {
        auto ticks = SDL_GetTicks();
        
        if (ticks >= delay_ + current_)
        {
            current_ = ticks;
            return true;
        }
        
        return false;
    }
    
    std::chrono::milliseconds Timer::get_delay() const noexcept
    {
        return std::chrono::milliseconds { delay_ };
    }
    
    void Timer::set_delay(std::chrono::milliseconds delay) noexcept
    {
        delay_ = static_cast <Uint32>(delay.count());
    }
}
