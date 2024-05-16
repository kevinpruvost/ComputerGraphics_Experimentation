#include <common/BaseFramework.h>

BaseFramework::BaseFramework()
{
}

BaseFramework::~BaseFramework()
{
}

void BaseFramework::SetConfig(const Config& config)
{
    _app->SetConfig(config);
}