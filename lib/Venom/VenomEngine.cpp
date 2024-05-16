#include <common/VenomEngine.h>

#include <common/Config.h>
#include <common/ECS/System.h>
#include <common/Rendering.h>

VenomEngine::VenomEngine()
	: __w{ nullptr }
	, __fw{ nullptr }
	, __gui{ nullptr }
	, __sceneConstructor{ nullptr }
{
}

VenomEngine::~VenomEngine()
{
	// Order of destruction has to be this way because of dependence
	__s.reset();
	__gui.reset();
	__fw.reset();
	__w.reset();
}

Venom::ErrorCode VenomEngine::LaunchEngine()
{
	Venom::ErrorCode err;
	if ((err = LoadEngine()) != Venom::ErrorCode::Success) return err;
	if ((err = LoadScene())  != Venom::ErrorCode::Success) return err;
	if ((err = Init())       != Venom::ErrorCode::Success) return err;
	if ((err = Run())        != Venom::ErrorCode::Success) return err;
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::LoadEngine()
{
	Venom::ErrorCode err;
    Logger::Initialize("test.txt");
    Config * config = Config::Load(__configPath);
    WindowAPI windowApi = config->WindowSettings().api;
    GraphicsEngineAPI engineApi = config->EngineSettings().api;

	if ((err = LoadContext(windowApi, config->WindowSettings())) != Venom::ErrorCode::Success) {
        Logger::Print("Window Init failed");
        return err;
    }

	if ((err = LoadGraphicsEngine(engineApi)) != Venom::ErrorCode::Success) {
        Logger::Print("Graphics Engine Init failed");
        return err;
    }

	if ((err = LoadGUI(windowApi, engineApi)) != Venom::ErrorCode::Success) {
		Logger::Print("GUI Init failed");
		return err;
	};

	if ((err = LoadResources(config)) != Venom::ErrorCode::Success) {
		Logger::Print("Resource loading failed");
		return err;
	}
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::LoadScene()
{
	assert(__sceneConstructor != nullptr && "Scene constructor is not set, call VenomEngine::SetScene<SceneType>()");
	__s = __sceneConstructor();
	__s->SetWindow(__w.get());
	__s->SetFramework(__fw.get());
	__s->SetGUI(__gui.get());
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::Init()
{
	Venom::ErrorCode err = __s->Init();
	if (err != Venom::ErrorCode::Success) {
		Logger::Print("Scene Init failed");
		return err;
	}
	err = SystemManager::Init(EntityPool::GetAllEntities());
	if (err != Venom::ErrorCode::Success) {
		Logger::Print("SystemManager Init failed");
		return err;
	}

	// Rendering settings
	Rendering::SetDepthTest(true);
	Rendering::SetBlending(true);
	Rendering::SetBlendingFunction(Rendering::BlendingFunction::SRC_ALPHA, Rendering::BlendingFunction::ONE_MINUS_SRC_ALPHA);
	Rendering::SetBlendingEquation(Rendering::BlendingEquation::ADD);

	Time::SetStartTime();
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::Run()
{
	// TODO: REWRITE RUN
	std::vector<Entity *> * entities = EntityPool::GetAllEntities();
	while (true)
	{
		if (__w->ShouldClose()) break;

		if (SystemManager::Update(entities) != Venom::ErrorCode::Success) {
			Logger::Print("SystemManager Stop");
			break;
		}

		if (__fw->Update() != Venom::ErrorCode::Success) {
            Logger::Print("Graphics Engine stop");
            break;
        }

		if (__s->Update() != Venom::ErrorCode::Success) {
			Logger::Print("Scene stop");
			break;
		}

		if (__w->Update() != Venom::ErrorCode::Success) {
            Logger::Print("Window stop");
            break;
        }
	}
	return Venom::ErrorCode::Success;
}

void VenomEngine::SetConfigName(const char* configName)
{
	__configPath = configName;
}

Venom::ErrorCode VenomEngine::ChangeGraphicsEngine(GraphicsEngineAPI engineAPI)
{
	Venom::ErrorCode err = LoadGraphicsEngine(engineAPI);
	if (err != Venom::ErrorCode::Success) {
		Logger::Print("ChangeGraphicsEngine failed on engine loading");
		return err;
	}
	if (err != Venom::ErrorCode::Success) {
		Logger::Print("ChangeGraphicsEngine failed on GUI loading");
		return err;
	}
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::ChangeContext(WindowAPI windowAPI)
{
	Venom::ErrorCode err = LoadContext(windowAPI, Config::Load(__configPath)->WindowSettings());
	if (err != Venom::ErrorCode::Success) {
		Logger::Print("ChangeContext failed on context loading");
		return err;
	}
	err = ReloadGUI();
	if (err != Venom::ErrorCode::Success) {
        Logger::Print("ChangeContext failed on GUI loading");
        return err;
    }
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::ReloadGUI()
{
	return LoadGUI(__windowAPI, __engineAPI);
}

Venom::ErrorCode VenomEngine::LoadGUI(WindowAPI windowAPI, GraphicsEngineAPI engineAPI)
{
	__gui = GUI::CreateGUIFromAPI(windowAPI, engineAPI);
	__gui->SetEngineAndWindowForInit(__w.get(), __fw.get());
	return __gui->Init();
}

Venom::ErrorCode VenomEngine::LoadResources(Config* config)
{
	return config->LoadResources();
}

Venom::ErrorCode VenomEngine::LoadGraphicsEngine(GraphicsEngineAPI engineAPI)
{
	EngineLoader loader(engineAPI);
	if ((__fw = loader.GetEngine()) == nullptr) {
        Logger::Print("Graphics Engine load failed");
        return Venom::ErrorCode::Failure;
    }
	Venom::ErrorCode err = __fw->Init();
	if (err != Venom::ErrorCode::Success)
	{
		Logger::Print("Graphics Engine Init failed");
        return err;
	}
	__engineAPI = engineAPI;
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::LoadContext(WindowAPI windowAPI, const WindowSettings & windowSettings)
{
	__w = ContextAPI::CreateWindowFromAPI(windowAPI);
	if (__w->Init(windowSettings) != Venom::ErrorCode::Success) {
		Logger::Print("Window Init failed");
		return Venom::ErrorCode::Failure;
	}
	__windowAPI = windowAPI;
	return Venom::ErrorCode::Success;
}