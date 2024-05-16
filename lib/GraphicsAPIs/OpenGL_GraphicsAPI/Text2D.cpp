#include "Text2D.h"

#include <common/Rendering.h>

// Export the factory function to create an instance of the class
EXPORT Text2D * createText2D() {
    return new OGL_Text2D();
}

void OGL_Text2D::RenderText(const char* text, float x, float y, float scale, const glm::vec3& color)
{
    assert(text);
    // Activate corresponding render state
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Rendering::SetDrawMode(Drawable3D::DrawMode::SOLID);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    for (const char* c = text; *c; ++c) {
        Character ch = characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale; // Adjusted y-coordinate

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos,     ypos,       0.0, 0.0 },
            { xpos + w, ypos,       1.0, 0.0 },

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos + w, ypos + h,   1.0, 1.0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor position
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
}

void OGL_Text2D::LoadCharacter(char character) {
    if (FT_Load_Char(face, character, FT_LOAD_RENDER))
        throw std::runtime_error("Failed to load glyph");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    characters.insert(std::pair<GLchar, Character>(character, {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<GLuint>(face->glyph->advance.x)
        }));
}

void OGL_Text2D::InitText2D(const char* fontPath, int fontSize)
{
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("Failed to initialize FreeType library");

    if (FT_New_Face(ft, fontPath, 0, &face))
        throw std::runtime_error("Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; ++c) {
        LoadCharacter(c);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Set up vertex data and buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
