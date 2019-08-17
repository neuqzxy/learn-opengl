#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Game.hpp"
#include "SpriteRender.hpp"

const GLint WIN_WIDTH = 800, WIN_HEIGHT = 600;
Game Breakout(WIN_WIDTH, WIN_HEIGHT);

// 回调函数，每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
// 回调函数，键盘事件触发
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.Keys[key] = GL_FALSE;
    }
}

GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE); // 窗口最大化
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 指定要为其创建上下文的OpenGL配置文件

    #ifdef __APPLE__
    // Mac环境需要这行配置
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "游戏", nullptr, nullptr);
    if (window == nullptr) {
        std::cout<<"create window error";
        return nullptr;
    }
    glfwMakeContextCurrent(window);


    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glfwSetKeyCallback(window, key_callback);

    // 设置视口大小，前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    // OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，
    // 将OpenGL中的位置坐标转换为你的屏幕坐标。
    // 例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。
    // 注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    // 注册窗口大小改变的事件回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//    glEnable(GL_CULL_FACE); // 面剔除
    glEnable(GL_BLEND); // 混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return window;
}


int main() {
    GLFWwindow* window = initWindow();
    if (window == nullptr) return -1;

    Breakout.Init();
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    Breakout.State = GAME_ACTIVE;

    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //deltaTime = 0.001f;
        // Manage user input
        Breakout.ProcessInput(deltaTime);

        // Update Game state
        Breakout.Update(deltaTime);
//        std::cout<<"deltaTime: "<<deltaTime<<", currentTime: "<<currentFrame<<std::endl;

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);

        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
