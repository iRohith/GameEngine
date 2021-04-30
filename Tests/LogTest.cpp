#include "GameEngine/Core/Log.hpp"
#include "GameEngine/Util/Array.hpp"
#include <iostream>

int main(){
    using namespace GameEngine;

    Log::init();

    Log::info(123);
    Log::info("OK");
    Log::info("{} {}", format_str("{}", "OK"), 1);
    Log::trace("OK");
    Log::debug("OK");
    Log::warn("OK");
    Log::error("OK");

    Log::DisableLevel(LogLevel::Debug);
    Log::debug("OK");
    Log::EnableLevel(LogLevel::Debug);
    Log::debug("OK");

    //Log::debug("{}", range(10));

    return 0;
}