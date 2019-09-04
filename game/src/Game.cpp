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

Collision Game::CheckCollision(GameObject &box1, GameObject &box2) {
    bool collisionX = box1.Position.x + box1.Size.x >= box2.Position.x && box2.Position.x + box2.Size.x >= box1.Position.x;
    bool collisionY = box1.Position.y + box1.Size.y >= box2.Position.y && box2.Position.y + box2.Size.y >= box1.Position.y;
    if (collisionX && collisionY) {
        const glm::vec2 difference{box1.Position.x + box1.Size.x / 2 - box2.Position.x - box2.Size.x / 2, box1.Position.y + box1.Size.y / 2 - box2.Position.y - box2.Size.y / 2};
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    }
    return std::make_tuple(GL_FALSE, UP, glm::vec2{0.0f});
}

Collision Game::CheckCollision(BallObject &ball, GameObject &box) {
    const glm::vec2& ballCenter = ball.Position + glm::vec2{ball.Radius};
    const glm::vec2& aabbHalfExtents{box.Size.x / 2, box.Size.y / 2};
    const glm::vec2& boxCenter = box.Position + aabbHalfExtents;
    // 球体和矩形中心的矢量
    const glm::vec2 difference = ballCenter - boxCenter;
    const glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
    // 最近的点
    const glm::vec2 closest = boxCenter + clamped;
    if (glm::length(ballCenter - closest) < ball.Radius) {
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    }
    return std::make_tuple(GL_FALSE, UP, glm::vec2{0.0f});
}

void Game::DoCollisions() {
    GameLevel& currentLevel{Levels[Level]}; // 获取当前的关卡
    for (auto& brick : currentLevel.Bricks) {
        if (!brick.Destroyed) {
            Collision collision = CheckCollision(*ball, brick);
            if (std::get<0>(collision)) { // 如果检测到碰撞
                if (!brick.IsSolid)
                    brick.Destroyed = true;
                Direction direction{std::get<1>(collision)};
                glm::vec2 diffVector{std::get<2>(collision)};
                // 水平方向
                if (direction == RIGHT || direction == LEFT) {
                    ball->Velocity.x = -ball->Velocity.x;
                    // 重定位
                    GLfloat penetration = ball->Radius - std::abs(diffVector.x);
                    if (direction == LEFT) {
                        ball->Position.x -= penetration; // 将球左移
                    } else {
                        ball->Position.x += penetration; // 将球右移
                    }
                } else {
                    ball->Velocity.y = -ball->Velocity.y;
                    // 重定位
                    GLfloat penetration = ball->Radius - std::abs(diffVector.y);
                    if (direction == UP) {
                        ball->Position.y -= penetration; // 将球上移
                    } else {
                        ball->Position.y += penetration; // 将球下移
                    }
                }
            }
        }
    }

    Collision collision = CheckCollision(*ball, *player);
    // 如果玩家接住小球
    if (!ball->Stuck && std::get<0>(collision)) {
        GLfloat centerBoard = player->Position.x + player->Size.x / 2;
        GLfloat distance = ball->Position.x + ball->Radius - centerBoard;
        GLfloat percentage = distance / (player->Size.x / 2);
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->Velocity.y = -ball->Velocity.y;
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
    }
}

Direction Game::VectorDirection(const glm::vec2 &target) {
    glm::vec2 compass[]{
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };
    GLfloat max = 0.0f;
    GLuint bestMatch = -1;
    for (int i = 0; i < compass->length(); i++) {
        GLfloat dotNum = glm::dot(compass[i], target);
        if (dotNum > max) {
            max = dotNum;
            bestMatch = i;
        }
    }
    return Direction(bestMatch);
}

void Game::Update(GLfloat dt) {
    DoCollisions();
    ball->Move(dt, Width);
    if (ball->Position.y > Height) {

    }
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
