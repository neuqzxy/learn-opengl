//
// Created by 周欣宇 on 2019-06-02.
//
#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "初次见面，请多多指教~", NULL, NULL);
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

    // 绘制6个顶点
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void initEBO() {
    // 索引数组
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 2, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // 生成一个EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // 与VBO类似，我们先绑定EBO然后用glBufferData把索引复制到缓冲里。
    // 同样，和VBO类似，我们会把这些函数调用放在绑定和解绑函数调用之间，只不过这次我们把缓冲的类型定义为GL_ELEMENT_ARRAY_BUFFER。
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void initVBO() {
    u_int vbo_0;
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,    // 顶部
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f    // 顶点偏左角
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
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

int main() {

    GLFWwindow* window = nullptr;

    initWindow(window);

    std::string vShaderPath = "../glsl/triangle_v_2.glsl";
    std::string fShaderPath = "../glsl/triangle_f_2.glsl";
    Shader shader(vShaderPath, fShaderPath);

    initVAO();
    initVBO();
    initEBO();

    shader.use();

    activeHC();

    if (window == nullptr) {
        std::cout<<"window is nullptr"<<std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        // 读取用户输入，判断是否退出
        processInput(window);

        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw();

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
