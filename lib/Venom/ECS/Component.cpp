#include <common/ECS/Component.h>

int Component::IDCounter = 0;
UPtr<std::vector<const char *>> _componentNames;

void Component::SetEntityName(const char* name)
{
    _entity->SetEntityName(name);
}

int Component::GetComponentID()
{
    return _componentID;
}

const char* Component::GetComponentName()
{
    assert(_componentID < _componentNames->size() && "Component ID out of range");
    return _componentNames->at(_componentID);
}

Callback<void>& Component::GetGUICallback()
{
    return _guiCallback;
}

Component::Component(int componentID)
    : _guiCallback{ nullptr }
    , _entity{ nullptr }
    , _componentID{ componentID }
{
}

int Component::GetNewComponentID(const char* componentName)
{
    if (_componentNames == nullptr) _componentNames.reset(new std::vector<const char*>());
    // If componentName contains "class ", then skips to next position
    if (strncmp(componentName, "class ", 6) == 0) componentName += 6;
    _componentNames->push_back(componentName);
    return IDCounter++;
}

std::vector<PropertyManager::Property>& Component::GetProperties()
{
    return _properties.GetProperties();
}
