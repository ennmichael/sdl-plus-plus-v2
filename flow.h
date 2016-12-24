#ifndef _SDLPP__FLOW__HEADER__
#define _SDLPP__FLOW__HEADER__

#include "drawing.h"
#include <functional>
#include <chrono>
#include <stdexcept>

namespace Sdl
{
    class Basic_loop {
    public:
        using Function = std::function<void(void)>;
        
        explicit Basic_loop(Function) noexcept;
        void start();
        
    private:
        Function m_f { };
    };
    
    enum class Event_result : bool {
        Quit, Continue
    };
    
    class Event_loop {
    public:
        using Event_function = std::function<Event_result(const SDL_Event&)>;
        using Unconditional_function = std::function<void(void)>;
        
        Event_loop(Event_function, Unconditional_function) noexcept;
        void start();
        
    private:
        Event_function m_event_f { };
        Unconditional_function m_unconditional_f { };
        SDL_Event m_event { };
    };
    
    // These loops can share a common concept in c++17
    
    // TODO maybe an option to change the timers delay?
    class Timer {
    public:
        explicit Timer(std::chrono::milliseconds) noexcept;
        /*
         * The user is expected to give a sane value, larger than 0,
         * not overflowing Uint32. Because small values are pretty much expected,
         * none of this is actually checked, so beware or check it yourself.
         */
        
        bool ready() noexcept;
        
        std::chrono::milliseconds get_delay() const noexcept;
        void set_delay(std::chrono::milliseconds) noexcept;
        
    private:
        Uint32 delay_ { };
        Uint32 current_ { SDL_GetTicks() };
    };
}

#endif
