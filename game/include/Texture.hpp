#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D {
public:
    GLuint ID; // 纹理对象的id
    GLuint Width, Height; // 纹理对象的宽高
    GLuint Internal_Format; // 纹理对象的格式
    GLuint Image_Format; // 所加载对象的格式
    // 纹理配置
    GLuint Wrap_S; // 纹理的环绕方式（s轴）
    GLuint Wrap_T; // 纹理的环绕方式（t轴）
    GLuint Filter_Min; // 过滤方式 if texture pixels < screen pixels
    GLuint Filter_Max; // 过滤方式 if texture pixels < screen pixels

    Texture2D();
    // 从图像数据生成纹理
    void Generate(GLuint width, GLuint height, unsigned char* data);
    // 绑定纹理到当前的active GL_TEXTURE_2D 纹理对象上
    void Bind() const;
};

#endif
