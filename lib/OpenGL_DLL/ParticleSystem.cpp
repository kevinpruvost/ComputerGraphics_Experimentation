#include "ParticleSystem.h"

Venom::ErrorCode ParticleSystem_OGL::ReloadObjectFromEngine()
{
    return Venom::ErrorCode::Success;
}

// Export the factory function to create an instance of the class
EXPORT ParticleSystem* createParticleSystem() {
    return new ParticleSystem_OGL();
}