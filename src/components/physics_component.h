#pragma once
#include "../config.h"
#include "../physics/softbody.h"
#include "../physics/rigidbody.h"
#include "../components/angle_axis_component.h"

struct PhysicsComponent{
    glm::vec3 velocity;
    glm::quat rotationQuat;
    SoftBody* softbody;
    RigidBody* rigidbody;
};
