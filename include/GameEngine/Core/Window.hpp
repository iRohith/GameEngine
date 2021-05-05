#pragma once

#include "Core.hpp"
#include "../Graphics/Renderer.hpp"
#include <stdint.h>

namespace GameEngine {
    struct GEAPI WindowConfig {
        const char* Title = "MyWindow";
        uint32_t Width = 620;
        uint32_t Height = 480;
        bool Fullscreen = false;
        inline constexpr const M::VecI<2> size() const { return {Width, Height}; }
        inline constexpr const float aspect() const { return (float)Width/Height; }
    };

    class GEAPI Window {
    public:
        static Window* Create(const WindowConfig& config = {});
        ~Window();

        void Close();
        
        inline void* GetNativeHandle() const { return hndl; }
        inline Renderer* GetRenderer() { return renderer; }
        inline const WindowConfig& GetConfig() const { return cfg; }
        void UpdateConfig(const WindowConfig& config);
        
        inline void SetFullscreen(bool fullscreen){
            WindowConfig ncfg = cfg;
            ncfg.Fullscreen = fullscreen;
            UpdateConfig(ncfg);
        }
        inline void ToggleFullscreen(){ SetFullscreen(!cfg.Fullscreen); }
        
        inline void Resize(uint32_t width = 0, uint32_t height = 0) {
            WindowConfig ncfg = cfg;
            if (width) ncfg.Width = width;
            if (height) ncfg.Height = height;
            UpdateConfig(ncfg);
        }
    private:
        Window(const WindowConfig& config = {});
        Window(const Window&) = delete;
        
        void* hndl;
        WindowConfig cfg;
        Renderer* renderer;
    };
}