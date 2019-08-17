#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include "Shader.hpp"
#include "Texture.hpp"

class ResourceManager {
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    // 加载
    static Shader& LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader& GetShader(std::string name);
    static Texture2D& LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
    static Texture2D& GetTexture(std::string name);
    static void Clear();
private:
    // 私有构造函数，不希望实例化对象。
    ResourceManager() {}
    // 加载和生成shader
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    // 从文件中加载单个纹理
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif
