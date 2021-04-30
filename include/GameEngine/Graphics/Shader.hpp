#pragma once

#include "../Core/Core.hpp"
#include "../Util/Array.hpp"
#include <string_view>

// TODO: ShaderLibrary (just copy from Hazel :| )

namespace GameEngine {
	template<typename T> using Ref = std::shared_ptr<T>;

    class ShaderUnit;

    class GEAPI Shader {
    public:
        static Ref<Shader> Create(const char* name, const char* vertFilePath, const char* fragFilePath, bool addToLibrary = true);
        static Ref<Shader> CreateFromUnits(const char* name, const ArrayConstView<Ref<ShaderUnit>>& shaderUnitsArray, bool addToLibrary = true, bool releaseShaderAfterUse = true);
        static Ref<Shader> Get(const char* name);
        inline const int GetID() const { return id; }
        inline const char* GetName() const { return name; }
        virtual void Use() const = 0;
        virtual void Unuse() const = 0;
        virtual ~Shader() = default;

        virtual void SetInt(const char* name, int value) = 0;
		virtual void SetIntArray(const char* name, const Array<int>&) = 0;
		virtual void SetFloat(const char* name, float value) = 0;
        virtual void SetFloat2(const char* name, const M::VecF<2>& value) = 0;
		virtual void SetFloat3(const char* name, const M::VecF<3>& value) = 0;
		virtual void SetFloat4(const char* name, const M::VecF<4>& value) = 0;
		virtual void SetMat4(const char* name, const M::MatrixF<4,4>& value) = 0;

    protected:
        Shader() = default;
        int id;
        const char* name;
    };

    class GEAPI ShaderUnit {
    public:
        enum ShaderType { NONE, VERTEX, FRAGMENT/* TODO: , GEOMETRY, COMPUTE, TESSELLATION*/ };
        static Ref<ShaderUnit> Create(const char* path, ShaderType t = ShaderType::NONE);
        static Ref<ShaderUnit> CreateFromCode(const std::string_view& str, ShaderType t = ShaderType::NONE);
        virtual void Attach(const Shader&) const = 0;
        virtual void Release() = 0;
		
        virtual ShaderType GetShaderType() const = 0;
        inline const int& GetID() const { return id; }

        virtual ~ShaderUnit() = default;
    protected:
        ShaderUnit() = default;
        int id;
    };

    using ShaderType = ShaderUnit::ShaderType;
}