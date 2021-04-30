#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Core/App.hpp"
#include "GELog.hpp"

#ifndef DISABLE_USER_LOGGING
    namespace GameEngine {
        extern int userLogLevelFlags;

        void Log::EnableLevel(LogLevel::LogLevel lvl) { userLogLevelFlags |= lvl; }
        void Log::DisableLevel(LogLevel::LogLevel lvl){ userLogLevelFlags &= ~lvl; }
        bool Log::IsLevelEnabled(LogLevel::LogLevel lvl) { return userLogLevelFlags & lvl; }
    }
    #ifdef NO_EXTERNAL_LOGGER
        #include <chrono>

        const char* mName;
        void GameEngine::Log::init(){
            auto app = App::get();
            if (app) {
                mName = app->GetAppConfig().Name;
            } else {
                mName = "Untitled";
                Log::error("No app instance created");
            }
        }

        void GameEngine::Log::log(const char* str, GameEngine::LogLevel::LogLevel lvl) {
            using namespace std::chrono;
            auto now = floor<milliseconds>(system_clock::now());
            auto now_c = system_clock::to_time_t(now);
            #ifdef PLATFORM_WINDOWS
            std::tm now_tm;
            localtime_s(&now_tm, &now_c);
            #else
            auto now_tm = *std::localtime(&now_c);
            #endif
            auto millis = (duration_cast<milliseconds>(now.time_since_epoch()) - duration_cast<seconds>(now.time_since_epoch())).count();
            
            if (userLogLevelFlags & lvl)
                switch (lvl) {
                case LogLevel::Trace: fmt::print("[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Trace] {}\n", now_tm, millis, mName, str); break;
                case LogLevel::Info:  fmt::print("[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [ Info] {}\n", now_tm, millis, mName, str); break;
                case LogLevel::Debug: fmt::print(fg(fmt::color::deep_sky_blue), "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Debug] {}\n", now_tm, millis, mName, str); break;
                case LogLevel::Warn:  fmt::print(fg(fmt::color::orange), "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [ Warn] {}\n", now_tm, millis, mName, str); break;
                case LogLevel::Error: fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Error] {}\n", now_tm, millis, mName, str); break;
                case LogLevel::Fatal: fmt::print(fg(fmt::color::white) | bg(fmt::color::red) | fmt::emphasis::bold, "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Fatal] {}\n", now_tm, millis, mName, str); break;
                default: break;
            }
        }

    #else

        #include <spdlog/spdlog.h>
        #include <spdlog/sinks/stdout_color_sinks.h>

        namespace GameEngine {
            extern std::shared_ptr<spdlog::logger> ge_logger_user;

            void Log::init(){
                auto app = App::get();
                if (app) {
                    ge_logger_user = any(spdlog::stdout_color_mt(app->GetAppConfig().Name))._apply_(it, it->set_level(spdlog::level::trace));
                } else {
                    ge_logger_user = any(spdlog::stdout_color_mt("Untitled"))._apply_(it, it->set_level(spdlog::level::trace));
                    Log::error("No app instance created");
                }
            }

            void Log::log(const char* str, LogLevel::LogLevel lvl) {
                if (userLogLevelFlags & lvl)
                    switch (lvl) {
                    case LogLevel::Trace: ge_logger_user->trace(str); break;
                    case LogLevel::Debug: ge_logger_user->debug(str); break;
                    case LogLevel::Info: ge_logger_user->info(str); break;
                    case LogLevel::Warn: ge_logger_user->warn(str); break;
                    case LogLevel::Error: ge_logger_user->error(str); break;
                    case LogLevel::Fatal: ge_logger_user->critical(str); break;
                    default: break;
                }
            }
        }

    #endif    
#endif

#ifndef DISABLE_ENGINE_LOGGING
    #ifdef NO_EXTERNAL_LOGGER
        #include <chrono>

        void _log_fmt_impl_(GameEngine::LogLevel::LogLevel lvl, const char* str) {
            using namespace std::chrono;
            auto now = floor<milliseconds>(system_clock::now());
            auto now_c = system_clock::to_time_t(now);
                
            #ifdef PLATFORM_WINDOWS
            std::tm now_tm;
            localtime_s(&now_tm, &now_c);
            #else
            auto now_tm = *std::localtime(&now_c);
            #endif
            auto millis = (duration_cast<milliseconds>(now.time_since_epoch()) - duration_cast<seconds>(now.time_since_epoch())).count();
                
            #define mName "GameEngine"
                
            switch (lvl) {
                case GameEngine::LogLevel::Trace: fmt::print("[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Trace] {}\n", now_tm, millis, mName, str); break;
                case GameEngine::LogLevel::Info:  fmt::print("[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [ Info] {}\n", now_tm, millis, mName, str); break;
                case GameEngine::LogLevel::Debug: fmt::print(fg(fmt::color::deep_sky_blue), "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Debug] {}\n", now_tm, millis, mName, str); break;
                case GameEngine::LogLevel::Warn:  fmt::print(fg(fmt::color::orange), "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [ Warn] {}\n", now_tm, millis, mName, str); break;
                case GameEngine::LogLevel::Error: fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Error] {}\n", now_tm, millis, mName, str); break;
                case GameEngine::LogLevel::Fatal: fmt::print(fg(fmt::color::white) | bg(fmt::color::red) | fmt::emphasis::bold, "[{:%d-%m-%y %H:%M:%S}.{:03}] [{}] [Fatal] {}\n", now_tm, millis, mName, str); break;
                default: break;
            }

            #undef mName
        }

    #else

        #include <spdlog/spdlog.h>
        #include <spdlog/sinks/stdout_color_sinks.h>

        extern std::shared_ptr<spdlog::logger> ge_logger_core;
        
        void _log_fmt_impl_(GameEngine::LogLevel::LogLevel lvl, const char* str) {
            switch (lvl) {
                case GameEngine::LogLevel::Trace: ge_logger_core->trace(str); break;
                case GameEngine::LogLevel::Debug: ge_logger_core->debug(str); break;
                case GameEngine::LogLevel::Info : ge_logger_core->info(str); break;
                case GameEngine::LogLevel::Warn : ge_logger_core->warn(str); break;
                case GameEngine::LogLevel::Error: ge_logger_core->error(str); break;
                case GameEngine::LogLevel::Fatal: ge_logger_core->critical(str); break;
                default: break;
            }
        }

    #endif
#endif
