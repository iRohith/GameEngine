#include "EventHandler.hpp"

#ifdef PLATFORM_WINDOWS
#include "platform/Events_win32.hpp"
#else
#include "platform/Events_x11.hpp"
#endif

namespace GameEngine {
    extern std::unique_ptr<EventHandler> evh;
}

EventHandler* EventHandler::Get(){
    return evh.get();
}
