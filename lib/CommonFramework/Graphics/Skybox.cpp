#include <common/Skybox.h>
#include <common/Camera.h>
#include <common/Rendering.h>

static VertexBuffer* skyboxVertexBuffer = nullptr;

Skybox::Skybox(ShaderPipeline* shader, Texture* texture)
    : __shader(shader), __texture(texture)
{
    if (skyboxVertexBuffer == nullptr)
    {
        const VertexArray skyboxVertices = {
            // Positions
            {{-1.0f,  1.0f, -1.0f}},
            {{-1.0f, -1.0f, -1.0f}},
            {{ 1.0f, -1.0f, -1.0f}},
            {{ 1.0f, -1.0f, -1.0f}},
            {{ 1.0f,  1.0f, -1.0f}},
            {{ -1.0f,  1.0f, -1.0f }},

            {{-1.0f, -1.0f,  1.0f}},
            {{-1.0f, -1.0f, -1.0f}},
            {{-1.0f,  1.0f, -1.0f}},
            {{-1.0f,  1.0f, -1.0f}},
            {{-1.0f,  1.0f,  1.0f}},
            {{-1.0f, -1.0f,  1.0f}},

            {{ 1.0f, -1.0f, -1.0f}},
            {{ 1.0f, -1.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{ 1.0f,  1.0f, -1.0f}},
            {{ 1.0f, -1.0f, -1.0f}},

            {{-1.0f, -1.0f,  1.0f}},
            {{-1.0f,  1.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{ 1.0f, -1.0f,  1.0f}},
            {{-1.0f, -1.0f,  1.0f}},

            {{-1.0f,  1.0f, -1.0f}},
            {{ 1.0f,  1.0f, -1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}},
            {{-1.0f,  1.0f,  1.0f}},
            {{-1.0f,  1.0f, -1.0f}},

            {{-1.0f, -1.0f, -1.0f}},
            {{-1.0f, -1.0f,  1.0f}},
            {{ 1.0f, -1.0f, -1.0f}},
            {{ 1.0f, -1.0f, -1.0f}},
            {{-1.0f, -1.0f,  1.0f}},
            {{ 1.0f, -1.0f,  1.0f}}
        };
        skyboxVertexBuffer = VertexBuffer::CreateVertexBuffer();
        skyboxVertexBuffer->SetVertices(skyboxVertices);
    }

}

void Skybox::Draw()
{
    assert(__shader != nullptr && __texture != nullptr);

    Rendering::SetDrawMode(Drawable3D::DrawMode::SOLID);
    Rendering::SetDepthTest(false);
    __shader->Use();
    __texture->BindTexture(Texture::TextureType::TextureCubemap);

    __shader->SetUniformMatrix4("view", glm::mat4(glm::mat3(Camera::MainCamera->GetViewMatrix())));
    __shader->SetUniformMatrix4("projection", Camera::MainCamera->GetProjectionMatrix());
    __shader->SetUniformInt("textureSampler0", 0);
    Rendering::DrawVertices(skyboxVertexBuffer);
    Rendering::SetDepthTest(true);
}

Venom::ErrorCode Skybox::ReloadObjectFromEngine()
{
    return Venom::ErrorCode::Success;
}
