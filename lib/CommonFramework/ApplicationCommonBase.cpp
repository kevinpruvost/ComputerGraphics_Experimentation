#include <common/ApplicationCommonBase.h>

ApplicationCommonBase::ApplicationCommonBase()
{

}

ApplicationCommonBase::~ApplicationCommonBase()
{

}

void ApplicationCommonBase::SetConfig(const Config & config)
{
    _engineSettings = config.EngineSettings();
}

void ApplicationCommonBase::SetWindow(Window* w)
{
    _w = w;
}
