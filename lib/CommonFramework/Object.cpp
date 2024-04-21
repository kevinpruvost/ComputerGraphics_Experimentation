#include <common/Object.h>

Object::Object(Model* model, Texture* texture, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : __model(model), __texture(texture), __position(position), __rotation(rotation), __scale(scale), __parent(nullptr)
{
}

void Object::Draw(Model::DrawMode drawMode) const
{
    __model->SetDrawMode(drawMode);
    __model->Draw();
}

void Object::Rotate(const glm::vec3& rotation)
{
    __rotation += rotation;
}

void Object::RotateAroundParent(const glm::vec3& rotation)
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

void Object::Translate(const glm::vec3& translation)
{
    __position += translation;
}

void Object::Scale(const glm::vec3& scale)
{
    __scale += scale;
}

void Object::SetParent(Object* parent)
{
    __parent = parent;
}

void Object::SetName(const char* name)
{
    __name = name;
}

const char* Object::GetName() const
{
    return __name;
}

glm::vec3 Object::GetWorldPosition() const
{
    if (!__parent) return __position;

    // Get the model matrix of the parent
    glm::mat4 parentModel = __parent->GetModelMatrix();

    // Apply the transformation of the parent object
    glm::vec4 worldPosition = parentModel * glm::vec4(__position, 1.0f);

    return glm::vec3(worldPosition);
}

glm::vec3 Object::GetPosition() const
{
    return __position;
}

glm::vec3 Object::GetRotation() const
{
    return __rotation;
}

glm::mat4 Object::GetModelMatrix() const
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

const Texture* Object::GetTexture() const
{
    return __texture;
}