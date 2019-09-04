//
// Created by 周欣宇 on 2019-08-25.
//

#ifndef GAME_GAMELEVEL_HPP
#define GAME_GAMELEVEL_HPP

#include <glad/glad.h>
#include <vector>
#include "GameObject.hpp"
#include "SpriteRender.hpp"

class GameLevel {
public:
    std::vector<GameObject> Bricks;

    GameLevel() {};

    void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
    void Draw(SpriteRender &renderer);

    // 检测每一个关卡是否完成
    GLboolean IsCompeted();

private:
    void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif //GAME_GAMELEVEL_HPP
