#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>

class Application : public ApplicationTools
{
public:
    Application();
    ErrorCode Run() override;
    ErrorCode Initialize();
    ErrorCode Loop();
    void Terminate();

    ~Application();

private:
};
