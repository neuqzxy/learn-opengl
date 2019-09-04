//
// Created by 周欣宇 on 2019-08-25.
//
#include <fstream>
#include <sstream>
#include "GameLevel.hpp"
#include "ResourceManager.hpp"
#include "SpriteRender.hpp"

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight) {
    Bricks.clear();
    GLuint tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> tileData;

    if (fstream) {
        while (getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<GLuint> row;

            // 读取空格分隔的字
            while (sstream >> tileCode) {
                row.push_back(tileCode);
            }
            if (!row.empty()) tileData.push_back(row);
        }
        if (!tileData.empty()) {
            init(tileData, levelWidth, levelHeight);
        }
    }
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
    // 计算每个维度的大小
    GLuint rowNum = tileData.size();
    GLuint columnNum = tileData[0].size();
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(columnNum);
    GLfloat unit_height = levelHeight / rowNum;
    // 基于tileDataC初始化关卡
    for (GLuint y = 0; y < rowNum; ++y)
    {
        for (GLuint x = 0; x < columnNum; ++x)
        {
            // 检查砖块类型
            if (tileData[y][x] == 1)
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size,
                               ResourceManager::GetTexture("block_solid"),
                               glm::vec3(0.8f, 0.8f, 0.7f)
                );
                obj.IsSolid = GL_TRUE;
                Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec3 color = glm::vec3(1.0f); // 默认为白色
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Bricks.emplace_back(
                        GameObject(pos, size, ResourceManager::GetTexture("block"), color)
                );
            }
        }
    }
}

void GameLevel::Draw(SpriteRender &renderer)
{
    for (GameObject &brick : Bricks)
        if (!brick.Destroyed)
            brick.Draw(renderer);
}
