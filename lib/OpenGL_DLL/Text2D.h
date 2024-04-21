#pragma once

#include <glad/glad.h>
#include <common/Text2D.h>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class OGL_Text2D : public Text2D
{
public:
    void RenderText(const char* text, float x, float y, float scale, const glm::vec3& color) override;
    void InitText2D(const char* fontPath, int fontSize) override;
    void LoadCharacter(char character);

private:
    FT_Library ft;
    FT_Face face;
    GLuint VAO, VBO;
    int screenWidth, screenHeight;

    struct Character {
        GLuint textureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };
    std::map<GLchar, Character> characters;
};

extern "C"
{
    EXPORT Text2D * createText2D();
}