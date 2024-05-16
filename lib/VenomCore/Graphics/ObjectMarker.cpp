#include <common/ObjectMarker.h>
#include <common/Rendering.h>
#include <common/Camera.h>

static ShaderPipeline * s_markerShaderPipeline = nullptr;
static VertexBuffer * s_markerVertexBuffer = nullptr;

void ObjectMarker::DrawMarker(const glm::vec3& position, const glm::vec3& scale)
{
    if (s_markerShaderPipeline == nullptr) return;

    s_markerShaderPipeline->Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    //model = glm::rotate(model, __rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, __rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, __rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    s_markerShaderPipeline->SetUniformMatrix4("model", model);
    s_markerShaderPipeline->SetUniformMatrix4("view", Camera::MainCamera->GetViewMatrix());
    s_markerShaderPipeline->SetUniformMatrix4("projection", Camera::MainCamera->GetProjectionMatrix());
    Rendering::DrawLines(s_markerVertexBuffer);
}

void ObjectMarker::SetMarkerShaderPipeline(ShaderPipeline* shader)
{
    s_markerShaderPipeline = shader;
    if (s_markerVertexBuffer == nullptr) {
        s_markerVertexBuffer = VertexBuffer::CreateVertexBuffer();
        s_markerVertexBuffer->SetVertices({
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, // X axis red
            { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // Y axis green
            { glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // Z axis blue
            { glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
        });
    }
}
