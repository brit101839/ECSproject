#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>
#include <glm/glm.hpp>

struct Character {
    GLuint TextureID;    // 字符紋理 ID
    glm::ivec2 Size;     // 字符大小
    glm::ivec2 Bearing;  // 字符起始位置
    GLuint Advance;      // 字符偏移量
};

std::map<GLchar, Character> Characters;

void initFont(const char* fontPath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not initialize FreeType library\n";
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font\n";
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);  // 設置字體大小

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // 禁用字節對齊限制

    // 遍歷所有 ASCII 字符並加載到內存
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load character " << c << "\n";
            continue;
        }

        // 創建字體紋理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // 設置紋理選項
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 保存字符數據
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
