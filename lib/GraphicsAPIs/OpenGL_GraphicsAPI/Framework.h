#pragma once
#include <common/BaseFramework.h>
#include <common/Memory.h>

class Framework : public BaseFramework
{
public:
    ~Framework();
    Venom::ErrorCode Init() override;
    Venom::ErrorCode Update() override;
    void Destroy() override;
    Logger* GetLogger() override;
    Config* GetConfig() override;
private:
};

extern "C"
{
    EXPORT BaseFramework* createFrameworkInstance();
}