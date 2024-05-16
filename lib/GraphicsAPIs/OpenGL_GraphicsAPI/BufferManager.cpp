#include "BufferManager.h"

#include <common/Exception.h>

#include <vector>
#include <array>

std::vector<std::array<GLuint, BufferManager::BlockSize>> VBOs, VAOs, EBOs;
uint32_t VBOIndex = 0, VAOIndex = 0, EBOIndex = 0;

static inline void AddVBOs()
{
    std::array<GLuint, BufferManager::BlockSize> vbos;
    glGenBuffers(BufferManager::BlockSize, vbos.data());
    if (vbos[0] == GL_INVALID_VALUE)
        throw RuntimeException("Failed to generate VBOs");
    VBOs.push_back(vbos);
}

static inline void AddVAOs()
{
    std::array<GLuint, BufferManager::BlockSize> vaos;
    glGenVertexArrays(BufferManager::BlockSize, vaos.data());
    if (vaos[0] == GL_INVALID_VALUE)
        throw RuntimeException("Failed to generate VAOs");
    VAOs.push_back(vaos);
}

static inline void AddEBOs()
{
    std::array<GLuint, BufferManager::BlockSize> ebos;
    glGenBuffers(BufferManager::BlockSize, ebos.data());
    if (ebos[0] == GL_INVALID_VALUE)
        throw RuntimeException("Failed to generate EBOs");
    EBOs.push_back(ebos);
}

void BufferManager::Initialize()
{
    AddVBOs();
    AddVAOs();
    AddEBOs();
}

void BufferManager::Destroy()
{
    for (auto& vao : VAOs)
        glDeleteVertexArrays(BufferManager::BlockSize, vao.data());
    for (auto& vbo : VBOs)
        glDeleteBuffers(BufferManager::BlockSize, vbo.data());
    for (auto& ebo : EBOs)
        glDeleteBuffers(BufferManager::BlockSize, ebo.data());
}

GLuint BufferManager::GetVBO()
{
    ++VBOIndex;
    if (VBOIndex >= BufferManager::BlockSize * VBOs.size())
        AddVBOs();
    return VBOs[VBOIndex / BufferManager::BlockSize][VBOIndex % BufferManager::BlockSize];
}

GLuint BufferManager::GetVAO()
{
    ++VAOIndex;
    if (VAOIndex >= BufferManager::BlockSize * VAOs.size())
        AddVAOs();
    return VAOs[VAOIndex / BufferManager::BlockSize][VAOIndex % BufferManager::BlockSize];
}

GLuint BufferManager::GetEBO()
{
    ++EBOIndex;
    if (EBOIndex >= BufferManager::BlockSize * EBOs.size())
        AddEBOs();
    return EBOs[EBOIndex / BufferManager::BlockSize][EBOIndex % BufferManager::BlockSize];
}
