#pragma once

#include <common/WindowSettings.h>
#include <common/InputSystem.h>
#include <array>

class Scene;

/**
 * @brief Window class common to all 3 APIs, managed with GLFW for now
 */
class COMMONFRAMEWORK_API Window : public InputSystem
{
public:
	~Window();

    static Window * CreateWindowFromAPI(const WindowAPI & api);
	
	Venom::ErrorCode Init(const Config * config);
	virtual Venom::ErrorCode _Init() = 0;

	virtual Venom::ErrorCode Loop() = 0;

	// Window management
	virtual Venom::ErrorCode CloseWindow() = 0;
	virtual Venom::ErrorCode MinimizeWindow() = 0;

	virtual Venom::ErrorCode SetWindowSize(int width, int height) = 0;
	virtual Venom::ErrorCode SetWindowPosition(int x, int y) = 0;
	virtual Venom::ErrorCode SetWindowTitle(const char * title) = 0;
	virtual Venom::ErrorCode SetWindowIcon(const std::filesystem::path & iconPath) = 0;
	enum class VSyncModes { SingleBuffer = 0, DoubleBuffer = 1, TripleBuffer = 2 };
	virtual Venom::ErrorCode SetWindowVSync(VSyncModes mode) = 0;

	struct VideoMode
	{
        int width;
        int height;
        int refreshRate;
		int bitsPerPixel;
    };
	virtual Venom::ErrorCode SetWindowFullscreen(bool enabled, int monitorIndex = 0) = 0;
	virtual Venom::ErrorCode SetWindowBorderless(bool enabled) = 0;
	virtual Venom::ErrorCode SetWindowVideoMode(const VideoMode & videoMode, int monitorIndex = 0) = 0;

	virtual Venom::ErrorCode SetWindowResizable(bool enabled) = 0;
	virtual Venom::ErrorCode SetWindowFocused(bool focused) = 0;

	void SetScene(UPtr<Scene> * scene);

	void SetApplicationLoopCallback(CallbackContainer<void> callback);

	void SetFramebufferSizeCallback(CallbackContainer<void, int, int> callback);

	virtual int GetWindowWidth() const = 0;
	virtual int GetWindowHeight() const = 0;
	virtual std::array<int, 2> GetWindowSize() const = 0;

	virtual void LockCursor() = 0;
	virtual void UnlockCursor() = 0;

protected:
	Window(const WindowAPI & api);

protected:
	WindowSettings _settings;
	Callback<void> _appLoopCallback;
	// Framebuffer size callback
	Callback<void, int, int> _framebufferSizeCallback;
	UPtr<Scene> * _scene;

private:
	const WindowAPI __api;
	static UPtr<DLL> __dll;
};

