#include "entity.h"
#include "base_component.h"

using namespace ecs;

Entity::Entity(ComponentPool& pool, EntityID eid) : _pool(pool), _eid(eid) {}

Entity::~Entity()
{
    DestroyAllComponents();
}

Entity& Entity::AddComponent(const index_t index, BaseComponent* component)
{
    ECS_ASSERT(!HasComponent(index), "Error, cannot add component to entity, component already exists");
    _components[index] = component;
    component->SetOwner(this);
    return *this;
}

Entity& Entity::RemoveComponent(const index_t index)
{
    ECS_ASSERT(HasComponent(index), "Error, cannot remove component to entity, component does not exist");
    ReplaceWith(index, nullptr);
    return *this;
}

Entity& Entity::ReplaceComponent(const index_t index, BaseComponent* component)
{
    if (HasComponent(index))
    {
        ReplaceWith(index, component);
    }
    else if (component != nullptr)
    {
        AddComponent(index, component);
    }

    return *this;
}

BaseComponent* Entity::GetComponent(const index_t index) const
{
    if (!HasComponent(index))
        return nullptr;

    return _components.at(index);
}

bool Entity::HasComponent(const index_t index) const
{
    return (_components.find(index) != _components.end());
}

void Entity::Destroy() {}

void Entity::ReplaceWith(const index_t index, BaseComponent* replacement)
{
    BaseComponent* prev_component = GetComponent(index);

    if (prev_component == replacement) {}
    else 
    {
        // On remove
        _pool.RemoveComponent(index, prev_component);

        if (replacement == nullptr)
        {
            _components.erase(index);
        }
        else 
        {
            _components[index] = replacement;
            _components[index]->SetOwner(this);
        }
    }
}

void Entity::DestroyAllComponents()
{
    for (const auto& kv : _components)
    {
        // Remove 
        _pool.RemoveComponent(kv.first, kv.second);
    }

    _components.clear();
}