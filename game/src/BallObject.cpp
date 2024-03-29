//
// Created by 周欣宇 on 2019-09-04.
//
#include "BallObject.hpp"

BallObject::BallObject(): GameObject(), Radius(12.5f), Stuck(true) {}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3{1.0f}, velocity), Radius(radius), Stuck(true) { }

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width) {
    // If not stuck to player board
    if (!Stuck)
    {
        // Move the ball
        Position += Velocity * dt;
        // Then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (Position.x <= 0.0f) {
            Velocity.x = -Velocity.x;
            Position.x = 0.0f;
        } else if (Position.x + Size.x >= window_width) {
            Velocity.x = -Velocity.x;
            Position.x = window_width - Size.x;
        }

        if (Position.y <= 0.0f) {
            Velocity.y = -Velocity.y;
            Position.y = 0.0f;
        }
    }
    return Position;
}


void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
    Position = position;
    Velocity = velocity;
    Stuck = true;
}
