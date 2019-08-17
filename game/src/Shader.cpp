#include <iostream>
#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include "Shader.hpp"

Shader& Shader::Use() {
    glUseProgram(ID);
    return *this;
}

// 编译链接着色器
void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource) {
    GLuint sVertex, sFragment, gShader;
    // 顶点着色器
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    // 片元着色器
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    // 传入geometry的话，编译这个getmetry shader
    if (geometrySource != nullptr) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    // 创建一个program
    ID = glCreateProgram();
    glAttachShader(ID, sVertex);
    glAttachShader(ID, sFragment);
    if (geometrySource != nullptr) {
        glAttachShader(ID, gShader);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr) {
        glDeleteShader(gShader);
    }
}

void Shader::setInt(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setBool(const GLchar *name, GLboolean value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name), GLint(value));
}
void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::setVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::setVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::setVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::setVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::setVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::setVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const GLchar *name, const glm::mat4 &value, GLboolean useShader) {
    if (useShader) this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

// 错误捕获函数
void Shader::checkCompileErrors(GLuint object, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
