#pragma once

#include <common/Mesh.h>
#include <common/Texture.h>
#include <common/Model.h>

class Entity
{
public:
    Entity(Model* model, Texture* texture, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    ~Entity() = default;

    void Draw() const;
    void Rotate(const glm::vec3& rotation);
    void RotateAroundParent(const glm::vec3& rotation);

    void Translate(const glm::vec3& translation);
    void Scale(const glm::vec3& scale);
    void SetParent(Entity* parent);

    void SetName(const char * name);
    const char * GetName() const;

    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::mat4 GetModelMatrix() const;
    const Texture * GetTexture() const;

private:
    Entity* __parent;
    Model* __model;
    Texture* __texture;
    glm::vec3 __position;
    glm::vec3 __rotation;
    glm::vec3 __scale;
    const char * __name;
};

