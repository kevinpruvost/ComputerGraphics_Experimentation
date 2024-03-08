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

    virtual ErrorCode CloseWindow() override;
    virtual ErrorCode MinimizeWindow() override;

    virtual ErrorCode SetWindowSize(int width, int height) override;
    virtual ErrorCode SetWindowPosition(int x, int y) override;
    virtual ErrorCode SetWindowTitle(const char* title) override;
    virtual ErrorCode SetWindowIcon(const std::filesystem::path & iconPath) override;
    virtual ErrorCode SetWindowVSync(VSyncModes mode) override;
    virtual ErrorCode SetWindowFullscreen(bool enabled, int monitorIndex) override;
    virtual ErrorCode SetWindowBorderless(bool enabled) override;
    virtual ErrorCode SetWindowVideoMode(const VideoMode& videoMode, int monitorIndex = 0) override;
    virtual ErrorCode SetWindowResizable(bool enabled) override;
    virtual ErrorCode SetWindowFocused(bool focused) override;

protected:
    virtual ErrorCode _Init() override;

private:
    GLFWwindow * __mainW;
    GLFWvidmode * __mainMode;
    std::vector<GLFWwindow *> __windows;
    std::vector<GLFWvidmode> __modes;
};

extern "C"
{
    EXPORT Window* createWindowInstance();
}