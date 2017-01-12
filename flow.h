#ifndef _SDLPP__FLOW__HEADER__
#define _SDLPP__FLOW__HEADER__

#include "drawing.h"
#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <atomic>

namespace Sdl {
bool key_down(SDL_Scancode);

class Basic_loop {
   public:
    using Function = std::function<void(void)>;

    explicit Basic_loop(Function) noexcept;
    void start();

   private:
    Function m_f{};
};

enum class Event_result : bool { Quit, Continue };

// Maybe add a Pump_event_loop class?

class Event_loop {
   public:
    using Event_function = std::function<Event_result(const SDL_Event&)>;
    using Unconditional_function = std::function<void(void)>;

    Event_loop(Event_function, Unconditional_function) noexcept;
    void start();

   private:
    Event_function m_event_f{};
    Unconditional_function m_unconditional_f{};
    SDL_Event m_event{};
};

class Thread_manager {
   public:
    using Quit_flag = std::atomic<bool>;
    using Thread_function = std::function<void()>;
    using Graphics_init_function = std::function<Screen()>;
    using Graphics_thread_function = std::function<void(Screen&)>;

    void add_graphics_thread(Graphics_init_function,
                             Graphics_thread_function f);
    void add_thread(Thread_function f);
    void start_all();

   private:
    Quit_flag quit_{false};
    std::vector<std::thread> threads_{};
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
    Uint32 delay_{};
    Uint32 current_{SDL_GetTicks()};
};
}

#endif
