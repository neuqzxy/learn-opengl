#ifndef SPRIT_RENDER_HPP
#define SPRIT_RENDER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

class SpriteRender {
public:
    explicit SpriteRender(Shader &shader);
    ~SpriteRender();

    void DrawSprite(const Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
                    GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader shader;
    GLuint quadVAO;

    void initRenderData();
};

#endif
