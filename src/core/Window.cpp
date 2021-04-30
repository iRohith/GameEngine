#include <GameEngine/Core/Window.hpp>
#include <GameEngine/Graphics/GraphicsContext.hpp>
#include <vector>

namespace GameEngine {
    extern std::vector<Window*> windows;
    extern Renderer* CreateRenderer(GraphicsApiType);
}

#ifdef PLATFORM_WINDOWS
#include "platform/Window_win32.hpp"
#else
#include "platform/Window_x11.hpp"
#endif

namespace GameEngine {
    Window* _GetDefaultWindow(){
        return windows.empty() ? nullptr : windows[0];
    }

    Window* _GetWindow(const char* str) {
        for (auto&& w : windows) if (strcmp(w->GetConfig().Title, str) == 0) return w;
        return nullptr;
    }

    void _CloseAllWindows(){
        for (auto&& w : windows) w->Close();
    }

}