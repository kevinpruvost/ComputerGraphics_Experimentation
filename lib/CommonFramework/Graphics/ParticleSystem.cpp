#include <common/ParticleSystem.h>

constexpr const float defaultFloat = -999.0f;

ParticleSystem::ParticleSystem()
    : __timeSinceLastEmission { 0.0f }
    , __paused{ false }
    , __particleColor(1.0f, 1.0f, 1.0f, 1.0f)
    , __particleLifetime(defaultFloat)
    , __particleSize(defaultFloat)
    , __maxParticles((int)defaultFloat)
    , __emissionRate(defaultFloat)
    , __particleInitialVelocity(defaultFloat)
    , __particleAcceleration(defaultFloat)
    , __particleTexture(nullptr)
    , __particleShaderPipeline(nullptr)
    , __emitterPosition(defaultFloat)
    , __model(nullptr)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Draw() { RenderParticles(); }
void ParticleSystem::Update(float deltaTime)
{
    assert(__particleTexture != nullptr);
    assert(__particleShaderPipeline != nullptr);
    assert(__particleLifetime != defaultFloat);
    assert(__particleSize != defaultFloat);
    assert(__maxParticles != (int)defaultFloat);
    assert(__emissionRate != defaultFloat);
    assert(__particleInitialVelocity != glm::vec3(defaultFloat));
    assert(__particleAcceleration != glm::vec3(defaultFloat));
    assert(__emitterPosition != glm::vec3(defaultFloat));

    for (auto particle = __particles.begin(); particle != __particles.end(); ++particle)
    {
        particle->lifetime -= deltaTime;
        while (particle->lifetime <= 0.0f)
        {
            particle = __particles.erase(particle);
            if (particle == __particles.end())
                break;
            particle->lifetime -= deltaTime;
        }

        particle->position += particle->velocity * deltaTime;
        particle->velocity += particle->acceleration * deltaTime;
    }

    __timeSinceLastEmission += deltaTime;
    while (__timeSinceLastEmission >= 1.0f / __emissionRate)
    {
        AddParticle(deltaTime);
        __timeSinceLastEmission -= 1.0f / __emissionRate;
    }

    RenderParticles();
}

void ParticleSystem::AddParticle(const float deltaTime)
{
    if (__particles.size() >= __maxParticles) return;

    Particle particle = {
        __emitterPosition,
        __particleInitialVelocity,
        __particleAcceleration,
        __particleColor,
        __particleLifetime,
        __particleSize
    };
    __particles.push_back(particle);
}

void ParticleSystem::RenderParticles()
{
    Camera * camera = __camera ? __camera : Camera::MainCamera;
    if (__model == nullptr) __model = Model::CreateSquareModel();
    Model * model = __model;

    __particleShaderPipeline->Use();

    __particleTexture->BindTexture();
    __particleShaderPipeline->SetUniformMatrix4("view", camera->GetViewMatrix());
    __particleShaderPipeline->SetUniformMatrix4("projection", camera->GetProjectionMatrix());
    __particleShaderPipeline->SetUniformInt("textureSampler", 0);
    for (const auto& particle : __particles)
    {
        if (_drawMode & Drawable3D::DrawMode::SOLID)
        {

        }

    }
}

void ParticleSystem::Pause()
{
    __paused = !__paused;
}

void ParticleSystem::Stop()
{
    __paused = true;
    __particles.clear();
    __timeSinceLastEmission = 0.0f;
}

void ParticleSystem::Restart()
{
    __paused = false;
    __particles.clear();
    __timeSinceLastEmission = 0.0f;
}

void ParticleSystem::SetEmitterPosition(const glm::vec3& position) { __emitterPosition = position; }
void ParticleSystem::SetMaxParticles(int maxParticles) { __maxParticles = maxParticles; }
void ParticleSystem::SetParticleSize(float size) { __particleSize = size;}
void ParticleSystem::SetParticleTexture(Texture* texture) { __particleTexture = texture; }
void ParticleSystem::SetParticleShaderPipeline(ShaderPipeline* shader) { __particleShaderPipeline = shader; }
void ParticleSystem::SetParticleColor(const glm::vec4& color) { __particleColor = color; }
void ParticleSystem::SetParticleLifetime(float lifetime) { __particleLifetime = lifetime; }
void ParticleSystem::SetParticleInitialVelocity(const glm::vec3& velocity) { __particleInitialVelocity = velocity; }
void ParticleSystem::SetParticleAcceleration(const glm::vec3& acceleration) { __particleAcceleration = acceleration; }
void ParticleSystem::SetEmissionRate(float rate) { __emissionRate = rate; }
void ParticleSystem::SetCamera(Camera* camera) { __camera = camera; }
