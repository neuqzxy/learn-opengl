//
// Created by 周欣宇 on 2019-08-25.
//
#include "GameObject.hpp"

GameObject::GameObject(): Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity): Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {}

void GameObject::Draw(SpriteRender &renderer) {
    renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}
