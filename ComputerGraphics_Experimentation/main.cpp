#include <common/FrameworkLoader.h>
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

#include <common/Window.h>
#include <common/Shader.h>
#include <common/ShaderPipeline.h>
#include <common/Model.h>
#include <common/Memory.h>
#include <common/Math_Base.h>
#include <common/Camera.h>
#include <common/GUI.h>

class Scene
{
public:
	int displayMode = 1;
	UPtr<ShaderPipeline> m_shader;
	UPtr<Model> m_model;
	Camera camera;
	Window* w;
	BaseFramework* fw;
	GUI* gui;

public:
	bool cameraLock = false;
	bool drawFaces = true;
	bool drawLines = true;
	bool drawPoints = true;

	Scene(Window * window, BaseFramework * framework, GUI * g)
		: w{ window }
		, fw{ framework }
		, gui{ g }
		, camera{ window->GetWindowWidth(), window->GetWindowHeight() }
	{
		auto shaderFrag = Shader::CreateShader("resources/shader_test.frag", Shader::ShaderType::Fragment);
		auto shaderVert = Shader::CreateShader("resources/shader_test.vert", Shader::ShaderType::Vertex);

		m_shader = ShaderPipeline::CreateShaderPipeline({shaderVert, shaderFrag});
		m_model = Model::CreateModel("resources\\Assignment1\\eight.uniform.obj");

		// Setting color to red
		VertexArray vertices = m_model->GetVertices();
		for (auto& vertex : vertices)
		{
			vertex.normals = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
		}
		m_model->SetVertices(vertices);

		w->SetOnKeyDownCallback([&](InputSystem::Key key, InputSystem::KeyModifier key_mod) {
			switch (key)
			{
			case InputSystem::Key::KeyboardT:
				cameraLock = !cameraLock;
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

	glm::vec3 position = { -0.1f, -0.3f, -3.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 verticesColor = { 1.0f, 0.0f, 0.0f };
	void Update()
	{
		glm::mat4 model(1);
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		m_shader->Use();
		m_shader->SetUniformMatrix4("model", model);
		m_shader->SetUniformMatrix4("view", view); 
		m_shader->SetUniformMatrix4("projection", projection);
		m_shader->SetUniformVec3("vertColor", verticesColor);
		if (drawFaces)
		{
			m_shader->SetUniformInt("useVertColor", 0);
			m_model->SetDrawMode(Model::DrawMode::TRIANGLES);
			m_model->Draw();
		}
		if (drawLines)
		{
			m_shader->SetUniformInt("useVertColor", 1);
			m_model->SetDrawMode(Model::DrawMode::LINES);
			m_model->Draw();
		}
        if (drawPoints)
		{
			m_shader->SetUniformInt("useVertColor", 1);
			m_model->SetDrawMode(Model::DrawMode::POINTS);
			m_model->Draw();
		}

		gui->NewFrame();
		{
			static float f = 0.0f;
			static int counter = 0;


			ImGui::Begin("Hello TA!");

			ImGui::Text("Press T to enable/disable the camera from following your cursor movement.");
			ImGui::Checkbox("Camera Lock", &cameraLock);
			ImGui::Checkbox("Draw Faces", &drawFaces);
			ImGui::Checkbox("Draw Lines", &drawLines);
			ImGui::Checkbox("Draw Points", &drawPoints);

			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("Vertex/Lines Color", glm::value_ptr(verticesColor));

			ImGui::Text("Object Properties:");
			ImGui::SliderFloat3("Position", glm::value_ptr(position), -5.0f, 5.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), 0.0f, 1.0f);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::End();
		}
		ImGui::Render();
		gui->RenderDrawData();
	}

	~Scene()
	{
	}
};

UPtr<Scene> s;

void scene()
{
	s->Update();
}

int main()
{
	try
	{
		Logger::Initialize("test.txt");

		Config& config = *Config::Load("Config.yaml");
		WindowAPI windowApi = config.WindowSettings().api;
		EngineAPI engineApi = config.EngineSettings().api;

		UPtr<Window> w = Window::CreateWindowFromAPI(windowApi);
		w->Init(config);
		w->SetSceneLoopCallback(scene);

		FrameworkLoader loader(engineApi);
		UPtr<BaseFramework> fw = loader.GetFramework();
		fw->Init();
		fw->SetWindow(w.get());

		UPtr<GUI> gui = GUI::CreateGUIFromAPI(windowApi, engineApi);
		gui->SetEngineAndWindowForInit(w.get(), fw.get());
		gui->Init();

		s = new Scene(w.get(), fw.get(), gui.get());

		fw->Launch();
		fw->GetLogger()->Log("Framework launched with %d\n", engineApi);

		// Order of destruction has to be this way because of dependence
		s.reset();
		gui.reset();
		fw.reset();
		w.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (int)ErrorCode::Failure;
	}
    return (int)ErrorCode::Success;
}