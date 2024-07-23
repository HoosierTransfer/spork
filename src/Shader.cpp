#include <Shader.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Shader::Shader(const string &shaderName) {
    compiled = false;
    string vertexPath = "assets/shaders/" + shaderName + ".vsh";
    string fragmentPath = "assets/shaders/" + shaderName + ".fsh";
    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
    // get defines from shader code
    size_t pos = 0;
    while ((pos = vertexCode.find("#define ", pos)) != string::npos) {
        size_t end = vertexCode.find("\n", pos);
        string define = vertexCode.substr(pos, end - pos);
        size_t nameStart = define.find(" ") + 1;
        size_t nameEnd = define.find(" ", nameStart);
        size_t valueStart = nameEnd + 1;
        string name = define.substr(nameStart, nameEnd - nameStart);
        string value = define.substr(valueStart);
        vertexDefines.push_back(make_pair(name, value));
        pos = end;
    }
    pos = 0;
    while ((pos = fragmentCode.find("#define ", pos)) != string::npos) {
        size_t end = fragmentCode.find("\n", pos);
        string define = fragmentCode.substr(pos, end - pos);
        size_t nameStart = define.find(" ") + 1;
        size_t nameEnd = define.find(" ", nameStart);
        size_t valueStart = nameEnd + 1;
        string name = define.substr(nameStart, nameEnd - nameStart);
        string value = define.substr(valueStart);
        fragmentDefines.push_back(make_pair(name, value));
        pos = end;
    }
    
}

void Shader::addDefine(const string &name, const string &value, ShaderType shaderType) {
    auto addOrReplaceDefine = [&](string& shaderCode) {
        size_t pos = shaderCode.find("#version");
        if (pos != string::npos) {
            pos = shaderCode.find("\n", pos);
            size_t definePos = shaderCode.find("#define " + name);
            if (definePos != string::npos) {
                size_t end = shaderCode.find("\n", definePos);
                shaderCode.replace(definePos, end - definePos, "#define " + name + " " + value);
            } else {
                shaderCode.insert(pos + 1, "#define " + name + " " + value + "\n");
            }
        }
    };

    switch (shaderType) {
        case ShaderType::VERTEX:
            addOrReplaceDefine(vertexCode);
            vertexDefines.push_back(make_pair(name, value));
            break;
        case ShaderType::FRAGMENT:
            addOrReplaceDefine(fragmentCode);
            fragmentDefines.push_back(make_pair(name, value));
            break;
        default:
            break;
    }
    
}

void Shader::removeDefine(const string &name, ShaderType shaderType) {
    auto removeDefine = [&](string& shaderCode) {
        size_t pos = shaderCode.find("#define " + name);
        if (pos != string::npos) {
            size_t end = shaderCode.find("\n", pos);
            shaderCode.erase(pos, end - pos + 1);
        }
    };

    switch (shaderType) {
        case ShaderType::VERTEX:
            removeDefine(vertexCode);
            vertexDefines.erase(remove_if(vertexDefines.begin(), vertexDefines.end(), [&](pair<string, string> define) {
                return define.first == name;
            }), vertexDefines.end());
            break;
        case ShaderType::FRAGMENT:
            removeDefine(fragmentCode);
            fragmentDefines.erase(remove_if(fragmentDefines.begin(), fragmentDefines.end(), [&](pair<string, string> define) {
                return define.first == name;
            }), fragmentDefines.end());
            break;
        default:
            break;
    }
}

const string& Shader::getDefine(const string &name, ShaderType shaderType) {
    static const string emptyString = "";
    auto getDefine = [&](vector<pair<string, string>>& defines) -> const string& {
        auto it = find_if(defines.begin(), defines.end(), [&](pair<string, string> define) {
            return define.first == name;
        });
        if (it != defines.end()) {
            return it->second;
        }
        return emptyString;
    };

    switch (shaderType) {
        case ShaderType::VERTEX:
            return getDefine(vertexDefines);
        case ShaderType::FRAGMENT:
            return getDefine(fragmentDefines);
        default:
            return emptyString;
    }
}

