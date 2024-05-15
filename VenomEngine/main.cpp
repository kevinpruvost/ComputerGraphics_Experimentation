#include <common/VenomEngine.h>

#include "Scene.h"

int main()
{
	VenomEngine engine;
	Venom::ErrorCode err;
	if ((err = engine.LoadEngine("Config.yaml")) != Venom::ErrorCode::Success)
        return (int)err;
	if ((err = engine.LoadScene<MainScene>()) != Venom::ErrorCode::Success)
		return (int)err;
	if ((err = engine.Init()) != Venom::ErrorCode::Success)
        return (int)err;
	if ((err = engine.Run()) != Venom::ErrorCode::Success)
		return (int)err;
	return (int)Venom::ErrorCode::Success;
}