#pragma once

#ifdef DISABLE_ENGINE_LOGGING

    #define GE_LOGT(...)
    #define GE_LOGD(...)
    #define GE_LOGI(...)
    #define GE_LOGW(...)
    #define GE_LOGE(...)
    #define GE_LOGF(...)

    #define GE_ASSERT(...)
    #define GE_ASSERT_FATAL(...)

#else
    #include "GameEngine/Util/Util.hpp"
    #include "GameEngine/Core/Log.hpp"

    extern void _log_fmt_impl_(::GameEngine::LogLevel::LogLevel, const char*);

    template<typename S, typename ...Args>
    inline void _log_fmt_impl(::GameEngine::LogLevel::LogLevel lvl, const S& s, Args&&... args) {
        _log_fmt_impl_(lvl, format_str(s, std::forward<Args>(args)...).c_str());
    }
    template<typename S>
    inline void _log_fmt_impl(::GameEngine::LogLevel::LogLevel lvl, const S& s) {
        _log_fmt_impl_(lvl, format_str("{}", s).c_str());
    }

    #define GE_LOGT(...) _log_fmt_impl(::GameEngine::LogLevel::Trace, __VA_ARGS__)
    #define GE_LOGD(...) _log_fmt_impl(::GameEngine::LogLevel::Debug, __VA_ARGS__)
    #define GE_LOGI(...) _log_fmt_impl(::GameEngine::LogLevel::Info , __VA_ARGS__)
    #define GE_LOGW(...) _log_fmt_impl(::GameEngine::LogLevel::Warn , __VA_ARGS__)
    #define GE_LOGE(...) _log_fmt_impl(::GameEngine::LogLevel::Error, __VA_ARGS__); GE_DEBUGBREAK()
    #define GE_LOGF(...) _log_fmt_impl(::GameEngine::LogLevel::Fatal, __VA_ARGS__); GE_DEBUGBREAK()

    #define GE_ASSERT(condition, ...) if (!(condition)) { _log_fmt_impl(::GameEngine::LogLevel::Error, "Assertion failed: {}", fmt::format(__VA_ARGS__)); GE_DEBUGBREAK(); }
    #define GE_ASSERT_FATAL(condition, ...) if (!(condition)) { _log_fmt_impl(::GameEngine::LogLevel::Fatal, "Critical assertion failed: {}", fmt::format(__VA_ARGS__)); GE_DEBUGBREAK(); abort(); }

#endif
