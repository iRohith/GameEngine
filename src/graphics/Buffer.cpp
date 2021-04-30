#include <GameEngine/Core/App.hpp>
#include "platform/GLBuffer.hpp"

using namespace GameEngine;

template<BufferType BT, DataType DT>
Ref<Buffer> create(void* data, size_t size, const BufferLayout& bl){
#ifdef RENDERER_OPENGL  
    if (App::get()->GetAppConfig().gcConfig.apiType == GraphicsApiType::OPENGL) { 
        return Ref<Buffer>(new GLBuffer<BT, DT>(data, size, bl));
    }
#endif
    return nullptr;
}

#define GEN_TEMPLATE_IMPL(BT, DT) \
template<> Ref<Buffer> Buffer::Create<BT,DT>(typename DataTypeToNative<DT>::type* values, size_t size, const BufferLayout& bl){ \
    return create<BT,DT>(values, size, bl); \
}


GEN_TEMPLATE_IMPL(INDEXBUFFER, Int)

GEN_TEMPLATE_IMPL(VERTEXBUFFER, Float)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Float2)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Float3)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Float4)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Mat3)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Mat4)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Int)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Int2)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Int3)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Int4)
GEN_TEMPLATE_IMPL(VERTEXBUFFER, Bool)

GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Float)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Float2)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Float3)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Float4)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Mat3)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Mat4)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Int)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Int2)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Int3)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Int4)
GEN_TEMPLATE_IMPL(CONSTANTBUFFER, Bool)

#undef GEN_TEMPLATE_IMPL

#define RUNTBUFGEN(BT, DT) Buffer::Create<BT, DT>(reinterpret_cast<typename DataTypeToNative<DT>::type*>(values), size, bl)

Ref<Buffer> Buffer::Create(BufferType bt, DataType dt, void* values, size_t size, const BufferLayout& bl){
    switch (bt)
    {
    case INDEXBUFFER: return RUNTBUFGEN(INDEXBUFFER, Int);
    case VERTEXBUFFER: switch (dt)
    {
    case Float:  return RUNTBUFGEN(VERTEXBUFFER, Float);
    case Float2: return RUNTBUFGEN(VERTEXBUFFER, Float2);
    case Float3: return RUNTBUFGEN(VERTEXBUFFER, Float3);
    case Float4: return RUNTBUFGEN(VERTEXBUFFER, Float4);
    case Mat3:   return RUNTBUFGEN(VERTEXBUFFER, Mat3);
    case Mat4:   return RUNTBUFGEN(VERTEXBUFFER, Mat4);
    case Int:    return RUNTBUFGEN(VERTEXBUFFER, Int);
    case Int2:   return RUNTBUFGEN(VERTEXBUFFER, Int2);
    case Int3:   return RUNTBUFGEN(VERTEXBUFFER, Int3);
    case Int4:   return RUNTBUFGEN(VERTEXBUFFER, Int4);
    case Bool:   return RUNTBUFGEN(VERTEXBUFFER, Bool);
    default: return nullptr;
    }
    case CONSTANTBUFFER: switch (dt)
    {
    case Float:  return RUNTBUFGEN(CONSTANTBUFFER, Float);
    case Float2: return RUNTBUFGEN(CONSTANTBUFFER, Float2);
    case Float3: return RUNTBUFGEN(CONSTANTBUFFER, Float3);
    case Float4: return RUNTBUFGEN(CONSTANTBUFFER, Float4);
    case Mat3:   return RUNTBUFGEN(CONSTANTBUFFER, Mat3);
    case Mat4:   return RUNTBUFGEN(CONSTANTBUFFER, Mat4);
    case Int:    return RUNTBUFGEN(CONSTANTBUFFER, Int);
    case Int2:   return RUNTBUFGEN(CONSTANTBUFFER, Int2);
    case Int3:   return RUNTBUFGEN(CONSTANTBUFFER, Int3);
    case Int4:   return RUNTBUFGEN(CONSTANTBUFFER, Int4);
    case Bool:   return RUNTBUFGEN(CONSTANTBUFFER, Bool);
    default: return nullptr;
    }
    default: return nullptr;
    }
}

#undef RUNTBUFGEN