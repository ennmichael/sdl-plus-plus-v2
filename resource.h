#ifndef _SDLPP__RESOURCE__HEADER__
#define _SDLPP__RESOURCE__HEADER__

#include "SDL2/SDL.h"
#include <stdexcept>
#include <memory>

namespace Sdl {
class Sdl_exception : public std::exception {
   public:
    const char* what() const noexcept override;
};

class Sdl_system_control {
   public:
    Sdl_system_control(Uint32 = SDL_INIT_EVERYTHING);
    ~Sdl_system_control() noexcept;

    // No copying or moving allowed
    Sdl_system_control(const Sdl_system_control&) = delete;
    Sdl_system_control(Sdl_system_control&&) = delete;
    Sdl_system_control& operator=(const Sdl_system_control&) = delete;
    Sdl_system_control& operator=(Sdl_system_control&&) = delete;
};

struct Window_deleter;
struct Renderer_deleter;
struct Surface_deleter;
struct Texture_deleter;

using Unique_window = std::unique_ptr<SDL_Window, Window_deleter>;
using Unique_renderer = std::unique_ptr<SDL_Renderer, Renderer_deleter>;
using Unique_surface = std::unique_ptr<SDL_Surface, Surface_deleter>;
using Shared_texture = std::shared_ptr<SDL_Texture>;

Shared_texture load_texture(const std::string&, Unique_renderer&);

// Exception checking
template <class T>
void check_pointer(const T&);
void check_function(int);

struct Window_deleter {
    void operator()(SDL_Window*) const noexcept;
};

struct Renderer_deleter {
    void operator()(SDL_Renderer*) const noexcept;
};

struct Surface_deleter {
    void operator()(SDL_Surface*) const noexcept;
};

struct Texture_deleter {
    void operator()(SDL_Texture*) const noexcept;
};

template <class T>
void check_pointer(const T& t_ptr) {
    if (t_ptr == nullptr)
        throw Sdl_exception{};
}
}

#endif
