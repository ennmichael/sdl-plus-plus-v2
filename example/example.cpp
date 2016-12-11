#include "SDL2/SDL.h"
#include "resource.h"
#include "drawing.h"
#include "flow.h"
#include <iostream>

int main() 
{
    using namespace Sdl;
    
    try
    {
        Control _ { }; // Control the SDL subsystems
        
        Screen_properties props { }; // What our window will look like
        
        props.title = "Clock";
        props.width = 300;
        props.height = 300; // Set up...
        
        Screen scrn { props };
        Angle_rad angle { };
        
        Main_loop ml 
        {
            [&scrn, &angle]()
            {
                Line l { { 150, 150 }, angle, 80 };
                // Line starting at (150, 150) pixels that is 80 
                // pixels long under the angle angle
                
                scrn.add_draw(l, color_red()); // Draw the line l in red
                scrn.add_draw(
                    Sdl::make_rect(l.to, 50, 50), 
                    color_blue(),
                    Color_filling::Filled); 
                // Draw a rectange with that is 10 pixels wide
                // and 10 pixels long at the end of the line, filled with blue
                scrn.add_draw(
                    Sdl::make_rect(l.to, 50, 50), 
                    color_green(),
                    Color_filling::None); 
                // This time, the rectangle is green and has no filling
                
                scrn.redraw(color_white());
                angle.value += 0.0025;
                SDL_Delay(1); // In case someone has a super fast computer
            }
        };
        
        ml.start();
    }
    catch (Sdl_exception& e)
    { // In case something happens to go wrong
        std::cout << "Sdl exception: ";
        std::cout << e.what() << '\n';
    }
}
