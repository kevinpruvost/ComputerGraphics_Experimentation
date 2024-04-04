#pragma once

#include <common/WindowSettings.h>
#include <common/InputSystem.h>
#include <array>

/// <summary>
/// Window class common to all 3 APIs, managed with GLFW for now
/// </summary>
class COMMONFRAMEWORK_API Window : public InputSystem
{
public:
	~Window();

    static Window * CreateWindowFromAPI(const WindowAPI & api);
	
	ErrorCode Init(const Config & config);
	virtual ErrorCode _Init() = 0;

	virtual ErrorCode Loop() = 0;

	// Window management
	virtual ErrorCode CloseWindow() = 0;
	virtual ErrorCode MinimizeWindow() = 0;

	virtual ErrorCode SetWindowSize(int width, int height) = 0;
	virtual ErrorCode SetWindowPosition(int x, int y) = 0;
	virtual ErrorCode SetWindowTitle(const char * title) = 0;
	virtual ErrorCode SetWindowIcon(const std::filesystem::path & iconPath) = 0;
	enum class VSyncModes { SingleBuffer = 0, DoubleBuffer = 1, TripleBuffer = 2 };
	virtual ErrorCode SetWindowVSync(VSyncModes mode) = 0;

	struct VideoMode
	{
        int width;
        int height;
        int refreshRate;
		int bitsPerPixel;
    };
	virtual ErrorCode SetWindowFullscreen(bool enabled, int monitorIndex = 0) = 0;
	virtual ErrorCode SetWindowBorderless(bool enabled) = 0;
	virtual ErrorCode SetWindowVideoMode(const VideoMode & videoMode, int monitorIndex = 0) = 0;

	virtual ErrorCode SetWindowResizable(bool enabled) = 0;
	virtual ErrorCode SetWindowFocused(bool focused) = 0;

	void SetSceneLoopCallback(CallbackContainer<void> callback);
	void SetSceneLoopCallback(Callback<void> callback);

	void SetApplicationLoopCallback(CallbackContainer<void> callback);
	void SetApplicationLoopCallback(Callback<void> callback);

	virtual int GetWindowWidth() const = 0;
	virtual int GetWindowHeight() const = 0;
	virtual std::array<int, 2> GetWindowSize() const = 0;

protected:
	Window(const WindowAPI & api);

protected:
	WindowSettings _settings;
	Callback<void> _sceneLoopCallback;
	Callback<void> _appLoopCallback;

private:
	const WindowAPI __api;
	static UPtr<DLL> __dll;
};

