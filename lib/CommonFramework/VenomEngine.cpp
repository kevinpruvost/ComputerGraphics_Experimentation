#include <common/VenomEngine.h>

#include <common/Config.h>
#include <common/ECS/System.h>

VenomEngine::VenomEngine()
	: __w{ nullptr }
	, __fw{ nullptr }
	, __gui{ nullptr }
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

Venom::ErrorCode VenomEngine::LoadEngine(const char* configPath)
{
	Venom::ErrorCode err;
    Logger::Initialize("test.txt");
    Config * config = Config::Load("Config.yaml");
    WindowAPI windowApi = config->WindowSettings().api;
    EngineAPI engineApi = config->EngineSettings().api;

	__w = Window::CreateWindowFromAPI(windowApi);
	__w->Init(config);

	EngineLoader loader(engineApi);
	__fw = loader.GetFramework();
	__fw->Init();
	__fw->SetWindow(__w.get());

	__gui = GUI::CreateGUIFromAPI(windowApi, engineApi);
	__gui->SetEngineAndWindowForInit(__w.get(), __fw.get());
	__gui->Init();

	if ((err = config->LoadResources()) != Venom::ErrorCode::Success)
		return err;

	__w->SetScene(&__s);

	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::Init()
{
	Venom::ErrorCode err = __s->Init();
	if (err != Venom::ErrorCode::Success)
        return err;
	SystemManager::Init(EntityPool::GetAllEntities());
	return Venom::ErrorCode::Success;
}

Venom::ErrorCode VenomEngine::Run()
{
	// TODO: REWRITE RUN
	std::vector<Entity *> * entities = EntityPool::GetAllEntities();
	while (true) {
		SystemManager::Update(entities);
		__fw->Launch();
	}
	return Venom::ErrorCode::Success;
}
