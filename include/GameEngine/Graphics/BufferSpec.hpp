#pragma once

#include "Buffer.hpp"
#include <tuple>

namespace GameEngine {

    struct GEAPI BufferConfig {
        BufferType bt;
        DataType dt;
        BufferLayout bl;
        size_t n;
    };

    class GEAPI BufferSpec {
    public:
        static Ref<BufferSpec> Create(const Array<BufferConfig>&, int indexBufSize = -1);
        static Ref<BufferSpec> Create(const ArrayConstView<Ref<Buffer>>&, Ref<Buffer> indexBuffer = nullptr);
        
        BufferSpec(const Array<BufferConfig>&, int indexBufSize = -1);
        BufferSpec(const ArrayConstView<Ref<Buffer>>&, Ref<Buffer> indexBuffer = nullptr);
        virtual ~BufferSpec(){}

        inline const Array<Ref<Buffer>>& GetBuffers() { return buffers; }
        inline const Ref<Buffer>& GetIndexBuffer() { return indexBuffer; }
        inline bool HasIndexBuffer() const { return indexBuffer.use_count(); }
        
        inline const uint32_t GetID() const { return id; }
        inline const ArrayConstView<BufferConfig> GetCFG(){ return cfg; }

        int32_t vertexCount = 0;
    protected:
        uint32_t id;
        Array<BufferConfig> cfg;
        Array<Ref<Buffer>> buffers;
        Ref<Buffer> indexBuffer;
    };
}