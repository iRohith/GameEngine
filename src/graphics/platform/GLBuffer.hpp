#pragma once

#ifdef RENDERER_OPENGL

#include <GameEngine/Graphics/Buffer.hpp>
#include <glad/gl.h>

namespace GameEngine {

    template<BufferType BT = VERTEXBUFFER, DataType DT = Float>
    struct GLBuffer : public Buffer {
        using type = typename DataTypeToNative<DT>::type;
        const GLenum BUFFER_CONST = BT == INDEXBUFFER ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
        
        GLBuffer(void* _data, size_t _size, const BufferLayout& bl) {
            size = _size;
            layout = bl;
            glGenBuffers(1, &id); 
            glBindBuffer(BUFFER_CONST, id);  
            glBufferData(BUFFER_CONST, size * DataTypeSize(DT), _data, _data ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        }

        void SetData(const void* _data, size_t _size){ 
            glBindBuffer(BUFFER_CONST, id);  
            size = _size;
            glBufferData(BUFFER_CONST, _size * DataTypeSize(DT), _data, _data ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
        }

        void SetSubData(const void* _data, size_t _offset, size_t _count) {
            glBindBuffer(BUFFER_CONST, id);
            glBufferSubData(BUFFER_CONST, _offset * DataTypeSize(DT), _count * DataTypeSize(DT), _data);
        }

        void* GetData() const{
            type* data = new type[size];
            glGetBufferSubData(BUFFER_CONST, 0, size * DataTypeSize(DT), data);
            return data;
        }

        const BufferType GetBufferType() const { return BT; };
        const DataType GetDataype() const { return DT; };
    };

    template<DataType DT = Float> struct GLBufferMap : public BufferMap<DT> {
        using T = typename BufferMap<DT>::T;
        GLenum BUFFER_CONST;
        uint32_t id;

        GLBufferMap(const Buffer& b){
            BUFFER_CONST = b.GetBufferType() == INDEXBUFFER ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
            id = b.GetID();
            glBindBuffer(BUFFER_CONST, id);
            BufferMap<DT>::data = ArrayView<T>((T*)glMapBuffer(BUFFER_CONST,  GL_READ_WRITE), b.GetSize());
        }

        ~GLBufferMap(){
            glBindBuffer(BUFFER_CONST, id); 
            glUnmapBuffer(BUFFER_CONST);
        }
    };
    
}

#endif