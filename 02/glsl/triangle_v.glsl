#version 330 core
layout (location = 0) in vec3 aPos; // 顶点坐标
layout (location = 1) in vec2 aTexcoord; // 获取纹理坐标

out vec2 Texcoord;

void main()
{
    Texcoord = aTexcoord; // 将纹理坐标传入片元着色器
    gl_Position = vec4(aPos, 1.0);
}