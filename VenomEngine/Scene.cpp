#include "Scene.h"

#include <common/Rendering.h>
#include <common/ParticleSystem.h>

class MainScene : public Scene
{
public:
	int displayMode = 1;
	UPtr<ShaderPipeline> m_shader, m_textShader, m_particleShader;
	UPtr<Model> m_sphereModel;
	UPtr<Texture> m_textureParticles;
	UPtr<ParticleSystem> m_ParticleSystem;
	UPtr<Text2D> m_text2D;
	std::vector<Entity*> m_objects;
	Camera camera;

	bool cameraLock = false;
	bool drawFaces = true;
	bool drawLines = false;
	bool drawPoints = false;

	float rotationSpeed = 1.0f;
	glm::vec3 verticesColor = { 1.0f, 0.0f, 0.0f };

public:
	MainScene(Window* window, BaseFramework* framework, GUI* g)
		: Scene(window, framework, g)
		, camera{ window->GetWindowWidth(), window->GetWindowHeight() }
	{
		w->LockCursor();
		auto shaderFrag = Shader::CreateShader("resources/shader_hw2.frag", Shader::ShaderType::Fragment);
		auto shaderVert = Shader::CreateShader("resources/shader_hw2.vert", Shader::ShaderType::Vertex);
		auto textShaderFrag = Shader::CreateShader("resources/text.frag", Shader::ShaderType::Fragment);
		auto textShaderVert = Shader::CreateShader("resources/text.vert", Shader::ShaderType::Vertex);
		auto particleShaderFrag = Shader::CreateShader("resources/shader_particle.frag", Shader::ShaderType::Fragment);
		auto particleShaderVert = Shader::CreateShader("resources/shader_particle.vert", Shader::ShaderType::Vertex);

		m_shader = ShaderPipeline::CreateShaderPipeline({ shaderVert, shaderFrag });
		m_textShader = ShaderPipeline::CreateShaderPipeline({ textShaderVert, textShaderFrag });
		m_particleShader = ShaderPipeline::CreateShaderPipeline({ particleShaderVert, particleShaderFrag });
		m_sphereModel = Model::CreateSphereModel(1.0f, 30, 30);

		m_ParticleSystem = ParticleSystem::CreateParticleSystem();

		m_textureParticles = Texture::CreateTexture("resources/Particles/Star.bmp");

		// Create text objects
		const char* fontPath = "resources/fonts/arial.ttf";
		m_text2D = Text2D::CreateText2D(fontPath, 24);

		w->SetOnKeyDownCallback([&](InputSystem::Key key, InputSystem::KeyModifier key_mod) {
			switch (key)
			{
			case InputSystem::Key::KeyboardT:
				cameraLock = !cameraLock;
				if (!cameraLock) w->LockCursor();
				else w->UnlockCursor();
				break;
			}
			});

		w->SetWhileKeyDownCallback([&](InputSystem::Key key, InputSystem::KeyModifier key_mod) {
			float speed = 0.01f;
			float movementSpeed = 0.001f;

			//Logger::DebugPrint("Yaw: %f", camera.GetYaw());
			//Logger::DebugPrint("Position: %f %f %f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

			if (key_mod == InputSystem::KeyModifier::Shift)
			{
				movementSpeed *= 10.0f;
				speed *= 10.0f;
			}

			if (!cameraLock)
			{
				switch (key)
				{
				case InputSystem::Key::Space:
					camera.Translate(glm::vec3(0, movementSpeed, 0));
					break;
				case InputSystem::Key::LCtrl:
					camera.Translate(glm::vec3(0, -movementSpeed, 0));
					break;
				case InputSystem::Key::KeyboardW:
					camera.Translate(glm::vec3(0, 0, movementSpeed));
					break;
				case InputSystem::Key::KeyboardS:
					camera.Translate(glm::vec3(0, 0, -movementSpeed));
					break;
				case InputSystem::Key::KeyboardA:
					camera.Translate(glm::vec3(-movementSpeed, 0, 0));
					break;
				case InputSystem::Key::KeyboardD:
					camera.Translate(glm::vec3(movementSpeed, 0, 0));
					break;
				default:
					break;
				}
			}
			});
		w->SetOnMouseMoveCallback([&](double x, double y) {
			float sensitivity = 0.1f;
			float xoffset = x * sensitivity;
			float yoffset = y * sensitivity;

			if (!cameraLock)
			{
				camera.RotateYaw(xoffset);
				camera.RotatePitch(yoffset);
			}
			});

		m_ParticleSystem->SetEmitterPosition(glm::vec3(5.0f));
		m_ParticleSystem->SetDrawMode(Drawable3D::DrawMode::WIREFRAME_SOLID_POINTS);
		m_ParticleSystem->SetParticleTexture(m_textureParticles.get());
		m_ParticleSystem->SetParticleSize(2.0f);
		m_ParticleSystem->SetParticleColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_ParticleSystem->SetParticleAcceleration(glm::vec3(0.0f));
		m_ParticleSystem->SetParticleInitialVelocity(glm::vec3(0.0f, 1.0f, 0.0f));
		m_ParticleSystem->SetParticleLifetime(5.0f);
		m_ParticleSystem->SetEmissionRate(10);
		m_ParticleSystem->SetMaxParticles(1000);

		m_ParticleSystem->SetParticleShaderPipeline(m_particleShader.get());

		camera.SetPosition(glm::vec3(-3.0f, 0.0f, -3.0f));
		camera.LookAt(m_ParticleSystem->GetEmitterPosition());

		delete shaderFrag;
		delete shaderVert;

		Time::SetStartTime();
	}

