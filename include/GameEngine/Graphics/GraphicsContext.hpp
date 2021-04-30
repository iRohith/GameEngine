#pragma once

#include "../Core/Window.hpp"

namespace GameEngine {
    
    enum GEAPI GraphicsApiType { OPENGL, DIRECTX11 };

    struct GEAPI GraphicsConfig {
        GraphicsApiType apiType = GraphicsApiType::OPENGL;
        bool doubleBuffered = true;
        bool vsync = true;
    };

    class GEAPI GraphicsContext {
    public:
        
        static GraphicsContext* Create(const GraphicsConfig&);
        virtual void SwapBuffers(Window* w = nullptr) = 0;
        
        inline const GraphicsConfig& GetConfig() const { return cfg; }
        inline void* GetNativeHandle() const { return hndl; }

        virtual ~GraphicsContext() = default;
    protected:
        GraphicsContext() = default;
        void* hndl;
        GraphicsConfig cfg;
    };
}