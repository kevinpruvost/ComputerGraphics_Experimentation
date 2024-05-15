#include <common/Components/Transform.h>
#include <common/Camera.h>

Transform::Transform()
    : __position({ 0, 0, 0 })
    , __rotation({ 0, 0, 0 })
    , __scale({ 1, 1, 1 })
    , __parent(nullptr)
{
}

void Transform::Draw() const
{
    //__shaderPipeline->Use();
    //__shaderPipeline->SetUniformMatrix4("model", GetModelMatrix());
    //__shaderPipeline->SetUniformMatrix4("view", Camera::MainCamera->GetViewMatrix());
    //__shaderPipeline->SetUniformMatrix4("projection", Camera::MainCamera->GetProjectionMatrix());
    //__model->Draw();
}

void Transform::Init()
{
    SerializeProperty("Position", __position);
    SerializeProperty("Rotation", __rotation);
    SerializeProperty("Scale", __scale);
    SerializeProperty("Parent", &__parent);
}

void Transform::Update()
{
}

void Transform::Rotate(const glm::vec3& rotation)
{
    __rotation += rotation;
}

void Transform::RotateAroundParent(const glm::vec3& rotation)
{
    if (!__parent) return;

    //// Calculate the rotation matrix based on the given rotation vector
    //glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
    //    glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
    //    glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    //// Get the model matrix of the parent
    //glm::mat4 parentModel = __parent->GetModelMatrix();

    //// Translate to the origin of the parent, rotate, then translate back
    //glm::vec3 parentPosition = __parent->GetPosition();
    //glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -parentPosition);
    //glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), parentPosition);

    //// Apply the rotation to the object's position relative to the parent
    //glm::vec3 relativePosition = __position - parentPosition;
    //glm::vec4 rotatedPosition = translateBack * parentModel * translateToOrigin * glm::vec4(relativePosition, 1.0f) * rotationMatrix;

    //// Update the object's position
    //__position = glm::vec3(rotatedPosition);
}

void Transform::Translate(const glm::vec3& translation)
{
    __position += translation;
}

void Transform::Scale(const glm::vec3& scale)
{
    __scale += scale;
}

void Transform::SetParent(Transform* parent)
{
    __parent = parent;
}

void Transform::SetName(const char* name)
{
    _entity->SetEntityName(name);
}

const char* Transform::GetName() const
{
    return _entity->GetObjectName();
}

glm::vec3 Transform::GetWorldPosition() const
{
    if (!__parent) return __position;

    // Get the model matrix of the parent
    glm::mat4 parentModel = __parent->GetModelMatrix();

    // Apply the transformation of the parent object
    glm::vec4 worldPosition = parentModel * glm::vec4(__position, 1.0f);

    return glm::vec3(worldPosition);
}

const glm::vec3 & Transform::GetPosition() const
{
    return __position;
}

glm::vec3& Transform::GetPositionRef()
{
    return __position;
}

const glm::vec3 & Transform::GetRotation() const
{
    return __rotation;
}

glm::vec3& Transform::GetRotationRef()
{
    return __rotation;
}

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model(1);
    model = glm::translate(model, __position);
    model = glm::rotate(model, __rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, __rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, __rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, __scale);

    if (__parent) {
        // Apply the transformation of the parent object
        glm::mat4 parentModel = __parent->GetModelMatrix();
        model = parentModel * model;
    }

    return model;
}