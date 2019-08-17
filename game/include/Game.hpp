#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

/**
 * 游戏类
 */
class Game {
public:
    GameState State;
    GLboolean Keys[1024];
    GLuint Width, Height;

    Game(GLuint width, GLuint height);
    ~Game();

    // 初始化游戏
    void Init();
    // 游戏循环
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};

#endif
