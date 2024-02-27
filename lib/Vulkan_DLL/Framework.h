#pragma once
#include <common/BaseFramework.h>

class Framework : public BaseFramework
{
public:
    virtual bool Init() override;
    virtual void Destroy() override;
    virtual void Launch() override;
};

