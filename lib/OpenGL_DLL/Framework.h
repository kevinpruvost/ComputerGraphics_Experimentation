#pragma once
#include <common/BaseFramework.h>
#include <common/Memory.h>

class Framework : public BaseFramework
{
public:
    ~Framework();
    bool Init() override;
    void Destroy() override;
    int Launch() override;
    Logger* GetLogger() override;
    Config* GetConfig() override;
private:
};

extern "C"
{
    EXPORT BaseFramework* createFrameworkInstance();
}