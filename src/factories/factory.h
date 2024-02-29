#pragma once
#include "../config.h"

#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"
#define PI glm::pi<float>()
#define HALF_PI glm::half_pi<float>()
#define TWO_PI glm::two_pi<float>()

class Factory{

    public:
    Factory(

    std::unordered_map<unsigned int,TransformComponent>& transformComponents,
    std::unordered_map<unsigned int,RenderComponent>& renderComponents,
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents
    );
    ~Factory();

    unsigned int make_camera(glm::vec3 position,glm::vec3 eulers);

    void make_cube(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);

    void Factory::make_circle(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);
    private:

    unsigned int entities_made = 0;

    std::unordered_map<unsigned int,TransformComponent>& transformComponents;
    std::unordered_map<unsigned int,RenderComponent>& renderComponents;
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> EBOs;
    std::vector<unsigned int> textures;

    RenderComponent make_cube_mesh(glm::vec3 size);
    RenderComponent make_circle_mesh(glm::vec3 size);
    RenderComponent make_sphere_mesh(glm::vec3 size);

    unsigned int triangle_number(unsigned int n);
    



};