#ifndef _SDLPP__FLOW__HEADER__
#define _SDLPP__FLOW__HEADER__

#include <functional>

namespace Sdl
{
    class Main_loop { // TODO make this class able to handle events.
      // I don't need that right now, so I won't do it yet.
      // But do it once you need it, and be careful not to 
      // make it too monolithic.
    public:
        using Function = std::function<void(void)>;
        
        explicit Main_loop(const Function&) noexcept;
        void start();
        
    private:
        Function m_f;
    };
}

#endif
