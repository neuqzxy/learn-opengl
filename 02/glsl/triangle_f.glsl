#version 330 core
out vec4 FragColor;
in vec2 Texcoord; // 这里是映射关系，因为映射关系和顶点坐标有关，所以我们从顶点着色器中传入

// 供纹理对象使用的内建数据类型，叫做采样器(Sampler)，它以纹理类型作为后缀，比如sampler1D、sampler3D
uniform sampler2D ourTexture;

void main() {
    // 使用内建函数采样纹理
    // 第一个参数是纹理采样器，第二个参数是对应的纹理坐标
    FragColor = texture(ourTexture, Texcoord);
}