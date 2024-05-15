#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>

class Application : public ApplicationTools
{
public:
    Application();
    Venom::ErrorCode Update() override;
    void Terminate();

    ~Application();

private:
};
