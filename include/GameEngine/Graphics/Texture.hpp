#pragma once

#include "../Core/Core.hpp"
#include "../Core/MathTypes.hpp"

namespace GameEngine {
    
    namespace TextureEnums {
        enum WrapMode { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
        enum FilterMode { NEAREST, LINEAR, NEAREST_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR};
    }
    
    struct GEAPI Texture2DConfig {
        int width;
        int height;
        int channels = 3;
        bool createMipMaps = true;
        TextureEnums::FilterMode upscaleFilterMode = TextureEnums::LINEAR;
        TextureEnums::FilterMode downscaleFilterMode = TextureEnums::NEAREST;
        TextureEnums::WrapMode wrapMode[2] = { TextureEnums::REPEAT, TextureEnums::REPEAT};
        bool storeInMemory = false; // TODO: Implement to avoid repeated disk access in case of image files when creating many texture slots
    };

    // TODO: Remove slots / Automatic slot management
    class GEAPI Texture2D {
    public:
        static Ref<Texture2D> Create(const Texture2DConfig&);
        static Ref<Texture2D> FromFile(const char*);

        virtual ~Texture2D() = default;

        virtual void SetData(void* data) = 0;   // Length of data expected to be (width*height*channels) bytes
        virtual void Bind(uint32_t slot = 1) const = 0;
		
        inline const uint32_t GetID() const { return id; }
        inline const Texture2DConfig& GetConfig() const { return texConfig; }
    protected:
        uint32_t id;
        Texture2DConfig texConfig;
    };
}