#pragma once

#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Model.h>
#include <common/Object.h>
#include <common/ShaderPipeline.h>
#include <common/ECS/Entity.h>

class Object : public Component, public Drawable3D
{
public:
    Object(Model* model, ShaderPipeline * shaderPipeline, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    ~Object() = default;

    void Draw() const override;
    void Init() override;
    void Update() override;
    void Rotate(const glm::vec3& rotation);
    void RotateAroundParent(const glm::vec3& rotation);

    void Translate(const glm::vec3& translation);
    void Scale(const glm::vec3& scale);
    void SetParent(Object* parent);

    void SetName(const char * name);
    const char * GetName() const;

    glm::vec3 GetWorldPosition() const;
    const glm::vec3 & GetPosition() const;
    glm::vec3 & GetPositionRef();
    const glm::vec3 & GetRotation() const;
    glm::vec3& GetRotationRef();
    glm::mat4 GetModelMatrix() const;

protected:
    Object(const glm::vec3& position = glm::vec3(0), const glm::vec3& rotation = glm::vec3(0), const glm::vec3& scale = glm::vec3(1));

private:
    Object* __parent;
    Model* __model;
    ShaderPipeline* __shaderPipeline;
    glm::vec3 __position;
    glm::vec3 __rotation;
    glm::vec3 __scale;
};

