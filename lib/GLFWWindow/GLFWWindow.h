#pragma once

#include <vector>
#include <common/Window.h>
#include <GLFW/glfw3.h>

class GLFWWindow : public Window
{
public:
    GLFWWindow();
    ~GLFWWindow();

    virtual ErrorCode Loop() override;

    ErrorCode Destroy();

protected:
    virtual ErrorCode _Init() override;

private:
    GLFWwindow * __mainW;
    std::vector<GLFWwindow *> __windows;
};

extern "C"
{
    EXPORT Window* createWindowInstance();
}