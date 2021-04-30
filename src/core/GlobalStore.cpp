

//////////      TODO: DELETE THIS    ///////////


#include <memory>
#include "GameEngine/Core/App.hpp"
#include "GameEngine/Core/Log.hpp"
#include "EventHandler.hpp"
#include <map>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using XWindow = Window;

namespace GameEngine {

int userLogLevelFlags = LogLevel::Trace | LogLevel::Debug | LogLevel::Info | LogLevel::Warn | LogLevel::Error | LogLevel::Fatal;

Display* xdisplay;
Atom wmDeleteWindow;
std::vector<GameEngine::Window*> windows;
std::map<XWindow, GameEngine::Window*> windowsMap;
std::vector<Runnable *> runOnUiThread;

#ifndef NO_EXTERNAL_LOGGER
#ifndef DISABLE_USER_LOGGING
std::shared_ptr<spdlog::logger> ge_logger_user;
#endif
#ifndef DISABLE_USER_LOGGING
std::shared_ptr<spdlog::logger> ge_logger_core = any(spdlog::stdout_color_mt("GameEngine"))._apply_(it, it->set_level(spdlog::level::trace));
#endif
#endif

std::unique_ptr<App> appInstance;

std::unique_ptr<EventHandler> evh = std::make_unique<EventHandler>();

}