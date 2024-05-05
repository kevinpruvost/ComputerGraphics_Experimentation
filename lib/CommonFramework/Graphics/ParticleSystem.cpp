#include <common/ParticleSystem.h>

#include <common/Engine/EngineLoader.h>
#include <common/Rendering.h>

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
    , __particleGenerationFunction(nullptr)
{
}

ParticleSystem::~ParticleSystem()
{
}

typedef ParticleSystem* (*CreateParticleSystemFn)();

ParticleSystem* ParticleSystem::CreateParticleSystem()
{
    CreateParticleSystemFn createParticleSystemFn = EngineLoader::GetEngineDll()->getFunction<CreateParticleSystemFn>("createParticleSystem");
    assert(createParticleSystemFn != nullptr);
    ParticleSystem* shader = createParticleSystemFn();
    assert(shader != nullptr);
    return shader;
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
        if (__particles.size() < __maxParticles)
        {
            if (__particleGenerationFunction != nullptr)
                __particleGenerationFunction(this, deltaTime);
            else
                // Add a particle (default behavior
                AddParticle(deltaTime);
        }
        __timeSinceLastEmission -= 1.0f / __emissionRate;
    }

    RenderParticles();
}

Particle ParticleSystem::GetDefaultParticle() const
{
    return {
        __emitterPosition,
        __particleInitialVelocity,
        __particleAcceleration,
        __particleColor,
        __particleLifetime,
        __particleSize
    };
}

void ParticleSystem::EmitParticle(const Particle& particle)
{
    __particles.push_back(particle);
}

void ParticleSystem::EmitParticle()
{
    __particles.push_back(GetDefaultParticle());
}

void ParticleSystem::AddParticle(const float deltaTime)
{
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
    Rendering::SetDepthTest(false);
    Camera * camera = __camera ? __camera : Camera::MainCamera;
    if (__model == nullptr)
        __model = Model::CreateSquareModel();
    Model * model = __model;

    __particleShaderPipeline->Use();

    __particleTexture->BindTexture();
    __particleShaderPipeline->SetUniformMatrix4("view", camera->GetViewMatrix());
    __particleShaderPipeline->SetUniformMatrix4("projection", camera->GetProjectionMatrix());
    __particleShaderPipeline->SetUniformVec3("cameraPos", camera->GetPosition());
    __particleShaderPipeline->SetUniformInt("textureSampler", 0);
    for (const auto& particle : __particles)
    {
        __particleShaderPipeline->SetUniformVec4("color", particle.color);
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, particle.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(particle.size));
        // Rotates towards camera
        // Calculate the direction from the object's position to the camera position
        glm::vec3 objectToCamera = camera->GetPosition() - particle.position;

        // Calculate the rotation quaternion to align the object with the camera direction
        glm::quat rotation = glm::quatLookAt(glm::normalize(objectToCamera), glm::vec3(0.0f, 1.0f, 0.0f)); // Assuming up direction is (0, 1, 0)

        // Convert the quaternion to a rotation matrix
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

        // Apply the rotation to the model matrix
        modelMatrix = modelMatrix * rotationMatrix;
        __particleShaderPipeline->SetUniformMatrix4("model", modelMatrix);
        __model->SetDrawMode(_drawMode);
        __model->Draw();
    }
    Rendering::SetDepthTest(true);
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
void ParticleSystem::SetParticleGenerationFunction(CallbackContainer<void, ParticleSystem*, const float> generationFunction) { __particleGenerationFunction = generationFunction; }
glm::vec3 ParticleSystem::GetEmitterPosition() const { return __emitterPosition; }
ShaderPipeline* ParticleSystem::GetParticleShaderPipeline() const { return __particleShaderPipeline; }
int ParticleSystem::GetMaxParticles() const { return __maxParticles; }
float ParticleSystem::GetParticleSize() const { return __particleSize; }
Texture* ParticleSystem::GetParticleTexture() const { return __particleTexture; }
glm::vec4 ParticleSystem::GetParticleColor() const { return __particleColor; }
float ParticleSystem::GetParticleLifetime() const { return __particleLifetime; }
glm::vec3 ParticleSystem::GetParticleInitialVelocity() const { return __particleInitialVelocity; }
glm::vec3 ParticleSystem::GetParticleAcceleration() const { return __particleAcceleration; }
float ParticleSystem::GetEmissionRate() const { return __emissionRate; }
Camera* ParticleSystem::GetCamera() const { return __camera; }
