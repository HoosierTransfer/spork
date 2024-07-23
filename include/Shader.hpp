#pragma once

#include <glad/glad.h>
  
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <utility>
#include <ShaderType.hpp>
  
class Shader {
private:
    std::string vertexCode;
    std::string fragmentCode;
    std::vector<std::pair<std::string, std::string>> vertexDefines;
    std::vector<std::pair<std::string, std::string>> fragmentDefines;
    bool compiled = false;
public:
    unsigned int ID;

    Shader() = default;
  
    Shader(const std::string &shaderName);

    void addDefine(const std::string &name, const std::string &value, ShaderType shaderType);

    void removeDefine(const std::string &name, ShaderType shaderType);

    const std::string& getDefine(const std::string &name, ShaderType shaderType);

    void compile();

    void use();

    bool isCompiled() const;

    // set single uniform value
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    // set index in array for uniform value
    void setBoolInArray(const std::string &name, bool value, int index) const;
    void setIntInArray(const std::string &name, int value, int index) const;
    void setFloatInArray(const std::string &name, float value, int index) const;

    void setVec2InArray(const std::string &name, const glm::vec2 &value, int index) const;
    void setVec3InArray(const std::string &name, const glm::vec3 &value, int index) const;
    void setVec4InArray(const std::string &name, const glm::vec4 &value, int index) const;

    void setMat2InArray(const std::string &name, const glm::mat2 &mat, int index) const;
    void setMat3InArray(const std::string &name, const glm::mat3 &mat, int index) const;
    void setMat4InArray(const std::string &name, const glm::mat4 &mat, int index) const;

    // set whole array of uniform values
    void setBoolArray(const std::string &name, bool *value, int size) const;
    void setIntArray(const std::string &name, int *value, int size) const;
    void setFloatArray(const std::string &name, float *value, int size) const;

    void setVec2Array(const std::string &name, glm::vec2 *value, int size) const;
    void setVec3Array(const std::string &name, glm::vec3 *value, int size) const;
    void setVec4Array(const std::string &name, glm::vec4 *value, int size) const;
    
    void setMat2Array(const std::string &name, glm::mat2 *mat, int size) const;
    void setMat3Array(const std::string &name, glm::mat3 *mat, int size) const;
    void setMat4Array(const std::string &name, glm::mat4 *mat, int size) const;

    
    void setBoolInStructArray(const std::string &name, const std::string &field, bool value, int index) const;
    void setIntInStructArray(const std::string &name, const std::string &field, int value, int index) const;
    void setFloatInStructArray(const std::string &name, const std::string &field, float value, int index) const;

    void setVec2InStructArray(const std::string &name, const std::string &field, const glm::vec2 &value, int index) const;
    void setVec3InStructArray(const std::string &name, const std::string &field, const glm::vec3 &value, int index) const;
    void setVec4InStructArray(const std::string &name, const std::string &field, const glm::vec4 &value, int index) const;

    void setMat2InStructArray(const std::string &name, const std::string &field, const glm::mat2 &mat, int index) const;
    void setMat3InStructArray(const std::string &name, const std::string &field, const glm::mat3 &mat, int index) const;
    void setMat4InStructArray(const std::string &name, const std::string &field, const glm::mat4 &mat, int index) const;
};