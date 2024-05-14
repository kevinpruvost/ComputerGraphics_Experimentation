#include "ParticleSystem.h"


// Export the factory function to create an instance of the class
EXPORT ParticleSystem* createParticleSystem() {
    return new ParticleSystem_OGL();
}