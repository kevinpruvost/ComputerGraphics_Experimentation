#pragma once

#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Model.h>
#include <common/Object.h>
#include <common/ShaderPipeline.h>
#include <common/ECS/Entity.h>

class Transform : public VenomComponent<Transform>, public Drawable3D
{
public:
    Transform();
    ~Transform() = default;

    void Draw() const override;
    void Init() override;
    void Update() override;
    void Rotate(const glm::vec3& rotation);
    void RotateAroundParent(const glm::vec3& rotation);

    void Translate(const glm::vec3& translation);
    void Scale(const glm::vec3& scale);
    void SetParent(Transform* parent);

    void SetName(const char * name);
    const char * GetName() const;

    glm::vec3 GetWorldPosition() const;
    const glm::vec3 & GetPosition() const;
    glm::vec3 & GetPositionRef();
    const glm::vec3 & GetRotation() const;
    glm::vec3& GetRotationRef();
    glm::mat4 GetModelMatrix() const;

protected:

private:
    Transform* __parent;
    Model* __model;
    ShaderPipeline* __shaderPipeline;
    glm::vec3 __position;
    glm::vec3 __rotation;
    glm::vec3 __scale;
};

