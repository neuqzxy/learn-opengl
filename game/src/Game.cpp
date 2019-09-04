#include <glad/glad.h>
#include "Game.hpp"
#include "SpriteRender.hpp"
#include "ResourceManager.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"

SpriteRender *Renderer;

const glm::vec2 PLAYER_SIZE{100, 20};
const GLfloat PLAYER_VELOCITY{500.0f};

GameObject * player;

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

BallObject *ball;

Game::Game(GLuint width, GLuint height): State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {}

// 读取着色器代码，并编译
// 初始化精灵的VAO和VBO，加载纹理
void Game::Init() {
    ResourceManager::LoadShader("../script/sprite.vert", "../script/sprite.frag", nullptr, "sprite").Use();
    // 向着色器传入数据
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").setInt("image", 0);
    ResourceManager::GetShader("sprite").setMatrix4("projection", projection);

    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRender(shader);
    // 加载纹理
    ResourceManager::LoadTexture("../resources/awesomeface.png", true, "awesomeface");
    ResourceManager::LoadTexture("../resources/background.jpg", false, "background");
    ResourceManager::LoadTexture("../resources/block.png", true, "block");
    ResourceManager::LoadTexture("../resources/block_solid.png", true, "block_solid");
    ResourceManager::LoadTexture("../resources/paddle.png", true, "paddle");

    // 加载关卡
    GameLevel one;
    one.Load("../levels/one.lvl", Width, int64_t(Height*0.5));
    Levels.push_back(one);
    Level = 0;

    // 加载player
    glm::vec2 playerPos = glm::vec2(
            float(Width) / 2 - PLAYER_SIZE.x / 2,
            float(Height) - PLAYER_SIZE.y
    );
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"), {1.0f, 1.0f, 1.0f}, {0.0f, PLAYER_VELOCITY});

    // 加载ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("awesomeface"));
}

void Game::Update(GLfloat dt) {
    ball->Move(dt, Width);
}

void Game::ProcessInput(GLfloat dt) {
    if (Keys[GLFW_KEY_A]) {
        player->Position.x -= dt * PLAYER_VELOCITY;
        if (ball->Stuck) {
            ball->Position.x -= dt * PLAYER_VELOCITY;
        }
    }
    if (Keys[GLFW_KEY_D]) {
        player->Position.x += dt * PLAYER_VELOCITY;
        if (ball->Stuck) {
            ball->Position.x += dt * PLAYER_VELOCITY;
        }
    }
    if (Keys[GLFW_KEY_SPACE]) {
        ball->Stuck = false;
    }
}

void Game::Render() {
    if(this->State == GAME_ACTIVE) {
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                             glm::vec2(0, 0), glm::vec2(Width, Height), 0.0f
        );
        // 绘制关卡
        Levels[Level].Draw(*Renderer);
        // 绘制player
        player->Draw(*Renderer);
        // 绘制ball
        ball->Draw(*Renderer);
    }
//    Renderer->DrawSprite(ResourceManager::GetTexture("awesomeface"), glm::vec2(200, 100), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
