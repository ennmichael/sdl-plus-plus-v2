#ifndef SDLPP__FLOW__HEADER_
#define SDLPP__FLOW__HEADER_

#include "drawing.h"
#include "boost/variant.hpp"
#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <atomic>

#include <iostream> // TEST

namespace Sdl {
bool key_down(SDL_Scancode);

// Mouse currently only catches left mouse button, 
// this should be extended
// TODO test this class
class Mouse {
public:
    struct Up {};
    struct Down {};
    struct Just_pressed {};
    struct Just_released {};
    
    using State = boost::variant<Up, 
                                 Down, 
                                 Just_pressed, 
                                 Just_released>;
    
    void update(const SDL_Event&);
    Sdl::Point position() const noexcept;
    
    template <class Visitor>
    typename Visitor::result_type visit_state(Visitor v) {
        const auto temp = state_; 
                            // Forbid mutation, results in a compiler error
                            // if the visitor tries to take a non-const reference
        advance_just_pressed();
        advance_just_released();
        return boost::apply_visitor(v, temp); 
    }
    
private:
    void advance_just_pressed() noexcept;
    void advance_just_released() noexcept;
    void update_position(const SDL_Event&) noexcept;
    void update_state(const SDL_Event&) noexcept;
    
    template <class Select, class New>
    void replace_in_state() {
        if (boost::get<Select>(&state_)) {
            state_ = New{};
        }
    }
    
    template <class Avoid, class New, class Integral>
    void update_state_impl(
        const SDL_Event& event, 
        Integral sdl_macro_value) {
        if (event.type == sdl_macro_value &&
            event.button.button == SDL_BUTTON_LEFT &&
            !boost::get<Avoid>(&state_)) {
            state_ = New{};
        }
    }
    
    State state_{Up{}};
    Sdl::Point pos_{};
};

class Basic_loop {
   public:
    using Function = std::function<void(void)>;

    explicit Basic_loop(Function) noexcept;
    void start();

   private:
    Function m_f{};
};

enum class Event_result : bool { Quit, Continue };

class Event_loop {
   public:
    using Event_function = std::function<Event_result(const SDL_Event&)>;
    using Unconditional_function = std::function<void()>;

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
    using Thread_function = std::function<void(const Quit_flag&)>;
    using Graphics_init_function = std::function<Screen()>;
    using Graphics_thread_function = std::function<void(Screen&)>;

    // add_graphics_thread and add_thread are two separate functions 
    // because SDL_PumplEvents should only be called on the thread that
    // sets the video mode.
    void add_graphics_thread(Graphics_init_function, 
                             Graphics_thread_function);
    void add_thread(Thread_function);
    void start_all();

   private:
    Quit_flag quit_{false};
    std::vector<std::thread> threads_{};
};

// These loops can share a common concept in c++17

class Timer {
   public:
    explicit Timer(std::chrono::milliseconds) noexcept;
    // The user is expected to give a sane value, larger than 0,
    // not overflowing Uint32. Because small values are pretty much expected,
    // none of this is actually checked, so beware or check it yourself.

    bool ready() noexcept;

    std::chrono::milliseconds get_delay() const noexcept;
    void set_delay(std::chrono::milliseconds) noexcept;

   private:
    Uint32 delay_{};
    Uint32 current_{SDL_GetTicks()};
};
}

#endif
