#pragma once

#include <vector>
#include <common/Window.h>
#include <GLFW/glfw3.h>

class VENOM_CONTEXT_API GLFW_ContextAPI : public ContextAPI
{
public:
    GLFW_ContextAPI();
    ~GLFW_ContextAPI();

    Venom::ErrorCode Update() override;

    Venom::ErrorCode Destroy();

    Venom::ErrorCode CloseWindow() override;
    Venom::ErrorCode MinimizeWindow() override;

    Venom::ErrorCode SetWindowSize(int width, int height) override;
    Venom::ErrorCode SetWindowPosition(int x, int y) override;
    Venom::ErrorCode SetWindowTitle(const char* title) override;
    Venom::ErrorCode SetWindowIcon(const std::filesystem::path & iconPath) override;
    Venom::ErrorCode SetWindowVSync(VSyncModes mode) override;
    Venom::ErrorCode SetWindowFullscreen(bool enabled, int monitorIndex) override;
    Venom::ErrorCode SetWindowBorderless(bool enabled) override;
    Venom::ErrorCode SetWindowVideoMode(const VideoMode& videoMode, int monitorIndex = 0) override;
    Venom::ErrorCode SetWindowResizable(bool enabled) override;
    Venom::ErrorCode SetWindowFocused(bool focused) override;

    int GetWindowWidth() const override;
    int GetWindowHeight() const override;
    std::array<int, 2> GetWindowSize() const override;

    void LockCursor() override;
    void UnlockCursor() override;

    GLFWwindow* GetWindow();

protected:
    Venom::ErrorCode _Init() override;

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
    VENOM_CONTEXT_API ContextAPI* createContextAPIInstance();
}