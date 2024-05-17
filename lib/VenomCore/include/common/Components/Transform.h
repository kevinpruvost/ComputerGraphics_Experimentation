#pragma once

#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Model.h>
#include <common/Components/Transform.h>
#include <common/ShaderPipeline.h>
#include <common/ECS/Component.h>

class Transform : public VenomComponent<Transform>
{
public:
    Transform();
    ~Transform() = default;

    void Draw() const;
    void Init() override;
    void Update() override;
    void Rotate(const glm::vec3& rotation);
    void RotateAroundParent(const glm::vec3& rotation);

    void SetPosition(const glm::vec3& position);
    void Translate(const glm::vec3& translation);
    void Scale(const glm::vec3& scale);
    void SetScale(const glm::vec3& scale);
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
    glm::vec3 __position;
    glm::vec3 __rotation;
    glm::vec3 __scale;
};

