#include <common/BaseFramework.h>

BaseFramework::BaseFramework()
{
}

void BaseFramework::SetConfig(const Config& config)
{
    _app->SetConfig(config);
}

void BaseFramework::SetWindow(Window * w)
{
    _app->SetWindow(w);
}