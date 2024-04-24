#include "Scene.h"

class MainScene : public Scene
{
public:
	int displayMode = 1;
	UPtr<ShaderPipeline> m_shader, m_textShader;
	UPtr<Model> m_sphereModel;
	UPtr<Texture> m_textureEarth, m_textureJupiter, m_textureMars, m_textureMercury, m_textureMoon, m_textureNeptune, m_textureSaturn, m_textureSun, m_textureUranus, m_textureVenus;
	UPtr<Object> m_earth, m_jupiter, m_mars, m_mercury, m_moon, m_neptune, m_saturn, m_sun, m_uranus, m_venus;
	UPtr<Text2D> m_text2D;
	std::vector<Object*> m_objects;
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

		m_shader = ShaderPipeline::CreateShaderPipeline({ shaderVert, shaderFrag });
		m_textShader = ShaderPipeline::CreateShaderPipeline({ textShaderVert, textShaderFrag });
		m_sphereModel = Model::CreateSphereModel(1.0f, 30, 30);

		m_textureEarth = Texture::CreateTexture("resources/Planets/earth.jpg");
		m_textureJupiter = Texture::CreateTexture("resources/Planets/jupiter.jpg");
		m_textureMars = Texture::CreateTexture("resources/Planets/mars.jpg");
		m_textureMercury = Texture::CreateTexture("resources/Planets/mercury.jpg");
		m_textureMoon = Texture::CreateTexture("resources/Planets/moon.jpg");
		m_textureNeptune = Texture::CreateTexture("resources/Planets/neptune.jpg");
		m_textureSaturn = Texture::CreateTexture("resources/Planets/saturn.jpg");
		m_textureSun = Texture::CreateTexture("resources/Planets/sun.jpg");
		m_textureUranus = Texture::CreateTexture("resources/Planets/uranus.jpg");
		m_textureVenus = Texture::CreateTexture("resources/Planets/venus.jpg");

		// Create the sun (no parent)
		m_sun = new Object(m_sphereModel.get(), m_textureSun.get(), { 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 2.0f });
		m_sun->SetName("Sun");

		// Create the planets
		m_earth = new Object(m_sphereModel.get(), m_textureEarth.get(), { 4.0f, 0.0f, -2.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.5f });
		m_earth->SetName("Earth");
		m_moon = new Object(m_sphereModel.get(), m_textureMoon.get(), { 4.5f, 0.0f, -2.5f }, glm::vec3{ 0.0f }, glm::vec3{ 0.1f });
		m_moon->SetName("Moon");

		m_mercury = new Object(m_sphereModel.get(), m_textureMercury.get(), { -6.0f, 0.0f, 1.5f }, glm::vec3{ 0.0f }, glm::vec3{ 0.2f });
		m_mercury->SetName("Mercury");
		m_venus = new Object(m_sphereModel.get(), m_textureVenus.get(), { 7.5f, 0.0f, -2.5f }, glm::vec3{ 0.0f }, glm::vec3{ 0.3f });
		m_venus->SetName("Venus");
		m_mars = new Object(m_sphereModel.get(), m_textureMars.get(), { -9.5f, 0.0f, 1.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.4f });
		m_mars->SetName("Mars");
		m_jupiter = new Object(m_sphereModel.get(), m_textureJupiter.get(), { 14.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.6f });
		m_jupiter->SetName("Jupiter");
		m_saturn = new Object(m_sphereModel.get(), m_textureSaturn.get(), { -18.0f, 0.0f, 5.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.7f });
		m_saturn->SetName("Saturn");
		m_uranus = new Object(m_sphereModel.get(), m_textureUranus.get(), { 22.0f, 0.0f, -6.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.8f });
		m_uranus->SetName("Uranus");
		m_neptune = new Object(m_sphereModel.get(), m_textureNeptune.get(), { -26.0f, 0.0f, 7.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.9f });
		m_neptune->SetName("Neptune");

