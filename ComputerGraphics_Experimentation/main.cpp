#include <common/FrameworkLoader.h>
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

#include <common/Window.h>
#include <common/Shader.h>
#include <common/ShaderPipeline.h>
#include <common/Model.h>

class Scene
{
public:
	Scene() {
		auto shaderFrag = Shader::CreateShader(
			"resources/shader_test.frag",
			Shader::ShaderType::Fragment
		);
		auto shaderVert = Shader::CreateShader(
			"resources/shader_test.vert",
			Shader::ShaderType::Vertex
		);

		m_shader = ShaderPipeline::CreateShaderPipeline({shaderVert, shaderFrag});

		m_model = Model::CreateModel("resources\\Shrek\\Shrek.obj");
		//m_mesh->SetVertices({
		//	{{ -0.5f, -0.5f, 0.0f }},  // Left  
		//	{{ 0.5f,  -0.5f, 0.0f }},  // Right 
		//	{{ 0.0f,   0.5f, 0.0f  }}  // Top   
		//});

		delete shaderFrag;
		delete shaderVert;
	}

	~Scene()
	{
		if (m_shader) delete m_shader;
		if (m_model) delete m_model;
	}

public:
	ShaderPipeline * m_shader;
	Model * m_model;
};

Scene * s;

void scene()
{
	s->m_shader->Use();
	s->m_model->Draw();
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

		s = new Scene();

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