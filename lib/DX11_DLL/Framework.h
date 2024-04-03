#pragma once
#include <common/BaseFramework.h>

class Framework : public BaseFramework
{
public:
    ~Framework();
    bool Init() override;
    void Destroy() override;
    int Launch() override;
    Logger* GetLogger() override;
    Config* GetConfig() override;
};

extern "C"
{
    EXPORT BaseFramework* createFrameworkInstance();
}