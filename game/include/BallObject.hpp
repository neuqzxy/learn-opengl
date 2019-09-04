//
// Created by 周欣宇 on 2019-09-04.
//

#ifndef GAME_BALLOBJECT_HPP
#define GAME_BALLOBJECT_HPP

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include "Texture.hpp"
#include "GameObject.hpp"

class BallObject: public GameObject {
public:
    GLfloat Radius;
    GLboolean Stuck;

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif //GAME_BALLOBJECT_HPP
