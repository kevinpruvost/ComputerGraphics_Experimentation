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

class Scene
{
public:
	Scene(Window * window)
		: w{ window }
		, camera{800, 600}
	{
		auto shaderFrag = Shader::CreateShader(
			"resources/shader_test.frag",
			Shader::ShaderType::Fragment
		);
		auto shaderVert = Shader::CreateShader(
			"resources/shader_test.vert",
			Shader::ShaderType::Vertex
		);

		m_shader = ShaderPipeline::CreateShaderPipeline({shaderVert, shaderFrag});
		m_model = Model::CreateModel("resources\\Assignment1\\eight.uniform.obj");

		// Setting color to red
		VertexArray vertices = m_model->GetVertices();
		for (auto& vertex : vertices)
		{
			vertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		m_model->SetVertices(vertices);

		w->SetWhileKeyDownCallback([&](InputSystem::Key key, InputSystem::KeyModifier key_mod) {
			float speed = 0.01f;
			float movementSpeed = 0.0001f;
			Logger::DebugPrint("Yaw: %f", camera.GetYaw());
			Logger::DebugPrint("Position: %f %f %f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
			switch (key)
			{
			case InputSystem::Key::Left:
				camera.RotateYaw(-speed);
                break;
			case InputSystem::Key::Right:
				camera.RotateYaw(speed);
		        break;
			case InputSystem::Key::Up:
				camera.RotatePitch(speed);
                break;
			case InputSystem::Key::Down:
				camera.RotatePitch(-speed);
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
		});
		camera.SetPosition(glm::vec3(-3.0f, 0.0f, -3.0f));

		delete shaderFrag;
		delete shaderVert;
	}

	void Update()
	{
		glm::mat4 model(1);
		glm::vec3 position(-1.0f, 0.0f, -1.0f);
		model = glm::translate(model, position);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		m_shader->Use();
		m_shader->SetUniformMatrix4("model", model);
		m_shader->SetUniformMatrix4("view", view);
		m_shader->SetUniformMatrix4("projection", projection);
		m_model->Draw();
	}

	~Scene()
	{
	}

public:
	UPtr<ShaderPipeline> m_shader;
	UPtr<Model> m_model;
	Camera camera;
	Window* w;
};

Scene * s;

void scene()
{
	s->Update();
}

int main()
{
	try
	{
		Logger::Initialize("test.txt");

		Window * w = Window::CreateWindowFromAPI(Window::WindowAPI::GLFW);
		Config & windowConfig = *Config::Load("Config_GLFWWindow.yaml");
		w->Init(windowConfig);
		w->SetSceneLoopCallback(scene);

		FrameworkLoader::FrameworkType engineType = FrameworkLoader::FrameworkType::OpenGL;
		FrameworkLoader loader(engineType);
		BaseFramework* fw = loader.GetFramework();
		fw->Init();
		fw->SetWindow(w);

		s = new Scene(w);

		fw->Launch();
		fw->GetLogger()->Log("Framework launched with %d\n", engineType);

		delete s;
		delete fw;
		delete w;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (int)ErrorCode::Failure;
	}
    return (int)ErrorCode::Success;
}