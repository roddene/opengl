#pragma once
#include "../config.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"


class MotionSystem{
    public:

    void update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        std::unordered_map<unsigned int,PhysicsComponent> &physicsComponents,
    std::unordered_map <unsigned int , RenderComponent> &renderComponents,
        float dt);
};