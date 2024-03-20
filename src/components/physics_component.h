#pragma once
#include "../config.h"
#include "../physics/physics.h"

struct PhysicsComponent{
    glm::vec3 velocity;
    glm::vec3 eulerVelocity;
    SoftBody* softbody;
};
