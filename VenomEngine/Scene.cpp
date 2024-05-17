#include "Scene.h"

#include <common/Rendering.h>
#include <common/Components/Components.h>
#include <common/ECS/Entity.h>

#include <type_traits>

MainScene::MainScene()
	: Scene()
	, camera{nullptr}
{
}

Venom::ErrorCode MainScene::_Init()
{
	camera = new Camera(w->GetWindowWidth(), w->GetWindowHeight());
	w->LockCursor();
	m_wireframeShader = Resources::Load<ShaderPipeline>("Wireframe");
	m_particleShader = Resources::Load<ShaderPipeline>("Particle_Snow");
	m_skyboxShader = Resources::Load<ShaderPipeline>("Skybox");
	m_shader = Resources::Load<ShaderPipeline>("Normal_Shader");
	m_textShader = Resources::Load<ShaderPipeline>("Text2D");

	Entity* skybox = Entity::CreateEntity();
	Skybox* skyboxComponent = skybox->AddComponent<Skybox>();
	skyboxComponent->shader = Resources::Load<ShaderPipeline>("Skybox");
	skyboxComponent->texture = Resources::Load<Texture>("Skybox_Texture");

	Entity* e1 = Entity::CreateEntity();
	e1->AddComponent<Transform>();

	Entity* e = Entity::CreateEntity();
	auto transform = e->AddComponent<Transform>();
	auto particleSystem = e->AddComponent<ParticleSystem>();
	particleSystem->SetEmitterPosition(glm::vec3(5.0f));
	particleSystem->SetParticleTexture(Resources::Load<Texture>("Snowflake"));
	particleSystem->SetParticleSize(1.0f);
	particleSystem->SetParticleColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	particleSystem->SetParticleAcceleration(glm::vec3(0.0f, -1.0f, 0.0f));
	particleSystem->SetParticleInitialVelocity(glm::vec3(0.0f, -1.0f, 0.0f));
	particleSystem->SetParticleLifetime(10.0f);
	particleSystem->SetEmissionRate(3);
	particleSystem->SetMaxParticles(1000);
	particleSystem->SetParticleGenerationFunction([&](ParticleSystem* sys, const float deltaTime) {
		Particle p = sys->GetDefaultParticle();
		p.position = glm::vec3(glm::linearRand<float>(-10.0f, 10.0f), p.position.y, glm::linearRand<float>(-10.0f, 10.0f));
		p.size = glm::linearRand<float>(0.5f, 3.0f);
		sys->EmitParticle(p);
		});
	particleSystem->SetEntityName("Snow Particle System");
	particleSystem->SetParticleShaderPipeline(Resources::Load<ShaderPipeline>("Particle_Snow"));

	Entity* lightEntity = Entity::CreateEntity();
	lightEntity->SetEntityName("Light");
	Light * lightComp = lightEntity->AddComponent<Light>();
	lightComp->SetIntensity(20.0f);
	Transform * lightTransform = lightEntity->AddComponent<Transform>();
	Renderer * lightRenderer = lightEntity->AddComponent<Renderer>();
	lightRenderer->SetModel(Resources::Load<Model>("Sphere"));
	lightRenderer->SetShaderPipeline(Resources::Load<ShaderPipeline>("Normal_Shader"));
	lightTransform->SetPosition({1.0f, 1.0f, 1.0f});

	Entity * pbrEntity = Entity::CreateEntity();
	pbrEntity->SetEntityName("Compass");
	Transform * pbrTransform = pbrEntity->AddComponent<Transform>();
	pbrTransform->SetPosition({ 0.0f, -3.0f, -1.0f });
	Renderer * pbrRenderer = pbrEntity->AddComponent<Renderer>();
	pbrRenderer->SetModel(Resources::Load<Model>("Compass"));
	pbrRenderer->SetShaderPipeline(Resources::Load<ShaderPipeline>("PBR_Shader"));

	Entity* pbrEntity2 = Entity::CreateEntity();
	pbrEntity2->SetEntityName("Backpack");
	Transform* pbrTransform2 = pbrEntity2->AddComponent<Transform>();
	pbrTransform2->SetPosition({ 6.0f, -1.0f, -6.0f });
	pbrTransform2->Rotate({ 0.0f, -1.0f, 0.0f });
	pbrTransform2->SetScale({ 0.05f, 0.05f, 0.05f });
	Renderer* pbrRenderer2 = pbrEntity2->AddComponent<Renderer>();
	pbrRenderer2->SetModel(Resources::Load<Model>("Backpack"));
	pbrRenderer2->SetShaderPipeline(Resources::Load<ShaderPipeline>("PBR_Shader"));

	m_sphereModel = Resources::Load<Model>("Sphere");

	// Create text objects
	m_text2D = Text2D::CreateText2D("resources/fonts/arial.ttf", 24);

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

		//Logger::DebugPrint("Yaw: %f", camera->GetYaw());
		//Logger::DebugPrint("Position: %f %f %f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

		if (key_mod == InputSystem::KeyModifier::Shift)
		{
			movementSpeed *= 10.0f;
		}

		if (!cameraLock)
		{
			switch (key)
			{
			case InputSystem::Key::Space:
				camera->Translate(glm::vec3(0, movementSpeed, 0));
				break;
			case InputSystem::Key::LCtrl:
				camera->Translate(glm::vec3(0, -movementSpeed, 0));
				break;
			case InputSystem::Key::KeyboardW:
				camera->Translate(glm::vec3(0, 0, movementSpeed));
				break;
			case InputSystem::Key::KeyboardS:
				camera->Translate(glm::vec3(0, 0, -movementSpeed));
				break;
			case InputSystem::Key::KeyboardA:
				camera->Translate(glm::vec3(-movementSpeed, 0, 0));
				break;
			case InputSystem::Key::KeyboardD:
				camera->Translate(glm::vec3(movementSpeed, 0, 0));
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
			camera->RotateYaw(xoffset);
			camera->RotatePitch(yoffset);
		}
		});

	m_wireframeShader->Use();
	m_wireframeShader->SetUniformVec3("wireframeColor", verticesColor);


	camera->SetPosition(glm::vec3(-3.0f, 0.0f, -3.0f));
	camera->LookAt(particleSystem->GetEmitterPosition());

	Material* mat = Resources::Create<Material>("SunMaterial");
	mat->AddTexture(Resources::Load<Texture>("Mars"));
	m_sphereModel->AddMaterial(mat);
	//auto test = new Object(m_sphereModel, Resources::Load<ShaderPipeline>("Normal_Shader"), glm::vec3(0), glm::vec3(0), glm::vec3(0.5));
	//m_objects.push_back(test);
	//mat = Resources::Create<Material>("Assignment5_Material");
	//mat->AddTexture(Resources::Load<Texture>("Assignment5_Texture"));
	//auto model = Resources::Load<Model>("Assignment5_Model");
	//model->AddMaterial(mat);
	//model->SetShader(Resources::Load<ShaderPipeline>("Bezier"));
	//model->SetWireframeShader(Resources::Load<ShaderPipeline>("Bezier_Wireframe"));
	//test = new Object(
	//	model,
	//	Resources::Load<ShaderPipeline>("Bezier"), glm::vec3(5), glm::vec3(0), glm::vec3(0.5));
	//m_objects.push_back(test);
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode MainScene::Update()
{

	gui->NewFrame();
	{
		gui->DrawMainMenuBar();
		ImGui::Begin("Hello TA!");

		ImGui::Text("Press T to enable/disable the camera from following your cursor movement.");
		if (ImGui::Checkbox("Camera Lock", &cameraLock))
		{
			if (!cameraLock) w->LockCursor();
			else w->UnlockCursor();
		}
		ImGui::Text("Camera Properties:");
		if (ImGui::DragFloat3("Position", glm::value_ptr(camera->GetPositionRef()), -100, 100.0f))
		{
			camera->UpdateViewMatrix();
		}
		ImGui::DragFloat("Speed", &camera->GetSpeedRef(), 0.0f, 100.0f);

		
		if (ImGui::Checkbox("Draw Faces", &drawFaces) || ImGui::Checkbox("Draw Lines", &drawLines) || ImGui::Checkbox("Draw Points", &drawPoints))
		{
			Rendering::SetGlobalDrawMode(Drawable3D::GetDrawModeFromStates(drawPoints, drawLines, drawFaces));
		}

		if (ImGui::ColorEdit3("Vertex/Lines Color", glm::value_ptr(verticesColor)))
		{
			m_wireframeShader->Use();
			m_wireframeShader->SetUniformVec3("wireframeColor", verticesColor);
			Resources::Load<ShaderPipeline>("Bezier_Wireframe")->Use();
			Resources::Load<ShaderPipeline>("Bezier_Wireframe")->SetUniformVec3("wireframeColor", verticesColor);
		}

		gui->DrawObjectsProperties();

		ImGui::End();
	}

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = camera->GetProjectionMatrix();

	//for (auto& obj : m_objects)
	//{
	//	obj->Draw();
	//}


	//for (auto& obj : m_objects)
	//{
	//	// Draw Text
	//	bool isOnScreen = IsOnScreen(obj->GetWorldPosition(), view, projection, w->GetWindowWidth(), w->GetWindowHeight());
	//	if (!isOnScreen) continue;
	//	glm::vec2 screenPos = WorldToScreenSpace(obj->GetWorldPosition(), view, projection, w->GetWindowWidth(), w->GetWindowHeight());
	//	m_textShader->Use();
	//	m_textShader->SetUniformMatrix4("projection", glm::ortho(0.0f, static_cast<float>(w->GetWindowWidth()), 0.0f, static_cast<float>(w->GetWindowHeight())));
	//	m_textShader->SetUniformVec3("textColor", glm::vec3{ 0.0f, 1.0f, 0.0f });
	//	m_text2D->RenderText(obj->GetName(), screenPos.x, screenPos.y, 1.0f, glm::vec3{ 0.0f, 1.0f, 0.0f });
	//}

	ImGui::Render();
	gui->RenderDrawData();
	return Venom::ErrorCode::Success;
}