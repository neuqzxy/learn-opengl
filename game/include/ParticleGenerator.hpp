//
// Created by 周欣宇 on 2019-09-05.
//

#ifndef GAME_PARTICLEGENERATOR_HPP
#define GAME_PARTICLEGENERATOR_HPP

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "Shader.hpp"
#include "Texture.hpp"
#include "GameObject.hpp"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    void init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif //GAME_PARTICLEGENERATOR_HPP
