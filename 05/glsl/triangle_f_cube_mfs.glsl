#version 330 core
out vec4 FragColor;
in vec3 Normal;

uniform vec3 aColor;
uniform vec3 lightColor;

// 根据下面两个坐标计算光线向量
uniform vec3 lightPos; // 光源位置
in vec3 FragPos; // 片段位置（基于世界坐标系）

void main() {
    // 环境光照分量
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(lightPos - FragPos); // 求光线向量

    float diff = max(dot(norm, lightDir), 0.0); // 点乘，求法线方向上的分量

    // 漫反射分量
    vec3 diffuse = diff * lightColor;

    // 总输出
    FragColor = vec4((ambient + diffuse)*aColor, 1.0f);
}