#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameLevel.hpp"
#include "BallObject.hpp"

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

    std::vector<GameLevel> Levels;
    GLuint                 Level;

    Game(GLuint width, GLuint height);
    ~Game();

    // 初始化游戏
    void Init();
    // 碰撞检测
    void DoCollisions();
    static GLboolean CheckCollision(GameObject& box1, GameObject& box2); // aabb检测碰撞
    static GLboolean CheckCollision(BallObject& ball, GameObject& box); // aabb球体和矩形碰撞检测
    // 游戏循环
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};

#endif
