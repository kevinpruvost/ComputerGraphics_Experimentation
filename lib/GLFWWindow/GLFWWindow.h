#pragma once

#include <vector>
#include <common/Window.h>
#include <GLFW/glfw3.h>

class CONTEXT_API GLFWWindow : public Window
{
public:
    GLFWWindow();
    ~GLFWWindow();

    ErrorCode Loop() override;

    ErrorCode Destroy();

    ErrorCode CloseWindow() override;
    ErrorCode MinimizeWindow() override;

    ErrorCode SetWindowSize(int width, int height) override;
    ErrorCode SetWindowPosition(int x, int y) override;
    ErrorCode SetWindowTitle(const char* title) override;
    ErrorCode SetWindowIcon(const std::filesystem::path & iconPath) override;
    ErrorCode SetWindowVSync(VSyncModes mode) override;
    ErrorCode SetWindowFullscreen(bool enabled, int monitorIndex) override;
    ErrorCode SetWindowBorderless(bool enabled) override;
    ErrorCode SetWindowVideoMode(const VideoMode& videoMode, int monitorIndex = 0) override;
    ErrorCode SetWindowResizable(bool enabled) override;
    ErrorCode SetWindowFocused(bool focused) override;

    int GetWindowWidth() const override;
    int GetWindowHeight() const override;
    std::array<int, 2> GetWindowSize() const override;

    void LockCursor() override;
    void UnlockCursor() override;

    GLFWwindow* GetWindow();

protected:
    ErrorCode _Init() override;

private:
    InputSystem::Key __TranslateKey(int key);
    InputSystem::KeyModifier __TranslateKeyModifier(int shiftState, int ctrlState, int altState);
    InputSystem::KeyModifier __TranslateKeyModifier(int state);
    InputSystem::MouseButton __TranslateMouseButton(int button);
    bool ProcessInput();

private:
    GLFWwindow * __mainW;
    GLFWvidmode * __mainMode;
    std::vector<GLFWwindow *> __windows;
    std::vector<GLFWvidmode> __modes;
};

extern "C"
{
    CONTEXT_API Window* createWindowInstance();
}