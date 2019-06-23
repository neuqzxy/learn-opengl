#version 330 core
out vec4 FragColor;
in vec3 Normal;

uniform vec3 aColor;
uniform vec3 lightColor;

uniform vec3 viewPos; // 摄像机位置
// 根据下面两个坐标计算光线向量
uniform vec3 lightPos; // 光源位置
in vec3 FragPos; // 片段位置（基于世界坐标系）

void main() {
    // 环境光照分量
    float ambientStrength = 0.1; // 环境光照强度
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.5; // 镜面光照强度

    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(lightPos - FragPos); // 求光线向量

    // 求镜面反射的光线向量
    vec3 viewDir = normalize(viewPos - FragPos); // 视线向量
    vec3 reflectDir = reflect(-lightDir, norm); // 求出反射的光线向量

    // 计算镜面反射的光线在视线向量上的分量
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32); // 求32次幂，表示反光度
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0); // 点乘，求法线方向上的分量

    // 漫反射分量
    vec3 diffuse = diff * lightColor;

    // 总输出
    FragColor = vec4((ambient + diffuse + specular)*aColor, 1.0f);
}