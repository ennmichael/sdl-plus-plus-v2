#ifndef _SDPP__DRAWING__HEADRER__
#define _SDPP__DRAWING__HEADRER__

#include "SDL2/SDL.h"
#include "resource.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <functional>

namespace Sdl
{
    template <class T>
    struct Basic_point;
    struct Canvas;
    struct Line;
    
    using Point = Basic_point<int>;
    using Precise_point = Basic_point<double>;
    // More aliases like these?
    
    template <class T>
    struct Basic_point { // A 2D point, as simple as they make them
        T x;
        T y;
    };
    
    template <class T1, class T2>
    bool operator==(const Basic_point<T1>&, const Basic_point<T2>&);
    template <class T1, class T2, class TR>
    auto operator+=(Basic_point<T1>& lhs, const Basic_point<T2>& rhs)
        -> Basic_point<decltype(lhs.x + rhs.x)>;
    template <class T1, class T2>
    auto operator+(
        const Basic_point<T1>& lhs, 
        const Basic_point<T2>& rhs) -> Basic_point<decltype(lhs.x + rhs.x)>;
    
    SDL_Color color_red(Uint8 = 255, Uint8 alpha = 255) noexcept;
    SDL_Color color_green(Uint8 = 255, Uint8 alpha = 255) noexcept;
    SDL_Color color_blue(Uint8 = 255, Uint8 alpha = 255) noexcept;
    SDL_Color color_black(Uint8 alpha = 255) noexcept;
    SDL_Color color_white(Uint8 alpha = 255) noexcept;
    
    SDL_Rect make_rect(Sdl::Point, Sdl::Point) noexcept;
    SDL_Rect make_rect(Sdl::Point, int w, int h) noexcept;
    
    struct Screen_properties { // Properties of a window
        int width { 800 };
        int height { 600 };
        Point position { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED };
        std::string title { };
    };
    
    struct Canvas { // Canvas is what you need in order to draw something
      // Should this class even exist?
        Canvas() = default;
        explicit Canvas(const Screen_properties&); 
        // Creates a canvas based on the given window properties
        
        Unique_window window { nullptr };
        Unique_renderer renderer { nullptr };
    };
    
    struct Angle_rad { // Angle in radians
        double value { };
    };
    
    struct Line {
        Line() = default;
        Line(Point, Angle_rad, int) noexcept;
        Line(Point, Point) noexcept;
        
        Point from { };
        Point to { };
    };
    
    struct Drawable_texture {
        Shared_texture texture { nullptr };
        int width { };
        int height { };
    };
    
    class Texture_factory {
    public:
        void add(const std::string&, Unique_renderer&);
        bool exists(const std::string&) const noexcept;
        Drawable_texture get(const std::string&) const noexcept;
        
    private:
        std::map<std::string, Drawable_texture> m_map { };
    };
    
    enum class Color_filling : bool {
        Filled, None
    };
    
    // TODO update this class to use snapshots
    class Screen { // Manipulates a canvas
    public:
        explicit Screen(const Screen_properties&);
        
        void add_draw(const std::string&, Point);
        void add_draw(Line, SDL_Color);
        void add_draw(SDL_Rect, SDL_Color, Color_filling);
        
        void redraw(SDL_Color); // Clear, draw, render present
        
    private:
        using Snapshot = std::function<void(void)>;
        
        void set_renderer_color(SDL_Color);
        
        Canvas m_canvas { };
        Texture_factory m_factory { };
        // Stuff to draw
        std::vector<Snapshot> m_snapshots { };
    };
    
    // Basic_point operator definitions
    
    template <class T1, class T2>
    bool operator==(const Basic_point<T1>& lhs, const Basic_point<T2>& rhs)
    {
        return lhs.x == rhs.x &&
               lhs.y == rhs.y;
    }
    
    template <class T1, class T2>
    auto operator+=(Basic_point<T1>& lhs, const Basic_point<T2>& rhs)
        -> Basic_point<decltype(lhs.x + rhs.x)>
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }
        
    template <class T1, class T2>
    auto operator+(
        const Basic_point<T1>& lhs, 
        const Basic_point<T2>& rhs) -> Basic_point<decltype(lhs.x + rhs.x)>
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }
}

#endif