#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID; // 存储着色器程序的ID
    Shader() = default;
    // 设置当前的shader为active
    Shader& Use();
    // 读取并构建着色器
    void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar *geometrySource = nullptr);
    // uniform工具函数
    void setInt(const GLchar *name, GLint value, GLboolean useShader = false);
    void setBool(const GLchar *name, GLboolean value, GLboolean useShader = false);
    void setFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
    void setVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void setVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void setVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void setVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void setVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void setVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void setMatrix4 (const GLchar *name, const glm::mat4 &value, GLboolean useShader = false);
    // 检查是否编译或者链接错误，如果错误，打印出错误
    void checkCompileErrors(GLuint object, std::string type);
};

#endif //PART1_SHADER_H
