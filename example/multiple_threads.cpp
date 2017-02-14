#include "../flow.h"
#include "../resource.h"
#include "../drawing.h"

using namespace Sdl;

int main(int, char**) {
    Sdl_system_control _{};
    
    Thread_manager threads{};
    threads.add_graphics_thread(
        [] {
            Screen_properties props{};
            props.title = "Example";
            props.width = props.height = 500;
            
            return Screen{props};
        },
        [](Screen& screen) {
            screen.add_draw(
                make_rect({0, 0}, 100, 100), 
                color_black(), 
                Color_filling::Filled);
            screen.add_draw(
                make_rect({105, 105}, {200, 200}), 
                color_blue(), 
                Color_filling::None);
            
            screen.redraw(color_red());
        }
    );
    
    threads.add_thread(
        [] {
            if (key_down(SDL_SCANCODE_RETURN)) {
                Message_content content{};
                content.title = "Info";
                content.text = "Return pressed.";
                
                show_message(content, Message_box_type::Basic);
            }
        }
    );
    
    threads.start_all();
}
