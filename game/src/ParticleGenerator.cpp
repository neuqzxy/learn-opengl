//
// Created by 周欣宇 on 2019-09-05.
//
#include "ParticleGenerator.hpp"
#include <random>

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
        : shader(shader), texture(texture), amount(amount) {
    this->init();
}

void ParticleGenerator::init() {
    // 设置VBO并绑定到VAO中
    GLuint VBO;
    GLfloat particle_quad[]{
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    for (int i = 0; i < this->amount; i++) {
        this->particles.emplace_back(Particle());
    }
}

void ParticleGenerator::Draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (auto particle : this->particles) {
        if (particle.Life > 0.0f) {
            this->shader.setVector2f("offset", particle.Position);
            this->shader.setVector4f("color", particle.Color);
            this->texture.Bind();
            // 使用ParticleGenerator的VAO进行绘制6个顶点
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
static GLuint renderedNum = 0;
std::random_device rd;
void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset) {
    GLfloat percent = 100.0f - GLfloat(renderedNum) / particles.size() * 100.0f;
    if ((rd() % 100) < percent) {
        // Add new particles
        for (GLuint i = 0; i < newParticles; ++i)
        {
            renderedNum++;
            int unusedParticle = this->firstUnusedParticle();
            this->respawnParticle(this->particles[unusedParticle], object, offset);
        }
    }
    // Update all particles
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        if (p.Life > 0.0f) {
            p.Life -= dt; // reduce life
            if (p.Life <= 0.0f) {
                renderedNum--;
            }
        }
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Color.a -= dt * 2.5;
        }
    }
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset) {
    GLfloat random = ((rd() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rd() % 100) / 100.0f);
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}
