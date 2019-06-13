//
// Created by 周欣宇 on 2019-06-13.
//
#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern void draw();

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

glm::vec3 positions[]{
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 2.0f),
        glm::vec3(1.0f, 2.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 2.0f),
        glm::vec3(3.0f, 1.0f, 0.0f),
};

// 观察矩阵（摄像机）参数
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// 鼠标位置
float lastX{400}, lastY{300};
// 欧拉角
float pitch{0.0f}, yaw{0.0f};
// 鼠标滚轮角度
float fov{0};

void rotate(Shader &shader) {
    for (auto pos : positions) {
        glm::mat4 model{1.0f}, view{1.0f}, projection{1.0f}; // 生成变换矩阵，观察矩阵，透视投影矩阵

        float step = glfwGetTime();

        model = glm::translate(model, pos);

        model = glm::rotate(model, glm::radians(50.0f), pos);

        // 通过鼠标偏移设置cameraFront
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);

        // 观察矩阵向z轴反方向移动，也就是让场景向前移动
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // 设置透视投影矩阵
        projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

        // 传递给uniform变量
        u_int ID{shader.getID()};
        u_int model_location = glGetUniformLocation(ID, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        u_int view_location = glGetUniformLocation(ID, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        u_int projection_location = glGetUniformLocation(ID, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        draw();
    }
}

void loadTexure() {
    int width, height, nrChannels;
    u_char *data = stbi_load("../resources/container.jpg", &width, &height, &nrChannels, 0); // 加载图片

    // 生成纹理
    u_int texture, texture2;
    glGenTextures(1, &texture); // 生成一个纹理
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定到2D纹理缓冲

    // 将图片数据传入2D纹理单元中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放图像内存
    stbi_image_free(data);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/awesomeface.png", &width, &height, &nrChannels, 0); // 加载图片
    glGenTextures(1, &texture2); // 生成一个纹理
    glBindTexture(GL_TEXTURE_2D, texture2); // 绑定到2D纹理缓冲

    // 将图片数据传入2D纹理单元中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 将texture1和GL_TEXTURE0关联，间接关联到纹理0号单元
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 将texture2和GL_TEXTURE1关联，间接关联到纹理1号单元
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 释放图片数据内存
    stbi_image_free(data);
}

// 回调函数，每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int initWindow(GLFWwindow *&window) {
    glfwInit(); // 初始化GLFW
    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 指定创建的上下文必须兼容的客户端API版本。这些提示的确切行为取决于请求的客户端API。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 指定创建的上下文必须兼容的客户端API版本。这些提示的确切行为取决于请求的客户端API。
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE); // 窗口最大化
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 指定要为其创建上下文的OpenGL配置文件

    // Mac环境需要这行配置
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建一个窗口对象，返回GLFWwindow对象地址
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "你好，OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置视口大小，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    // OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，
    // 将OpenGL中的位置坐标转换为你的屏幕坐标。
    // 例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。
    // 注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // 注册窗口大小改变的事件回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return 0;
}

// 读取用户输入
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 0.01f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// 鼠标事件回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX, yoffset = lastY - ypos, sensitivity = 0.05f;
    lastX = xpos;lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    pitch += yoffset;
    yaw += xoffset;
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

// 鼠标滚轮事件
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraPos += float(yoffset) * cameraFront;
}

// 绘制draw
void draw() {
    // 设置绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 绘制6个顶点
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void initVBO() {
    u_int vbo_0;
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenBuffers(1, &vbo_0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
}

void initVAO() {
    u_int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void activeHC() {
    // 解析顶点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 解析纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

int main() {

    GLFWwindow* window = nullptr;

    initWindow(window);

    glEnable(GL_DEPTH_TEST); // 启动深度测试

    std::string vShaderPath = "../glsl/triangle_v.glsl";
    std::string fShaderPath = "../glsl/triangle_f.glsl";
    Shader shader(vShaderPath, fShaderPath);

    loadTexure();

    initVAO();
    initVBO();

    shader.use();

    glUniform1i(glGetUniformLocation(shader.getID(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.getID(), "texture2"), 1);

    activeHC();

    if (window == nullptr) {
        std::cout<<"window is nullptr"<<std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        // 读取用户输入，判断是否退出
        processInput(window);
        // 读取鼠标事件
        glfwSetCursorPosCallback(window, mouse_callback);
        // 读取鼠标滚轮事件
        glfwSetScrollCallback(window, scroll_callback);

        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotate(shader);

//        draw();

        // 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        /**
         * 双缓冲(Double Buffer)
         * 应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
         * 这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
         * 最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
         * 为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；
         * 而所有的的渲染指令都会在后缓冲上绘制。
         * 当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
         */
        glfwSwapBuffers(window);

        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }
}