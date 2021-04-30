#include <GameEngine/Core/App.hpp>
#include <GameEngine/Graphics/BufferSpec.hpp>
#include "platform/GLBufferSpec.hpp"

namespace GameEngine {
    Ref<BufferSpec> BufferSpec::Create(const Array<BufferConfig>& bc, int indexBufSize) {
        #ifdef RENDERER_OPENGL  
        if (App::get()->GetAppConfig().gcConfig.apiType == GraphicsApiType::OPENGL) { 
            return Ref<BufferSpec>(new GLBufferSpec(bc, indexBufSize));
        }
        #endif
        return nullptr;
    }

    Ref<BufferSpec> BufferSpec::Create(const ArrayConstView<Ref<Buffer>>& bc, Ref<Buffer> _indexBuffer) {
        #ifdef RENDERER_OPENGL  
        if (App::get()->GetAppConfig().gcConfig.apiType == GraphicsApiType::OPENGL) { 
            return Ref<BufferSpec>(new GLBufferSpec(bc, _indexBuffer));
        }
        #endif
        return nullptr;
    }

    BufferSpec::BufferSpec(const Array<BufferConfig>& bc, int indexBufSize) : cfg(bc){
        if (indexBufSize >= 0) indexBuffer = Ref<Buffer>(Buffer::Create<INDEXBUFFER, Int>(nullptr, indexBufSize, {}));
        buffers = Array<Ref<Buffer>>(bc.size());
        int i=0;
        for (BufferConfig& c : cfg) {
            buffers[i++] = Ref<Buffer>(Buffer::Create(c.bt, c.dt, nullptr, c.n, c.bl));
        }
    }

    BufferSpec::BufferSpec(const ArrayConstView<Ref<Buffer>>& bc, Ref<Buffer> _indexBuffer) : buffers(bc), indexBuffer(_indexBuffer) {
        cfg = Array<BufferConfig>(buffers.size());
        int i=0;
        for (Ref<Buffer>& b : buffers) {
            cfg[i].bt = b->GetBufferType();
            cfg[i].dt = b->GetDataype();
            cfg[i].n = b->GetSize();
            cfg[i++].bl = b->GetLayout();
        }
    }
}