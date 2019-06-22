#version 330 core
layout (location = 0) in vec3 aPos; // 顶点坐标
layout (location = 1) in vec3 aNormal; // 法向量

uniform mat4 model; // 传入rotate的4X4矩阵
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal; // 导出法向量
out vec3 FragPos; // 导出片段位置，基于世界坐标系的坐标

void main()
{
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));
}

//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//
//out vec3 FragPos;
//out vec3 Normal;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main()
//{
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    Normal = aNormal;
//
//    gl_Position = projection * view * vec4(FragPos, 1.0);
//}