		m_earth->SetParent(m_sun.get());
		m_moon->SetParent(m_earth.get());
		m_mercury->SetParent(m_sun.get());
		m_venus->SetParent(m_sun.get());
		m_mars->SetParent(m_sun.get());
		m_jupiter->SetParent(m_sun.get());
		m_saturn->SetParent(m_sun.get());
		m_uranus->SetParent(m_sun.get());
		m_neptune->SetParent(m_sun.get());

		m_objects.push_back(m_sun.get());
		m_objects.push_back(m_earth.get());
		m_objects.push_back(m_moon.get());
		m_objects.push_back(m_mercury.get());
		m_objects.push_back(m_venus.get());
		m_objects.push_back(m_mars.get());
		m_objects.push_back(m_jupiter.get());
		m_objects.push_back(m_saturn.get());
		m_objects.push_back(m_uranus.get());
		m_objects.push_back(m_neptune.get());

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

			Logger::DebugPrint("Yaw: %f", camera.GetYaw());
			Logger::DebugPrint("Position: %f %f %f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

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
		camera.SetPosition(glm::vec3(-3.0f, 0.0f, -3.0f));

		delete shaderFrag;
		delete shaderVert;
	}

	void Update() override
	{
		// Define rotation periods in days
		constexpr double earthRotationPeriod = 1.0;
		constexpr double moonRotationPeriod = 27.3;
		constexpr double mercuryRotationPeriod = 58.6;
		constexpr double venusRotationPeriod = 243.0;
		constexpr double marsRotationPeriod = 1.03;
		constexpr double jupiterRotationPeriod = 0.41;
		constexpr double saturnRotationPeriod = 0.44;
		constexpr double uranusRotationPeriod = 0.72;
		constexpr double neptuneRotationPeriod = 0.67;

		// Adjust the rotation speed factor based on real-world rotation periods
		double rotationSpeedFactor = 360.0 / 365.0; // 1 revolution per year for the earth

		// Adjust rotation speed for each planet
		m_earth->Rotate(glm::vec3{ 0.0f,   rotationSpeed * rotationSpeedFactor / earthRotationPeriod * Time::GetLambda(), 0.0f });
		m_moon->Rotate(glm::vec3{ 0.0f,    rotationSpeed * rotationSpeedFactor / moonRotationPeriod * Time::GetLambda(), 0.0f });
		m_mercury->Rotate(glm::vec3{ 0.0f, rotationSpeed * rotationSpeedFactor / mercuryRotationPeriod * Time::GetLambda(), 0.0f });
		m_venus->Rotate(glm::vec3{ 0.0f,   rotationSpeed * rotationSpeedFactor / venusRotationPeriod * Time::GetLambda(), 0.0f });
		m_mars->Rotate(glm::vec3{ 0.0f,    rotationSpeed * rotationSpeedFactor / marsRotationPeriod * Time::GetLambda(), 0.0f });
		m_jupiter->Rotate(glm::vec3{ 0.0f, rotationSpeed * rotationSpeedFactor / jupiterRotationPeriod * Time::GetLambda(), 0.0f });
		m_saturn->Rotate(glm::vec3{ 0.0f,  rotationSpeed * rotationSpeedFactor / saturnRotationPeriod * Time::GetLambda(), 0.0f });
		m_uranus->Rotate(glm::vec3{ 0.0f,  rotationSpeed * rotationSpeedFactor / uranusRotationPeriod * Time::GetLambda(), 0.0f });
		m_neptune->Rotate(glm::vec3{ 0.0f, rotationSpeed * rotationSpeedFactor / neptuneRotationPeriod * Time::GetLambda(), 0.0f });
		m_sun->Rotate(glm::vec3{ 0.0f,     rotationSpeed * rotationSpeedFactor / earthRotationPeriod * Time::GetLambda(), 0.0f }); // Same as Earth for simplicity


		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

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
				m_shader->SetUniformInt("useVertColor", 0);
				obj->Draw(Model::DrawMode::TRIANGLES);
			}
			if (drawLines)
			{
				m_shader->SetUniformInt("useVertColor", 1);
				obj->Draw(Model::DrawMode::LINES);
			}
			if (drawPoints)
			{
				m_shader->SetUniformInt("useVertColor", 1);
				obj->Draw(Model::DrawMode::POINTS);
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