#include <common/VenomEngine.h>
#include "Scene.h"

Venom::ErrorCode VenomMain()
{
	Venom::ErrorCode err;
	VenomEngine engine;
	engine.SetConfigName("Config.yaml");
	engine.SetScene<MainScene>();
	if ((err = engine.LaunchEngine()) != Venom::ErrorCode::Success) {
		Logger::Print("VenomEngine Fatal Error.");
		return err;
	}
	return Venom::ErrorCode::Success;
}

int main()
{
    return static_cast<int>(VenomMain());
}