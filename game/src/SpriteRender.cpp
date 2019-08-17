#include "SpriteRender.hpp"
#include <GLFW/glfw3.h>

// 初始化VAO和VBO，保存VAO到quadVAO
void SpriteRender::initRenderData() {
    std::cout<<"初始化VAO和VBO"<<std::endl;
    // 配置VAO和VBO
    GLuint VBO;
    float vertices[] {
        // 位置     // 纹理
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRender::DrawSprite(const Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate,
                              glm::vec3 color) {
    shader.Use();
    // 当试图在一个场景中用旋转矩阵和缩放矩阵放置一个对象的时候，建议是首先做缩放变换，再旋转，最后才是位移变换。
    // 因为矩阵乘法是从右向左执行的，所以我们变换的矩阵顺序是相反的：移动，旋转，缩放。
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(position, 1.0f));
    model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader.setMatrix4("model", model);
    shader.setVector3f("spriteColor", color);
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shader.ID, "uniformColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0);
    // 绑定纹理目标到纹理单元
    texture.Bind();
    // 绑定VAO绘制图形
    glBindVertexArray(quadVAO);
    // 设置绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    // unbind
    glBindVertexArray(0);
}

SpriteRender::SpriteRender(Shader &shader): shader(shader) {
    initRenderData();
}

SpriteRender::~SpriteRender() {
    glDeleteVertexArrays(1, &quadVAO);
}
