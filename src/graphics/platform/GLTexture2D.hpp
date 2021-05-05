#pragma once

#include <GameEngine/Graphics/Texture.hpp>
#include <glad/gl.h>

namespace GameEngine {
    constexpr GLint ToGLWrapMode(TextureEnums::WrapMode m) {
        switch (m) {
            case TextureEnums::REPEAT: return GL_REPEAT;
            case TextureEnums::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
            case TextureEnums::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case TextureEnums::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
            default: return 0;
        }
    }

    constexpr GLint ToGLFilterMode(TextureEnums::FilterMode m) {
        switch (m) {
            case TextureEnums::NEAREST: return GL_NEAREST;
            case TextureEnums::LINEAR: return GL_LINEAR;
            case TextureEnums::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
            case TextureEnums::NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
            case TextureEnums::LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
            case TextureEnums::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
            default: return 0;
        }
    }

    struct GLTexture2D : public Texture2D {

        GLint internalFormat;
        GLenum format;
        
        GLTexture2D(const Texture2DConfig& cfg) {
            texConfig = cfg;
            if (cfg.channels == 3) {
                internalFormat = GL_RGB8;
                format = GL_RGB;
            } else {
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
            }

            glGenTextures(1, &id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLWrapMode(cfg.wrapMode[0]));	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLWrapMode(cfg.wrapMode[1]));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(cfg.downscaleFilterMode));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(cfg.upscaleFilterMode));
        }

        void SetData(void* data) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texConfig.width, texConfig.height, 0, format, GL_UNSIGNED_BYTE, data);
            if (texConfig.createMipMaps) glGenerateMipmap(GL_TEXTURE_2D);
        }

        void Bind(uint32_t slot) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, id);
        }

    };
}