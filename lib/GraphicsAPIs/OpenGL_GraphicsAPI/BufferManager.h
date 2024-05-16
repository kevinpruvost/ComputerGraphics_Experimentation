#pragma once

#include <glad/glad.h>

class BufferManager
{
public:
    static void Initialize();
    static void Destroy();

    static GLuint GetVBO();
    static GLuint GetVAO();
    static GLuint GetEBO();
    const static int BlockSize = 0b1000000;
};

