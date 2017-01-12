#include "flow.h"
#include "SDL2/SDL.h"

namespace Sdl {
bool key_down(SDL_Scancode scancode) {
    return SDL_GetKeyboardState(nullptr)[scancode];
}

Basic_loop::Basic_loop(Function f) noexcept : m_f{f} {}

void Basic_loop::start() {
    while (!SDL_QuitRequested()) {
        m_f();
    }
}

Event_loop::Event_loop(Event_function event_f,
                       Unconditional_function unconditional_f) noexcept
    : m_event_f{event_f},
      m_unconditional_f{unconditional_f} {}

void Event_loop::start() {
    while (true) {
        while (SDL_PollEvent(&m_event)) {
            if (m_event_f(m_event) == Event_result::Quit)
                return;  // Quit if the user says so
        }

        m_unconditional_f();
    }
}

void Thread_manager::add_graphics_thread(Graphics_init_function init,
                                         Graphics_thread_function f) {
    threads_.push_back(std::thread{[this, init, f] {
        auto screen = init();

        while (!quit_) {
            SDL_PumpEvents();
            f(screen);
            if (SDL_QuitRequested()) {
                quit_ = true;
            }
        }
    }});
}

void Thread_manager::add_thread(Thread_function f) {
    threads_.push_back(std::thread{[f, this] {
        while (!quit_) {
            f();
        }
    }});
}

void Thread_manager::start_all() {
    for (auto& thread : threads_) {
        thread.join();
    }
}

Timer::Timer(std::chrono::milliseconds delay) noexcept {
    set_delay(delay);
}

bool Timer::ready() noexcept {
    auto ticks = SDL_GetTicks();

    if (ticks >= delay_ + current_) {
        current_ = ticks;
        return true;
    }

    return false;
}

std::chrono::milliseconds Timer::get_delay() const noexcept {
    return std::chrono::milliseconds{delay_};
}

void Timer::set_delay(std::chrono::milliseconds delay) noexcept {
    delay_ = static_cast<Uint32>(delay.count());
}
}
