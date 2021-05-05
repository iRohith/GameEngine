#pragma once

/**
 * Few parts taken from Buffer.h of Project: 'Hazel Engine' - by 'The Cherno', with modifications.
 * https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Renderer/Buffer.h
 * License(Apache License 2.0): https://github.com/TheCherno/Hazel/blob/master/LICENSE
 */

#include "../Core/Core.hpp"
#include "../Core/MathTypes.hpp"
#include "../Util/Array.hpp"
#include <stdint.h>

namespace GameEngine {

    enum DataType { Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, UNKNOWN_DT };
    enum BufferType { VERTEXBUFFER, INDEXBUFFER, CONSTANTBUFFER, UNKNOWN_BT };

    static constexpr uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::Float:    return 4;
			case DataType::Float2:   return 4 * 2;
			case DataType::Float3:   return 4 * 3;
			case DataType::Float4:   return 4 * 4;
			case DataType::Mat3:     return 4 * 3 * 3;
			case DataType::Mat4:     return 4 * 4 * 4;
			case DataType::Int:      return 4;
			case DataType::Int2:     return 4 * 2;
			case DataType::Int3:     return 4 * 3;
			case DataType::Int4:     return 4 * 4;
			case DataType::Bool:     return 1;
		}
		return 0;
	}

    static constexpr uint32_t DataTypeComponentCount(DataType type)
	{
		switch (type)
		{
			case DataType::Float:   return 1;
			case DataType::Float2:  return 2;
			case DataType::Float3:  return 3;
			case DataType::Float4:  return 4;
			case DataType::Mat3:    return 3; // 3* float3
			case DataType::Mat4:    return 4; // 4* float4
			case DataType::Int:     return 1;
			case DataType::Int2:    return 2;
			case DataType::Int3:    return 3;
			case DataType::Int4:    return 4;
			case DataType::Bool:    return 1;
		}
		return 0;
	}

    template<DataType> struct DataTypeToNative;
    //template<> struct DataTypeToNative<UByte>  { using type = uint8_t; };
    template<> struct DataTypeToNative<Float>  { using type = float; };
    template<> struct DataTypeToNative<Float2> { using type = M::VecF<2>; };
    template<> struct DataTypeToNative<Float3> { using type = M::VecF<3>; };
    template<> struct DataTypeToNative<Float4> { using type = M::VecF<4>; };
    template<> struct DataTypeToNative<Mat3> { using type = M::MatrixF<3>; };
    template<> struct DataTypeToNative<Mat4> { using type = M::MatrixF<4>; };
    template<> struct DataTypeToNative<Int>  { using type = int32_t; };
    template<> struct DataTypeToNative<Int2> { using type = M::VecI<2>; };
    template<> struct DataTypeToNative<Int3> { using type = M::VecI<3>; };
    template<> struct DataTypeToNative<Int4> { using type = M::VecI<4>; };
    template<> struct DataTypeToNative<Bool> { using type = bool; };

    template<typename T> struct NativeToDataType;
    //template<> struct NativeToDataType<uint8_t>  { const DataType value = UByte; };
    template<> struct NativeToDataType<float>  { const DataType value = Float; };
    template<> struct NativeToDataType<M::VecF<2>>  { const DataType value = Float2; };
    template<> struct NativeToDataType<M::VecF<3>>  { const DataType value = Float3; };
    template<> struct NativeToDataType<M::VecF<4>>  { const DataType value = Float4; };
    template<> struct NativeToDataType<M::Matrix<3>>  { const DataType value = Mat3; };
    template<> struct NativeToDataType<M::Matrix<4>>  { const DataType value = Mat4; };
    template<> struct NativeToDataType<uint32_t>  { const DataType value = Int; };
    template<> struct NativeToDataType<M::VecI<2>>  { const DataType value = Int2; };
    template<> struct NativeToDataType<M::VecI<3>>  { const DataType value = Int3; };
    template<> struct NativeToDataType<M::VecI<4>>  { const DataType value = Int4; };
    template<> struct NativeToDataType<bool>  { const DataType value = Bool; };

    struct BufferElement
	{
		const char* Name;
		DataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;
		constexpr BufferElement(DataType type, const char* name, bool normalized = false)
			: Name(name), Type(type), Size(DataTypeSize(type)), Offset(0), Normalized(normalized) {}

		inline constexpr uint32_t GetComponentCount() const { return DataTypeComponentCount(Type); }
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			//m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				//m_Stride += element.Size;
			}
			m_Stride = offset;
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
    
    class GEAPI Buffer {
    public:
        static Ref<Buffer> Create(BufferType, DataType, void* values, size_t size, const BufferLayout&);
        
        template<BufferType BT = VERTEXBUFFER, DataType DT = Float>
        static Ref<Buffer> Create(typename DataTypeToNative<DT>::type* values, size_t size, const BufferLayout&);

        template<BufferType BT = VERTEXBUFFER, DataType DT = Float>
        static inline constexpr Ref<Buffer> Create(const ArrayConstView<typename DataTypeToNative<DT>::type>& arr, const BufferLayout& bl){
            return Create(arr.data(), arr.size(), bl);
        }

		virtual ~Buffer() = default;
        virtual const BufferType GetBufferType() const { return UNKNOWN_BT; }
        virtual const DataType GetDataype() const { return UNKNOWN_DT; }
		
        virtual void SetData(const void* _data, size_t _size) = 0;
		virtual void SetSubData(const void* _data, size_t offset, size_t _size) = 0;
        virtual void* GetData() const = 0;
        inline constexpr size_t GetSize() const { return size; }
        inline constexpr uint32_t GetID() const { return id; }
		inline constexpr const BufferLayout& GetLayout() const { return layout; }

        template<DataType DT = Float>
        inline constexpr void SetData(const ArrayConstView<typename DataTypeToNative<DT>::type>& _data) 
            { SetData(_data.data(), _data.size()); }

		template<DataType DT = Float>
        inline constexpr void SetSubData(typename DataTypeToNative<DT>::type* _data, size_t offsetInBuffer, size_t count) 
            { SetSubData((const void*)_data, offsetInBuffer, count); }
        
        template<DataType DT = Float>
        inline constexpr auto GetData() const
            { return Array<typename DataTypeToNative<DT>::type>((typename DataTypeToNative<DT>::type*)GetData(), GetSize(), false); }
        
    protected:
        uint32_t id, size;
		BufferLayout layout;
    };

	template<DataType DT = Float> class GEAPI BufferMap {
	public:
		using T = typename DataTypeToNative<DT>::type;
		static std::unique_ptr<BufferMap> Create(const Buffer&);
		virtual ~BufferMap() = default;
		inline void SetSubData(const ArrayConstView<T>& d, size_t offset){
			std::copy(d.begin(), d.end(), data.begin() + offset);
		}

		inline auto get() { return data; }
		inline operator ArrayView<T>&(){ return data; }
		inline T& operator[](const size_t& idx) { return data[idx]; }
		inline T& operator[](const size_t& idx) const { return data[idx]; }
	protected:
		ArrayView<T> data;
	};
}