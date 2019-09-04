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

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

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
    static Collision CheckCollision(GameObject& box1, GameObject& box2); // aabb检测碰撞
    static Collision CheckCollision(BallObject& ball, GameObject& box); // aabb球体和矩形碰撞检测
    static Direction VectorDirection(const glm::vec2& target); // 获取一个矢量的方向
    // 游戏循环
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};

#endif