void Shader::compile() {
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    compiled = true;
}

bool Shader::isCompiled() const {
    return compiled;
}

void Shader::use() {
    glUseProgram(ID);
}

// set single uniform value

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(const string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// set index in array for uniform value

void Shader::setBoolInArray(const string &name, bool value, int index) const {
    glUniform1i(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), (int)value);
}

void Shader::setIntInArray(const string &name, int value, int index) const {
    glUniform1i(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), value);
}

void Shader::setFloatInArray(const string &name, float value, int index) const {
    glUniform1f(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), value);
}

void Shader::setVec2InArray(const string &name, const glm::vec2 &value, int index) const {
    glUniform2fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, &value[0]);
}

void Shader::setVec3InArray(const string &name, const glm::vec3 &value, int index) const {
    glUniform3fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, &value[0]);
}

void Shader::setVec4InArray(const string &name, const glm::vec4 &value, int index) const {
    glUniform4fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, &value[0]);
}

void Shader::setMat2InArray(const string &name, const glm::mat2 &mat, int index) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3InArray(const string &name, const glm::mat3 &mat, int index) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4InArray(const string &name, const glm::mat4 &mat, int index) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]").c_str()), 1, GL_FALSE, &mat[0][0]);
}

// set whole array of uniform values

void Shader::setBoolArray(const string &name, bool *value, int size) const {
    glUniform1iv(glGetUniformLocation(ID, name.c_str()), size, (int *)value);
}

void Shader::setIntArray(const string &name, int *value, int size) const {
    glUniform1iv(glGetUniformLocation(ID, name.c_str()), size, value);
}

void Shader::setFloatArray(const string &name, float *value, int size) const {
    glUniform1fv(glGetUniformLocation(ID, name.c_str()), size, value);
}

void Shader::setVec2Array(const string &name, glm::vec2 *value, int size) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), size, &value[0][0]);
}

void Shader::setVec3Array(const string &name, glm::vec3 *value, int size) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), size, &value[0][0]);
}

void Shader::setVec4Array(const string &name, glm::vec4 *value, int size) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), size, &value[0][0]);
}

void Shader::setMat2Array(const string &name, glm::mat2 *mat, int size) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), size, GL_FALSE, &mat[0][0][0]);
}

void Shader::setMat3Array(const string &name, glm::mat3 *mat, int size) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), size, GL_FALSE, &mat[0][0][0]);
}

void Shader::setMat4Array(const string &name, glm::mat4 *mat, int size) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), size, GL_FALSE, &mat[0][0][0]);
}


void Shader::setBoolInStructArray(const std::string &name, const std::string &field, bool value, int index) const {
    glUniform1i(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), (int)value);
}

void Shader::setIntInStructArray(const std::string &name, const std::string &field, int value, int index) const {
    glUniform1i(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), value);
}

void Shader::setFloatInStructArray(const std::string &name, const std::string &field, float value, int index) const {
    glUniform1f(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), value);
}

void Shader::setVec2InStructArray(const std::string &name, const std::string &field, const glm::vec2 &value, int index) const {
    glUniform2fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, &value[0]);
}

void Shader::setVec3InStructArray(const std::string &name, const std::string &field, const glm::vec3 &value, int index) const {
    glUniform3fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, &value[0]);
}

void Shader::setVec4InStructArray(const std::string &name, const std::string &field, const glm::vec4 &value, int index) const {
    glUniform4fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, &value[0]);
}

void Shader::setMat2InStructArray(const std::string &name, const std::string &field, const glm::mat2 &mat, int index) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3InStructArray(const std::string &name, const std::string &field, const glm::mat3 &mat, int index) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4InStructArray(const std::string &name, const std::string &field, const glm::mat4 &mat, int index) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, (name + "[" + to_string(index) + "]." + field).c_str()), 1, GL_FALSE, &mat[0][0]);
}
