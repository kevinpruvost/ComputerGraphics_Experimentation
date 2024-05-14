#pragma once

#include <common/ParticleSystem.h>

class ParticleSystem_OGL : public ParticleSystem
{
};

extern "C"
{
    EXPORT ParticleSystem* createParticleSystem();
}