#version 330 core
out vec4 FragColor;
in vec3 Normal;

uniform vec3 viewPos; // 摄像机位置
// 根据下面两个坐标计算光线向量
//uniform vec3 lightPos; // 光源位置
in vec3 FragPos; // 片段位置（基于世界坐标系）

// 定义一个材质
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

/* 向量*向量表示分量相乘 */

void main() {
    // 环境光照分量
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(light.position - FragPos); // 求光线向量

    float diff = max(dot(norm, lightDir), 0.0); // 点乘，求法线方向上的分量

    // 漫反射分量
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // 求镜面反射的光线向量
    vec3 viewDir = normalize(viewPos - FragPos); // 视线向量
    vec3 reflectDir = reflect(-lightDir, norm); // 求出反射的光线向量

    // 计算镜面反射的光线在视线向量上的分量
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    // 总输出
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}