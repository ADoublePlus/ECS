#include "base_system.h"
#include "entity_admin.h"

using namespace ecs;

BaseSystem::BaseSystem(EntityAdmin* admin) : _admin(admin) {}