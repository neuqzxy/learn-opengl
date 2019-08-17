#include "ResourceManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stb_image.h>

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader& ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    for (auto &shader : Shaders) {
        glDeleteProgram(shader.second.ID);
    }
    for (auto &texture : Textures) {
        glDeleteTextures(1, &texture.second.ID);
    }
}

// 从文件中加载shader
Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile) {
    using std::string; using std::ifstream; using std::stringstream;
    string vertexCode;
    string fragmaneCode;
    string geometryCode;

    try {
        ifstream vertexShaderFile(vShaderFile);
        ifstream fragmentShaderFile(fShaderFile);
        stringstream vShaderStream, fShaderStream;

        vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmaneCode = fShaderStream.str();
        if (gShaderFile != nullptr) {
            ifstream geometryShaderFile(gShaderFile);
            stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch (ifstream::failure &e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmaneCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    std::cout<<strlen(vShaderCode)<<"\n"<<strlen(fShaderCode)<<"\n";

    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

// 从文件中加载纹理
Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha) {
    Texture2D texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    int width, height, channel;
    unsigned char *image = stbi_load(file, &width, &height, &channel, alpha ? 4 : 3);
    if (image == nullptr) {
        std::cout<<"image load fail";
    }
    texture.Generate(width, height, image);

    stbi_image_free(image);
    return texture;
}
