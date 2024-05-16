#pragma once

#include <common/Components/ParticleSystem.h>

class ParticleSystem_OGL : public ParticleSystem
{
};

extern "C"
{
    EXPORT ParticleSystem* createParticleSystem();
}