	void Update() override
	{
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		Time::SetNewLambda();

		m_ParticleSystem->Update(Time::GetLambda());

		for (auto& obj : m_objects)
		{
			m_shader->Use();
			m_shader->SetUniformMatrix4("view", view);
			m_shader->SetUniformMatrix4("projection", projection);
			m_shader->SetUniformVec3("vertColor", verticesColor);
			m_shader->SetUniformInt("textureSampler", 0);
			m_shader->SetUniformMatrix4("model", obj->GetModelMatrix());
			obj->GetTexture()->BindTexture();
			if (drawFaces)
			{
				m_shader->SetDrawMode(Drawable3D::DrawMode::SOLID);
				obj->Draw();
			}
			if (drawLines)
			{
				m_shader->SetDrawMode(Drawable3D::DrawMode::WIREFRAME);
				obj->Draw();
			}
			if (drawPoints)
			{
				m_shader->SetDrawMode(Drawable3D::DrawMode::POINTS);
				obj->Draw();
			}
		}

		for (auto& obj : m_objects)
		{
			// Draw Text
			bool isOnScreen = IsOnScreen(obj->GetWorldPosition(), view, projection, w->GetWindowWidth(), w->GetWindowHeight());
			if (!isOnScreen) continue;
			glm::vec2 screenPos = WorldToScreenSpace(obj->GetWorldPosition(), view, projection, w->GetWindowWidth(), w->GetWindowHeight());
			m_textShader->Use();
			m_textShader->SetUniformMatrix4("projection", glm::ortho(0.0f, static_cast<float>(w->GetWindowWidth()), 0.0f, static_cast<float>(w->GetWindowHeight())));
			m_textShader->SetUniformVec3("textColor", glm::vec3{ 0.0f, 1.0f, 0.0f });
			m_text2D->RenderText(obj->GetName(), screenPos.x, screenPos.y, 1.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
		}

		gui->NewFrame();
		{
			static float f = 0.0f;

			ImGui::Begin("Hello TA!");

			ImGui::Text("Press T to enable/disable the camera from following your cursor movement.");
			if (ImGui::Checkbox("Camera Lock", &cameraLock))
			{
				if (!cameraLock) w->LockCursor();
				else w->UnlockCursor();
			}
			ImGui::Checkbox("Draw Faces", &drawFaces);
			ImGui::Checkbox("Draw Lines", &drawLines);
			ImGui::Checkbox("Draw Points", &drawPoints);

			ImGui::ColorEdit3("Vertex/Lines Color", glm::value_ptr(verticesColor));

			ImGui::Text("Planet Properties:");
			ImGui::DragFloat("Rotation Speed", &rotationSpeed, 0.01f, 0.0f, 10.0f);

			ImGui::SameLine();

			ImGui::End();
		}
		ImGui::Render();
		gui->RenderDrawData();
	}
};

Scene::Scene(Window* window, BaseFramework* framework, GUI* g)
	: w(window), fw(framework), gui(g)
{
}

Scene* Scene::CreateScene(Window* window, BaseFramework* framework, GUI* g)
{
	return new MainScene(window, framework, g);
}