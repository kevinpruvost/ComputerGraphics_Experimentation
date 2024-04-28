#pragma once

#include "Model.h"
#include "Texture.h"
#include "Drawable3D.h"
#include "ShaderPipeline.h"
#include "Camera.h"
#include "Scene.h"

struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec4 color;
    float lifetime;
    float size;
};

class ParticleSystem : public Drawable3D
{
public:
    ParticleSystem();
    ~ParticleSystem();

    static ParticleSystem * CreateParticleSystem();

    void Draw() override;

    // Updates the particle system based on the time passed
    void Update(float deltaTime);

    // Pauses/Resumes the particle system
    void Pause();
    inline void Resume() { Pause(); }

    // Stops the particle system
    void Stop();

    // Restarts the particle system
    void Restart();

    // Function to set the position of the emitter
    void SetEmitterPosition(const glm::vec3& position);

    // Function to set the maximum number of particles
    void SetMaxParticles(int maxParticles);

    // Function to set the particle size
    void SetParticleSize(float size);

    // Function to set the texture for rendering particles
    void SetParticleTexture(Texture* texture);

    // Function to set the texture for rendering particles
    void SetParticleShaderPipeline(ShaderPipeline * shader);

    // Function to set the color of the particles
    void SetParticleColor(const glm::vec4& color);

    // Function to set the lifetime of the particles
    void SetParticleLifetime(float lifetime);

    // Function to set the velocity of the particles
    void SetParticleInitialVelocity(const glm::vec3& velocity);
    
    // Function to set the acceleration of the particles
    void SetParticleAcceleration(const glm::vec3& acceleration);

    // Function to set frequency of particle emission (per second)
    void SetEmissionRate(float rate);

    // Set Camera
    void SetCamera(Camera * camera);

    // Set Particle Generation Function
    void SetParticleGenerationFunction(CallbackContainer<void, ParticleSystem *, const float> generationFunction);

    // Get Position
    glm::vec3 GetEmitterPosition() const;
    int GetMaxParticles() const;
    float GetParticleSize() const;
    Texture* GetParticleTexture() const;
    ShaderPipeline* GetParticleShaderPipeline() const;
    glm::vec4 GetParticleColor() const;
    float GetParticleLifetime() const;
    glm::vec3 GetParticleInitialVelocity() const;
    glm::vec3 GetParticleAcceleration() const;
    float GetEmissionRate() const;
    Camera* GetCamera() const;

    Particle GetDefaultParticle() const;
    void EmitParticle(const Particle & particle);
    void EmitParticle();
private:
    friend class Scene;

    // Function to add a new particle to the system
    void AddParticle(const float deltaTime);
    // Renders the particles
    void RenderParticles();

    Callback<void, ParticleSystem *, const float> __particleGenerationFunction;
    ShaderPipeline * __particleShaderPipeline;

    glm::vec3 __emitterPosition;

    std::vector<Particle> __particles;
    int __maxParticles;

    float __particleSize;
    glm::vec4 __particleColor;
    float __particleLifetime;
    glm::vec3 __particleInitialVelocity;
    glm::vec3 __particleAcceleration;
    float __emissionRate;
    float __timeSinceLastEmission;

    Texture* __particleTexture;
    Model* __model;

    Camera * __camera;

    bool __paused;
};