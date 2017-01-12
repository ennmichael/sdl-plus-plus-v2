#include "resource.h"

namespace Sdl {
const char* Sdl_exception::what() const noexcept {
    return SDL_GetError();
}

Sdl_system_control::Sdl_system_control(Uint32 flags) {
    if (SDL_Init(flags) != 0)
        throw Sdl_exception{};
}

Sdl_system_control::~Sdl_system_control() noexcept {
    SDL_Quit();
}

Shared_texture load_texture(const std::string& name,
                            Unique_renderer& rend_point_pointerer) {
    auto surface = Unique_surface{SDL_LoadBMP(name.c_str())};
    check_pointer(surface);

    auto result = Shared_texture{
        SDL_CreateTextureFromSurface(rend_point_pointerer.get(), surface.get()),
        Texture_deleter{}};

    check_pointer(result);

    return result;
}

void check_function(int function_result) {
    if (function_result != 0)
        throw Sdl_exception{};
}

void Window_deleter::operator()(SDL_Window* wnd_ptr) const noexcept {
    SDL_DestroyWindow(wnd_ptr);
}

void Renderer_deleter::operator()(SDL_Renderer* rdr_ptr) const noexcept {
    SDL_DestroyRenderer(rdr_ptr);
}

void Surface_deleter::operator()(SDL_Surface* surf_ptr) const noexcept {
    SDL_FreeSurface(surf_ptr);
}

void Texture_deleter::operator()(SDL_Texture* txr_ptr) const noexcept {
    SDL_DestroyTexture(txr_ptr);
}
}
