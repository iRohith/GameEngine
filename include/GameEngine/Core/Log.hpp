#pragma once

#include "Core.hpp"
#include "../Util/Util.hpp"

namespace GameEngine {
    namespace LogLevel {
        enum LogLevel : unsigned char { None = 0, Trace = 1 << 0, Debug = 1 << 1, Info = 1 << 2, Warn = 1 << 3, Error = 1 << 4, Fatal = 1 << 5 };
    }
}
#define ic inline constexpr

#ifdef DISABLE_USER_LOGGING

    namespace GameEngine {
        class Log {
        public:
            static ic void init(){}
            static ic void EnableLevel(LogLevel::LogLevel){}
            static ic void DisableLevel(LogLevel::LogLevel){}
            static ic bool IsLevelEnabled(LogLevel::LogLevel){ return false; }
            static ic void log(const char*, LogLevel::LogLevel){}

            template<typename S, typename ...Args> static ic void log(LogLevel::LogLevel, const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void info (const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void trace(const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void debug(const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void warn (const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void error(const S&, Args&&...) {}
            template<typename S, typename ...Args> static ic void fatal(const S&, Args&&...) {}
        private:
            Log() = delete;
        };
    }

#else

    namespace GameEngine {

        class GEAPI Log {
        public:
            static void init();
            static void log(const char*, LogLevel::LogLevel);
            static void EnableLevel(LogLevel::LogLevel lvl);
            static void DisableLevel(LogLevel::LogLevel lvl);
            static bool IsLevelEnabled(LogLevel::LogLevel lvl);
            
            template<typename S> static inline void log(LogLevel::LogLevel lvl, const S& v)  {
                if (!IsLevelEnabled(lvl)) return;
                log(format_str("{}", v).c_str(), lvl);
            }
            template<typename S, typename ...Args> static inline void log(LogLevel::LogLevel lvl, const S& str, Args&&... args)  {
                if (!IsLevelEnabled(lvl)) return;
                log(format_str(str, std::forward<Args>(args)...).c_str(), lvl);
            }
            template<typename S, typename ...Args> static inline void info (const S& str, Args&&... args) { log(LogLevel::Info,  str, std::forward<Args>(args)...); }
            template<typename S, typename ...Args> static inline void trace(const S& str, Args&&... args) { log(LogLevel::Trace, str, std::forward<Args>(args)...); }
            template<typename S, typename ...Args> static inline void debug(const S& str, Args&&... args) { log(LogLevel::Debug, str, std::forward<Args>(args)...); }
            template<typename S, typename ...Args> static inline void warn (const S& str, Args&&... args) { log(LogLevel::Warn,  str, std::forward<Args>(args)...); }
            template<typename S, typename ...Args> static inline void error(const S& str, Args&&... args) { log(LogLevel::Error, str, std::forward<Args>(args)...); }
            template<typename S, typename ...Args> static inline void fatal(const S& str, Args&&... args) { log(LogLevel::Fatal, str, std::forward<Args>(args)...); }
        private:
            Log() = delete;
        };
    }

#endif

#undef ic