#pragma once

#include <GameEngine/Graphics/Shader.hpp>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/gl.h>

namespace GameEngine {

bool read_file(const char* path, std::string& out){
    try {
        std::ifstream t(path, std::ios::in | std::ios::binary);
        t.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        out.clear();
        out.resize(size);
        t.seekg(0, std::ios::beg);
        t.read(&out[0], size);
        return true;
    } catch (std::exception e) {
        GE_LOGE("File read failed: path={}, msg: {}", path, e.what());
        return false;
    }
}

unsigned int compile_vert(const char* vShaderCode){
    unsigned int vertex;
    int success;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        GE_LOGE("Vertex shader compile failed: {}", infoLog);
        return UINT32_MAX;
    };
    return vertex;
}

unsigned int compile_frag(const char* fShaderCode){
    unsigned int fragment;
    int success;
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        GE_LOGE("Fragment shader compile failed: {}", infoLog);
        return UINT32_MAX;
    };
    return fragment;
}

class GLShader : public Shader {
public:
    int status = 0;

    GLShader(const char* _name, const char* vertFilePath, const char* fragFilePath, bool addToLibrary){
        name = _name;
        
        std::string code;
        if (!read_file(vertFilePath, code)){ status = -1; return; }

        auto vertex = compile_vert(code.c_str());
        if (vertex == UINT32_MAX){ status = -1; return; }

        if (!read_file(fragFilePath, code)){ status = -1; return; }

        auto fragment = compile_frag(code.c_str());
        if (fragment == UINT32_MAX){ status = -1; return; }

        id = glCreateProgram();

        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        
        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            GE_LOGE("Shader link failed: {}", infoLog);
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    GLShader(const char* _name, const ArrayConstView<Ref<ShaderUnit>>& s, bool addToLibrary, bool releaseShaderAfterUse){
        name = _name;

        id = glCreateProgram();

        for (auto&& su : s){
            su->Attach(*this);
        }
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            GE_LOGE("Shader link failed: {}", infoLog);
        }

        if (releaseShaderAfterUse)
        for (auto&& su : s){
            su->Release();
        }
    }

    ~GLShader(){
        glDeleteProgram(id);
        id = -1;
    }

    void Use() const {
        glUseProgram(id);
    }

    void Unuse() const {
        glUseProgram(0);
    }

    void SetInt(const char* name, int value){
        GLint location = glGetUniformLocation(id, name);
		glUniform1i(location, value);
    }

	void SetIntArray(const char* name, const Array<int>& ints){
        GLint location = glGetUniformLocation(id, name);
        glUniform1iv(location, ints.size(), ints.data());
    }

	void SetFloat(const char* name, float value){
        GLint location = glGetUniformLocation(id, name);
        glUniform1f(location, value);
    }

	void SetFloat2(const char* name, const M::VecF<2>& value){
        GLint location = glGetUniformLocation(id, name);
        glUniform2f(location, value.x, value.y);
    }

	void SetFloat3(const char* name, const M::VecF<3>& value){
        GLint location = glGetUniformLocation(id, name);
        glUniform3f(location, value.x, value.y, value.z);
    }

	void SetFloat4(const char* name, const M::VecF<4>& value){
        GLint location = glGetUniformLocation(id, name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

	void SetMat4(const char* name, const M::MatrixF<4,4>& value){
        GLint location = glGetUniformLocation(id, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

};

constexpr bool strequals_fixed(const char* a, const char* b, int len) {
    for (int i=0; i<len; i++) if (a[i] != b[i]) return false;
    return true;
}

class GLShaderUnit : public ShaderUnit {
public:
    int status = 0;
    ShaderType type;

    constexpr ShaderType GetTypeFromCode(const char* code, int len){
        for (int i=0; i < len; i++){
            if (code[i++] == '#' && i + 7 < len && strequals_fixed(code + i, "define ", 7)){
                i += 7;
                if (i + 6 + 1 < len && strequals_fixed(code + i, "vertex", 6)) return ShaderType::VERTEX;
                if (i + 5 + 1 < len && strequals_fixed(code + i, "pixel", 5)) return ShaderType::FRAGMENT;
                if (i + 8 + 1 < len && strequals_fixed(code + i, "fragment", 8)) return ShaderType::FRAGMENT;
                break;
            }
        }
        return ShaderType::NONE;
    }

    void create(const char* code, int len, ShaderType t){
        type = t == ShaderType::NONE ? GetTypeFromCode(code, len) : t;
        if (type == ShaderType::NONE) { status = -1; return; }

        switch (type) {
            case ShaderType::VERTEX:{
                id = compile_vert(code);
                if (id == UINT32_MAX) { status = -1; return; }
                break;
            }
            case ShaderType::FRAGMENT:{
                id = compile_frag(code);
                if (id == UINT32_MAX) { status = -1; return; }
                break;
            }
            default: break;
        }
    }

    GLShaderUnit(const char* code, int len, ShaderType t){
        create(code, len, t);
    }

    GLShaderUnit(const char* path, ShaderType t){
        if (t == ShaderType::NONE) {
            int dotIndex, len = strlen(path);
            for (dotIndex=0; dotIndex < len && path[dotIndex] != '.'; dotIndex++);
            dotIndex += 1;
            if (dotIndex + 4 < len) {
                if (strequals_fixed(path + dotIndex, "vert", 4)) type = ShaderType::VERTEX;
                else if (strequals_fixed(path + dotIndex, "frag", 4)) type = ShaderType::FRAGMENT;
                else type = ShaderType::NONE;
            } else type = ShaderType::NONE;
        } else type = t;
        
        std::string code;
        if (!read_file(path, code)){ status = -1; return; }
        create(code.c_str(), code.length(), type);
    }

    void Attach(const GameEngine::Shader& sh) const {
        glAttachShader(sh.GetID(), id);
    }
    
    void Release() {
        glDeleteShader(id);
        id = -1;
    }

    ~GLShaderUnit(){
        if (id != -1){
            Release();
        }
    }
		
    ShaderType GetShaderType() const { return type; }
};

}