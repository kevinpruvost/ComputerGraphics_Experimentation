#include <common/Entity.h>
#include <common/Camera.h>

Entity::Entity(Model* model, ShaderPipeline* shaderPipeline, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : __model(model), __position(position), __rotation(rotation), __scale(scale), __parent(nullptr), __shaderPipeline(shaderPipeline)
{
    _properties.SetProperty("Position", __position);
    _properties.SetProperty("Rotation", __rotation);
    _properties.SetProperty("Scale", __scale);
    _properties.SetProperty("Parent", &__parent);
    _properties.SetProperty("Model", &__model);
}

void Entity::Draw() const
{
    __shaderPipeline->Use();
    __shaderPipeline->SetUniformMatrix4("model", GetModelMatrix());
    __shaderPipeline->SetUniformMatrix4("view", Camera::MainCamera->GetViewMatrix());
    __shaderPipeline->SetUniformMatrix4("projection", Camera::MainCamera->GetProjectionMatrix());
    __model->Draw();
}

void Entity::Rotate(const glm::vec3& rotation)
{
    __rotation += rotation;
}

void Entity::RotateAroundParent(const glm::vec3& rotation)
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

void Entity::Translate(const glm::vec3& translation)
{
    __position += translation;
}

void Entity::Scale(const glm::vec3& scale)
{
    __scale += scale;
}

void Entity::SetParent(Entity* parent)
{
    __parent = parent;
}

void Entity::SetName(const char* name)
{
    _objectName = name;
}

const char* Entity::GetName() const
{
    return _objectName.c_str();
}

glm::vec3 Entity::GetWorldPosition() const
{
    if (!__parent) return __position;

    // Get the model matrix of the parent
    glm::mat4 parentModel = __parent->GetModelMatrix();

    // Apply the transformation of the parent object
    glm::vec4 worldPosition = parentModel * glm::vec4(__position, 1.0f);

    return glm::vec3(worldPosition);
}

const glm::vec3 & Entity::GetPosition() const
{
    return __position;
}

glm::vec3& Entity::GetPositionRef()
{
    return __position;
}

const glm::vec3 & Entity::GetRotation() const
{
    return __rotation;
}

glm::vec3& Entity::GetRotationRef()
{
    return __rotation;
}

glm::mat4 Entity::GetModelMatrix() const
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