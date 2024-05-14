#include <common/Skybox.h>
#include <common/Camera.h>
#include <common/Rendering.h>

static VertexBuffer* skyboxVertexBuffer = nullptr;

Skybox::Skybox()
    : shader(nullptr), texture(nullptr)
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

void Skybox::Draw() const
{
    assert(shader != nullptr && texture != nullptr);

    Rendering::SetDrawMode(Drawable3D::DrawMode::SOLID);
    Rendering::SetDepthTest(false);
    shader->Use();
    texture->BindTexture(Texture::TextureType::TextureCubemap);

    shader->SetUniformMatrix4("view", glm::mat4(glm::mat3(Camera::MainCamera->GetViewMatrix())));
    shader->SetUniformMatrix4("projection", Camera::MainCamera->GetProjectionMatrix());
    shader->SetUniformInt("textureSampler0", 0);
    Rendering::DrawVertices(skyboxVertexBuffer);
    Rendering::SetDepthTest(true);
}

void Skybox::Update()
{
    Draw();
}

void Skybox::Init()
{
}
