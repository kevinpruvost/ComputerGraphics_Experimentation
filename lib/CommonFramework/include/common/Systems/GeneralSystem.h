#pragma once

#include <common/ECS/System.h>

class COMMONFRAMEWORK_API GeneralSystem : public VenomSystem<GeneralSystem>
{
public:
    Venom::ErrorCode Init(std::vector<Entity*>* entities) override;
    Venom::ErrorCode Update(std::vector<Entity*>* entities) override;
};