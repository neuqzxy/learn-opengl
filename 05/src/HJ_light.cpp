//
// Created by 周欣宇 on 2019-06-22.
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

struct Cube {
    float color[3];
    glm::vec3 position;
};

Cube cubes[]{
        {{1.0f, 0.5f, 0.31f}, glm::vec3(0.0f,  0.0f, -1.0f)},
        {{1.0f, 0.5f, 1.0f}, glm::vec3(3.0f, 0.0f, -5.0f)}
};

void rotate(Shader &cubeShader, Shader &lightShader) {
    glm::mat4 model{1.0f}, view{1.0f}, projection{1.0f}; // 生成变换矩阵，观察矩阵，透视投影矩阵

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

    // 传递给uniform变量
    u_int cubeID{cubeShader.getID()}, lightID{lightShader.getID()};

    // 渲染cube和light
    for (int i = 0; i < sizeof(cubes)/sizeof(cubes[0]); i++) {
        auto cube = cubes[i];
        model = glm::mat4{1.0f};
        model = glm::translate(model, cube.position);
        model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        if (i == 0) {
            cubeShader.use(); // 在设置uniform之前必须要use一下
            u_int view_location = glGetUniformLocation(cubeID, "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
            u_int projection_location = glGetUniformLocation(cubeID, "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
            u_int model_location = glGetUniformLocation(cubeID, "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3f(glGetUniformLocation(cubeID, "aColor"), cube.color[0], cube.color[1], cube.color[2]);
            glUniform3f(glGetUniformLocation(cubeID, "lightColor"), cubes[1].color[0], cubes[1].color[1], cubes[1].color[2]);
        } else {
            lightShader.use(); // 在设置uniform之前必须要use一下
            u_int view_location = glGetUniformLocation(lightID, "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
            u_int projection_location = glGetUniformLocation(lightID, "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
            u_int model_location = glGetUniformLocation(lightID, "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
            glUniform3f(glGetUniformLocation(lightID, "aColor"), cube.color[0], cube.color[1], cube.color[2]);
        }
        draw();
    }
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
}

// 绘制draw
void draw() {
    // 设置绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

u_int initVBO() {
    u_int vbo_0;
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };

    glGenBuffers(1, &vbo_0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    return vbo_0;
}

u_int initVAO() {
    u_int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

void activeHC() {
    // 解析顶点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

int main() {

    GLFWwindow* window = nullptr;

    initWindow(window);

    glEnable(GL_DEPTH_TEST); // 启动深度测试

    std::string vCubeShaderPath = "../glsl/triangle_v.glsl";
    std::string fCubeShaderPath = "../glsl/triangle_f_cube_hj.glsl";
    std::string vLightShaderPath = "../glsl/triangle_v.glsl";
    std::string fLightShaderPath = "../glsl/triangle_f_light.glsl";
    Shader cubeShader(vCubeShaderPath, fCubeShaderPath);
    Shader lightShader(vLightShaderPath, fLightShaderPath);

    initVAO();
    u_int VBO = initVBO();
    activeHC();

    initVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    activeHC();

    if (window == nullptr) {
        std::cout<<"window is nullptr"<<std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        // 读取用户输入，判断是否退出
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotate(cubeShader, lightShader);

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
