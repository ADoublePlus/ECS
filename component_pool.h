#pragma once

#include "ecs_functional.h"
#include "base_component.h"

#include <map>
#include <memory>
#include <vector>

namespace ecs 
{
    class BaseComponent;
    using ComponentVector = std::vector<BaseComponent*>;

    class ComponentPool
    {
        private:
            std::map<index_t, ComponentVector> _component_pools;

        public:
            template <class C, typename... Args>
            BaseComponent* CreateComponent(Args&&... args);

            void RemoveComponent(index_t id, BaseComponent* component);

            ComponentVector& GetAllComponents(index_t id);
    };

    template <class C, typename... Args>
    BaseComponent* ComponentPool::CreateComponent(Args&&... args)
    {
        ComponentVector& pool = GetAllComponents(details::ComponentIndex::index<C>());
        BaseComponent* component = new C();
        (static_cast<C*>(component))->Reset(std::forward<Args>(args)...);
        pool.push_back(component);
        return component;
    }
}