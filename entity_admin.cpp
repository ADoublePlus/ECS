#include "entity_admin.h"

using namespace ecs;

EntityAdmin::~EntityAdmin()
{
    DestroyAllSystems();
    DestroyAllEntities();
}

void ecs::EntityAdmin::Update(float time_step)
{
    for (BaseSystem* s : _systems)
    {
        s->Update(time_step);
    }
}

Entity* ecs::EntityAdmin::FindEntity(EntityID eid)
{
    auto it = _entities.find(eid);
    return (it == _entities.end()) ? nullptr : it->second;
}

void ecs::EntityAdmin::DestroyEntity(EntityID eid)
{
    auto it = _entities.find(eid);

    if (it != _entities.end())
    {
        delete it->second;
        _entities.erase(it);
    }
}

void EntityAdmin::DestroyAllSystems()
{
    for (BaseSystem* sys : _systems)
    {
        delete sys;
    }

    _systems.clear();
}

void EntityAdmin::DestroyAllEntities()
{
    for (auto& kv : _entities)
    {
        delete kv.second;
    }

    _entities.clear();
}

ComponentVector& EntityAdmin::GetAllComponents(index_t id)
{
    return _component_pool.GetAllComponents(id);
}