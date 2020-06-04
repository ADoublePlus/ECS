#pragma once

namespace ecs
{
    class EntityAdmin;

    class BaseSystem
    {
        protected:
            EntityAdmin* _admin{ nullptr };

        public:
            BaseSystem(EntityAdmin* admin);
            virtual ~BaseSystem() = default;

            virtual void Update(float time_step) {}
    };
}