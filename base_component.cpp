#include "entity.h"

using namespace ecs;

BaseComponent* BaseComponent::SiblingComponent(const index_t index)
{
    return _ent ? _ent->GetComponent(index) : nullptr;
}