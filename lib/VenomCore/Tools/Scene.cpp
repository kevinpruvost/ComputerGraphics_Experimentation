#include <common/Scene.h>

bool IScene::_started = false;

Scene::Scene()
    : w{ nullptr }
    , fw{ nullptr }
    , gui{ nullptr }
{
}

Venom::ErrorCode Scene::Init()
{
    if (w == nullptr || fw == nullptr || gui == nullptr) {
        Logger::Print("Scene::Init() failed: w, fw or gui is nullptr");
        return Venom::ErrorCode::Failure;
    }
    Venom::ErrorCode err = _Init();
    if (err == Venom::ErrorCode::Success) _started = true;
    return err;
}

void Scene::SetWindow(ContextAPI* window)
{
    w = window;
}

void Scene::SetFramework(BaseFramework* framework)
{
    fw = framework;
}

void Scene::SetGUI(GUI* g)
{
    gui = g;
}

bool IScene::IsStarted()
{
    return _started;
}
