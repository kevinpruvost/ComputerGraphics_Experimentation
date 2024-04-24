#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>

class Application : public ApplicationTools
{
public:
    Application();
    Venom::ErrorCode Run() override;
    Venom::ErrorCode Initialize();
    Venom::ErrorCode Loop();
    void Terminate();

    ~Application();

private:
};
