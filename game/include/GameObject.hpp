//
// Created by 周欣宇 on 2019-08-25.
//

#ifndef GAME_GAMEOBJECT_HPP
#define GAME_GAMEOBJECT_HPP

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Texture.hpp"
#include "SpriteRender.hpp"

// 保存sprite的位置大小等信息
class GameObject {
public:
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    Texture2D Sprite;

    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRender &renderer);
};

#endif //GAME_GAMEOBJECT_HPP
