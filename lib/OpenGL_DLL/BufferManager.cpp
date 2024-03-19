#include "BufferManager.h"

#include <vector>
#include <array>

std::vector<std::array<GLuint, BufferManager::BlockSize>> VBOs, VAOs, EBOs;
uint32_t VBOIndex = 0, VAOIndex = 0, EBOIndex = 0;

static void AddVBOs()
{
    std::array<GLuint, BufferManager::BlockSize> vbos;
    glGenBuffers(BufferManager::BlockSize, vbos.data());
    VBOs.push_back(vbos);
}

static void AddVAOs()
{
    std::array<GLuint, BufferManager::BlockSize> vaos;
    glGenVertexArrays(BufferManager::BlockSize, vaos.data());
    VAOs.push_back(vaos);
}

static void AddEBOs()
{
    std::array<GLuint, BufferManager::BlockSize> ebos;
    glGenBuffers(BufferManager::BlockSize, ebos.data());
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
    {
        VBOs.push_back({});
        glGenBuffers(BufferManager::BlockSize, VBOs.back().data());
    }
    return VBOs[VBOIndex / BufferManager::BlockSize][VBOIndex % BufferManager::BlockSize];
}

GLuint BufferManager::GetVAO()
{
    ++VAOIndex;
    if (VAOIndex >= BufferManager::BlockSize * VAOs.size())
    {
        VAOs.push_back({});
        glGenVertexArrays(BufferManager::BlockSize, VAOs.back().data());
    }
    return VAOs[VAOIndex / BufferManager::BlockSize][VAOIndex % BufferManager::BlockSize];
}

GLuint BufferManager::GetEBO()
{
    ++EBOIndex;
    if (EBOIndex >= BufferManager::BlockSize * EBOs.size())
    {
        EBOs.push_back({});
        glGenBuffers(BufferManager::BlockSize, EBOs.back().data());
    }
    return EBOs[EBOIndex / BufferManager::BlockSize][EBOIndex % BufferManager::BlockSize];
}
