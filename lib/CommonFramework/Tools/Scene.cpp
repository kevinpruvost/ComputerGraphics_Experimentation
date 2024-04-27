#include <common/Scene.h>

Scene::Scene(Window* window, BaseFramework* framework, GUI* g)
    : w{ window }
    , fw{ framework }
    , gui{ g }
{
}
