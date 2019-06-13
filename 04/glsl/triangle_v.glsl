#version 330 core
layout (location = 0) in vec3 aPos; // 顶点坐标
layout (location = 1) in vec2 aTexcoord; // 获取纹理坐标

uniform mat4 model; // 传入rotate的4X4矩阵
uniform mat4 view;
uniform mat4 projection;

out vec2 Texcoord;

void main()
{
    Texcoord = aTexcoord; // 将纹理坐标传入片元着色器
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}