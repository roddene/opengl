#pragma once
#include "../config.h"

#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"
#include "../components/mesh_vector_component.h"
#include "../components/angle_axis_component.h"
#include "../view/shader.h"

#include "sphere.h"
#include "octahedron.h"

#include "cube.h"


class Factory{

    public:
    Factory(

    std::unordered_map<unsigned int,TransformComponent>& transformComponents,
    std::unordered_map<unsigned int,RenderComponent>& renderComponents,
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents,
    std::vector<Shader*> shaders
    );
    ~Factory();

    unsigned int make_camera(glm::vec3 position,AngleAxis angleaxis);

    void make_cube(glm::vec3 position,AngleAxis angleaxis,AngleAxis angleAxisVelocity);
    void make_cuboid_rigid(glm::vec3 position, glm::vec3 velocity, AngleAxis angleaxis, AngleAxis angleAxisVelocity);
    void make_cuboid(glm::vec3 position, glm::vec3 velocity, AngleAxis angleaxis, AngleAxis angleAxisVelocity);
    void make_cube_soft(glm::vec3 position, AngleAxis angleaxis, AngleAxis angleAxisVelocity);

    void make_cube_light(glm::vec3 position, AngleAxis angleaxis, AngleAxis angleAxisVelocity);

    void make_sphere(glm::vec3 position,AngleAxis angleaxis,AngleAxis angleAxisVelocity);

    void make_sphere_test(glm::vec3 position, AngleAxis angleaxis, AngleAxis angleAxisVelocity);

    void make_ground(glm::vec3 position, AngleAxis angleaxis, AngleAxis angleAxisVelocity);
    void make_octahedron(glm::vec3 position,AngleAxis angleaxis,AngleAxis angleAxisVelocity);
    void make_sphere_to_octahedron(glm::vec3 position,AngleAxis angleaxis,AngleAxis angleAxisVelocity);

    private:
    float lerp(float a,float b, float t);

    unsigned int entities_made = 0;

    std::unordered_map<unsigned int,TransformComponent>& transformComponents;
    std::unordered_map<unsigned int,RenderComponent>& renderComponents;
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> EBOs;
    std::vector<unsigned int> textures;
    std::vector<Shader*> shaders;

    RenderComponent make_shape_colored(MeshVectorComponent details,unsigned int frameCount,unsigned int shaderIndex);

    void make_physics_component_soft(unsigned int entityID, AngleAxis angleAxisVelocity, TransformComponent *transform);
    void make_physics_component(unsigned int entityID, AngleAxis angleAxisVelocity, glm::vec3 velocity);
    void make_physics_component(unsigned int entityID, AngleAxis angleAxisVelocity);
    void make_physics_component_rigid(unsigned int entityID, AngleAxis angleAxisVelocity, TransformComponent *transform);
    RenderComponent make_shape_colored_soft(MeshVectorComponent details, unsigned int frameCount, unsigned int shaderIndex);

    TransformComponent* make_transform_component(unsigned int entityID, glm::vec3 position, AngleAxis angleaxis);
};