#include "Scene.h"

#include <common/Rendering.h>
#include <common/ParticleSystem.h>

#include <type_traits>

MainScene::MainScene(Window* window, BaseFramework* framework, GUI* g)
	: Scene(window, framework, g)
	, camera{ window->GetWindowWidth(), window->GetWindowHeight() }
{
	w->LockCursor();
	m_wireframeShader = Resources::Load<ShaderPipeline>("Wireframe");
	m_particleShader = Resources::Load<ShaderPipeline>("Particle_Snow");
	m_shader = Resources::Load<ShaderPipeline>("Skybox");
	m_textShader = Resources::Load<ShaderPipeline>("Text2D");

	m_sphereModel = Model::CreateSphereModel(1.0f, 30, 30);

	m_ParticleSystem = ParticleSystem::CreateParticleSystem();
	m_particlesystem2 = ParticleSystem::CreateParticleSystem();

	m_textureParticles = Resources::Load<Texture>("Snowflake");

	m_backgroundTexture = Resources::Load<Texture>("Skybox_Texture");

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
		float movementSpeed = Time::GetLambda();

		//Logger::DebugPrint("Yaw: %f", camera.GetYaw());
		//Logger::DebugPrint("Position: %f %f %f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		if (key_mod == InputSystem::KeyModifier::Shift)
		{
			movementSpeed *= 10.0f;
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
	m_ParticleSystem->SetParticleTexture(m_textureParticles);
	m_ParticleSystem->SetParticleSize(1.0f);
	m_ParticleSystem->SetParticleColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_ParticleSystem->SetParticleAcceleration(glm::vec3(0.0f, -1.0f, 0.0f));
	m_ParticleSystem->SetParticleInitialVelocity(glm::vec3(0.0f, -1.0f, 0.0f));
	m_ParticleSystem->SetParticleLifetime(10.0f);
	m_ParticleSystem->SetEmissionRate(3);
	m_ParticleSystem->SetMaxParticles(1000);
	m_ParticleSystem->SetParticleGenerationFunction([&](ParticleSystem* sys, const float deltaTime) {
		Particle p = sys->GetDefaultParticle();
		p.position = glm::vec3(glm::linearRand<float>(-10.0f, 10.0f), p.position.y, glm::linearRand<float>(-10.0f, 10.0f));
		p.size = glm::linearRand<float>(0.5f, 3.0f);
		sys->EmitParticle(p);
	});
	m_ParticleSystem->SetObjectName("Snow Particle System");

	*m_particlesystem2 = *m_ParticleSystem;
	m_particlesystem2->SetObjectName("Snow Particle System 2");

	m_wireframeShader->Use();
	m_wireframeShader->SetUniformVec3("wireframeColor", verticesColor);

	m_ParticleSystem->SetParticleShaderPipeline(m_particleShader);

	camera.SetPosition(glm::vec3(-3.0f, 0.0f, -3.0f));
	camera.LookAt(m_ParticleSystem->GetEmitterPosition());

	m_skybox = std::make_unique<Skybox>(m_shader, m_backgroundTexture);

	m_sun = new Entity(m_sphereModel, m_textureParticles, glm::vec3(0), glm::vec3(0), glm::vec3(3));
	m_objects.push_back(m_sun);

	Time::SetStartTime();
}

void MainScene::Update()
{
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
		ImGui::Text("Camera Properties:");
		if (ImGui::SliderFloat3("Position", glm::value_ptr(camera.GetPositionRef()), -100, 100.0f))
		{
			camera.UpdateViewMatrix();
		}
		
		if (ImGui::Checkbox("Draw Faces", &drawFaces))
		{
		}
		if (ImGui::Checkbox("Draw Lines", &drawLines))
		{
		}
		if (ImGui::Checkbox("Draw Points", &drawPoints))
		{
		}
		m_ParticleSystem->SetDrawMode(Drawable3D::GetDrawMode(drawPoints, drawLines, drawFaces));

		if (ImGui::ColorEdit3("Vertex/Lines Color", glm::value_ptr(verticesColor)))
		{
			m_wireframeShader->Use();
			m_wireframeShader->SetUniformVec3("wireframeColor", verticesColor);
		}

		gui->DrawObjectsProperties();

		ImGui::End();
	}

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix();

	m_skybox->Draw();

	m_ParticleSystem->Update(Time::GetLambda());
	m_ParticleSystem->SetEmissionRate(m_ParticleSystem->GetEmissionRate() + Time::GetLambda() * 0.25f);

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
			m_sphereModel->SetDrawMode(Drawable3D::DrawMode::SOLID);
			obj->Draw();
		}
		if (drawLines)
		{
			m_sphereModel->SetDrawMode(Drawable3D::DrawMode::WIREFRAME);
			obj->Draw();
		}
		if (drawPoints)
		{
			m_sphereModel->SetDrawMode(Drawable3D::DrawMode::POINTS);
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

	ImGui::Render();
	gui->RenderDrawData();
}