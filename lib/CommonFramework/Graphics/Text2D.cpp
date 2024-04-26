#pragma once

#include <common/Text2D.h>
#include <common/Engine/EngineLoader.h>

typedef Text2D* (*CreateText2DFn)();

Text2D* Text2D::CreateText2D(const char* fontPath, int fontSize)
{
    CreateText2DFn createText2DFn = EngineLoader::GetEngineDll()->getFunction<CreateText2DFn>("createText2D");
    if (createText2DFn == nullptr)
        throw DLLException("Failed to load createText2D function from engine dll");
    Text2D* Text2D = createText2DFn();
    if (Text2D == nullptr)
        throw DLLException("Failed to create Text2D");
    Text2D->InitText2D(fontPath, fontSize);
    return Text2D;
}
