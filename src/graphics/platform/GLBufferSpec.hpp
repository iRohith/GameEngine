#pragma once

/**
 * Few parts taken from OpenGLVertexArray.cpp of Project: 'Hazel Engine' - by 'The Cherno', with modifications.
 * https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/OpenGL/OpenGLVertexArray.cpp
 * License(Apache License 2.0): https://github.com/TheCherno/Hazel/blob/master/LICENSE
 */

#ifdef RENDERER_OPENGL

#include <GameEngine/Graphics/BufferSpec.hpp>
#include "../../core/GELog.hpp"
#include <glad/gl.h>

namespace GameEngine {
    static GLenum ShaderDataTypeToOpenGLBaseType(DataType type)
	{
		switch (type)
		{
			case DataType::Float:    return GL_FLOAT;
			case DataType::Float2:   return GL_FLOAT;
			case DataType::Float3:   return GL_FLOAT;
			case DataType::Float4:   return GL_FLOAT;
			case DataType::Mat3:     return GL_FLOAT;
			case DataType::Mat4:     return GL_FLOAT;
			case DataType::Int:      return GL_INT;
			case DataType::Int2:     return GL_INT;
			case DataType::Int3:     return GL_INT;
			case DataType::Int4:     return GL_INT;
			case DataType::Bool:     return GL_BOOL;
		}

		GE_LOGE("Unknown DataType!");
		return 0;
	}
    
    struct GLBufferSpec : public BufferSpec {
        int32_t m_VertexBufferIndex;

        GLBufferSpec(const Array<BufferConfig>& bc, int indexBufSize) : BufferSpec(bc, indexBufSize) {
		    glGenVertexArrays(1, &id);
        }

        GLBufferSpec(const ArrayConstView<Ref<Buffer>>& bc, Ref<Buffer> _indexBuffer) : BufferSpec(bc, _indexBuffer) {
		    glGenVertexArrays(1, &id);
        }

        void enableAttributes(){
            glBindVertexArray(id);
            m_VertexBufferIndex = 0;
            
            for (int j=0; j<buffers.size(); ++j) {
                glBindBuffer(GL_ARRAY_BUFFER, buffers[j]->GetID());
                const auto& layout = buffers[j]->GetLayout();

                for (const auto& element : layout)
                {
                    switch (element.Type)
                    {
                        case DataType::Float:
                        case DataType::Float2:
                        case DataType::Float3:
                        case DataType::Float4:
                        case DataType::Int:
                        case DataType::Int2:
                        case DataType::Int3:
                        case DataType::Int4:
                        case DataType::Bool:
                        {
                            glEnableVertexAttribArray(m_VertexBufferIndex);
                            glVertexAttribPointer(m_VertexBufferIndex,
                                element.GetComponentCount(),
                                ShaderDataTypeToOpenGLBaseType(element.Type),
                                element.Normalized ? GL_TRUE : GL_FALSE,
                                layout.GetStride(),
                                (const void*)element.Offset);
                            m_VertexBufferIndex++;
                            break;
                        }
                        case DataType::Mat3:
                        case DataType::Mat4:
                        {
                            uint8_t count = element.GetComponentCount();
                            for (uint8_t i = 0; i < count; i++)
                            {
                                glEnableVertexAttribArray(m_VertexBufferIndex);
                                glVertexAttribPointer(m_VertexBufferIndex,
                                    count,
                                    ShaderDataTypeToOpenGLBaseType(element.Type),
                                    element.Normalized ? GL_TRUE : GL_FALSE,
                                    layout.GetStride(),
                                    (const void*)(element.Offset + sizeof(float) * count * i));
                                glVertexAttribDivisor(m_VertexBufferIndex, 1);
                                m_VertexBufferIndex++;
                            }
                            break;
                        }
                        default:
                            GE_LOGE("Unknown DataType!");
                    }
                }

            }
            if (HasIndexBuffer()) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetID());
        }

        void disableAttributes(){
            glBindVertexArray(id);
            while (m_VertexBufferIndex--) glDisableVertexAttribArray(m_VertexBufferIndex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        ~GLBufferSpec(){
            glDeleteVertexArrays(1, &id);
        }
    };
}

#endif