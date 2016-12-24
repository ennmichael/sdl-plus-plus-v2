#include "drawing.h"
#include <cmath>
#include <iostream>

namespace Sdl
{
    // TODO fix passing unique pointer references around,
    // start passing actual references to the required resources
    
    SDL_Rect make_rect(
        Sdl::Point upper_left, 
        Sdl::Point lower_right
    ) noexcept
    {
        return { 
            upper_left.x, upper_left.y,
            lower_right.x - upper_left.x, 
            lower_right.y - upper_left.y
        };
    }
    
    SDL_Rect make_rect(
        Sdl::Point upper_left, 
        int w, int h
    ) noexcept
    {
        return {
            upper_left.x , upper_left.y,
            w, h
        };
    }
    
    Canvas::Canvas(const Screen_properties& properties)
        : window 
          {
              SDL_CreateWindow(
                  properties.title.c_str(),
                  properties.position.x,
                  properties.position.y,
                  properties.width,
                  properties.height,
                  SDL_WINDOW_SHOWN)
          }
        , renderer { SDL_CreateRenderer(window.get(), -1 , 0) }
    {
        check_pointer(window);
        check_pointer(renderer);
    }
    
    Line::Line(Point new_from, Angle_rad rads, int length) noexcept
        : from { new_from }
        ,to
         {
             static_cast<int>(std::sin(rads.value) * length + from.x),
             static_cast<int>(std::cos(rads.value) * length + from.y)
         } // Sdl::Point_vector are integers in the SDL library, so narrowing
    {      // has to happen
    }
    
    Line::Line(Point new_from, Point new_to) noexcept
        : from { new_from }
        , to { new_to }
    {
    }
    
    void Texture_factory::add(const std::string& name, 
                              Unique_renderer& renderer)
    {
        Drawable_texture drawable = {
            load_texture(name.c_str(), renderer)
        };
        
        SDL_QueryTexture(drawable.texture.get(), nullptr, nullptr,
                         &drawable.width, &drawable.height);
        
        m_map[name] = std::move(drawable);
    }
    
    bool Texture_factory::exists(const std::string& name) const noexcept
    {
        return m_map.count(name) != 0;
    }
    
    Drawable_texture
    Texture_factory::get(const std::string& name) const noexcept
    {
        return m_map.at(name);
    }
    
    Screen::Screen(const Screen_properties& properties)
        : m_canvas { properties }
    {
        // TODO should this be here?
        SDL_SetRenderDrawBlendMode(m_canvas.renderer.get(), 
                                   SDL_BLENDMODE_BLEND);
    }
    
    void Screen::add_draw(const std::string& name, Point where)
    {
        if (!m_factory.exists(name))
            m_factory.add(name, m_canvas.renderer);
        
        m_snapshots.push_back(
            [this, name, where]
            {
                auto drawable = m_factory.get(name);
                
                SDL_Rect src { };
                SDL_Rect dst { };
                
                src.w = dst.w = drawable.width;
                src.h = dst.h = drawable.height;
                
                dst.x = where.x;
                dst.y = where.y;
                
                check_function(
                    SDL_RenderCopy(m_canvas.renderer.get(), 
                                drawable.texture.get(),
                                &src, &dst)
                );
            }
        );
    }
    
    void Screen::add_draw(Line line, SDL_Color color)
    {
        m_snapshots.push_back(
            [this, line, color]
            {
                set_renderer_color(color);
            
                check_function(
                    SDL_RenderDrawLine(
                        m_canvas.renderer.get(),
                        line.from.x, line.from.y, 
                        line.to.x, line.to.y
                    )
                );
            }
        );
    }
    
    void Screen::add_draw(SDL_Rect rect, 
                          SDL_Color color,
                          Color_filling filling)
    {
        m_snapshots.push_back(
            [this, rect, color, filling]
            {
                set_renderer_color(color);
                
                check_function(
                    SDL_RenderDrawRect(m_canvas.renderer.get(),
                                       &rect)
                );
                
                // do check_function
                if (filling == Color_filling::Filled)
                {
                    SDL_RenderFillRect(m_canvas.renderer.get(), 
                                       &rect);
                }
            }
        );
    }
    
    void Screen::redraw(SDL_Color color)
    {
        set_renderer_color(color);
        check_function(
            SDL_RenderClear(m_canvas.renderer.get())
        );
        
        for (const auto& snapshot : m_snapshots)
            snapshot();
        
        SDL_RenderPresent(m_canvas.renderer.get());
        m_snapshots.clear();
    }
    
    void Screen::set_renderer_color(SDL_Color color)
    {
        check_function(
            SDL_SetRenderDrawColor(m_canvas.renderer.get(),
                                   color.r, color.g, color.b, color.a)
        );
    }
}
