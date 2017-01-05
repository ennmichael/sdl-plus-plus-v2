#include "flow.h"
#include "resource.h"
#include "drawing.h"

int main(int, char**) {
    using namespace Sdl;
    Sdl_system_control _{};
    
    Screen_properties props{};
    props.title = "Example";
    props.width = props.height = 500;
    
    Screen screen{props};
    Basic_loop ml {
        [&screen] {
            screen.redraw(color_red());
        }
    };
    
    ml.start();
}
