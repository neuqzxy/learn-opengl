#include <glad/glad.h>
#include "Game.hpp"
#include "SpriteRender.hpp"
#include "ResourceManager.hpp"

SpriteRender *Renderer;

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
    ResourceManager::LoadTexture("../resources/awesomeface.png", true, "awesomeface");
}

void Game::Update(GLfloat dt) {}

void Game::ProcessInput(GLfloat dt) {}

void Game::Render() {
    Renderer->DrawSprite(ResourceManager::GetTexture("awesomeface"), glm::vec2(200, 100), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
