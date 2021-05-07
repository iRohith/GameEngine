

//////////      TODO: DELETE THIS    ///////////


#include <memory>
#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "EventHandler.hpp"
#include <map>

#ifdef PLATFORM_LINUX
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    using XWindow = Window;
#endif

namespace GameEngine {

int userLogLevelFlags = LogLevel::Trace | LogLevel::Debug | LogLevel::Info | LogLevel::Warn | LogLevel::Error | LogLevel::Fatal;

#ifdef PLATFORM_LINUX
    Display* xdisplay;
    Atom wmDeleteWindow;
    std::map<XWindow, GameEngine::Window*> windowsMap;
#endif

std::vector<GameEngine::Window*> windows;
std::vector<Runnable *> runOnUiThread;

#ifndef NO_EXTERNAL_LOGGER
#ifndef DISABLE_USER_LOGGING
std::shared_ptr<spdlog::logger> ge_logger_user;
#endif
#ifndef DISABLE_USER_LOGGING
std::shared_ptr<spdlog::logger> ge_logger_core = any(spdlog::stdout_color_mt("GameEngine"))._apply_(it, it->set_level(spdlog::level::trace));
#endif
#endif

std::filesystem::path asset_folder_path = std::filesystem::current_path().append("assets");
std::unique_ptr<App> appInstance;

std::unique_ptr<EventHandler> evh = std::make_unique<EventHandler>();

}