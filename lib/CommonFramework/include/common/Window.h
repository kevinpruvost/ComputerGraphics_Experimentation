#pragma once

#include <common/Config.h>
#include <common/Logger.h>
#include <common/Exception.h>
#include <common/DLL.h>
#include <common/Callback.h>

/// <summary>
/// Window class common to all 3 APIs, managed with GLFW for now
/// </summary>
class Window
{
public:
	enum class WindowAPI : int
	{
		GLFW = 0,
		WindowsNative = 1
	};

	~Window();

    static Window * CreateWindowFromAPI(const WindowAPI & api);
	
	ErrorCode Init(const Config & config);
	virtual ErrorCode _Init() = 0;

	virtual ErrorCode Loop() = 0;

	void SetLoopCallback(CallbackContainer<void> callback);
	void SetLoopCallback(Callback<void> callback);

protected:
	Window(const WindowAPI & api);

protected:
	WindowSettings _settings;
	Callback<void> _loopCallback;

private:
	const WindowAPI __api;
	static DLL * __dll;
};

