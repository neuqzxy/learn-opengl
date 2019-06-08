//
// Created by 周欣宇 on 2019-06-02.
//
#ifndef PART1_SHADER_H
#define PART1_SHADER_H

class Shader {
private:
    u_int ID; // 存储着色器程序的ID
public:
    inline u_int getID() const {
        return ID;
    }
    // 读取并构建着色器
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    // 激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif //PART1_SHADER_H
