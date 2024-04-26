#pragma once

#include <common/ParticleSystem.h>

class ParticleSystem_OGL : public ParticleSystem
{
    Venom::ErrorCode ReloadObjectFromEngine() override;
};