#pragma once
#include "../config.h"

#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"
#include "../components/render_component.h"
#include "../components/mesh_vector_component.h"
#include "../view/shader.h"

#include "sphere.h"
#include "octahedron.h"
class Factory{

    public:
    Factory(

    std::unordered_map<unsigned int,TransformComponent>& transformComponents,
    std::unordered_map<unsigned int,RenderComponent>& renderComponents,
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents,
    std::vector<Shader*> shaders
    );
    ~Factory();

    unsigned int make_camera(glm::vec3 position,glm::vec3 eulers);

    void make_cube(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);

    void make_sphere(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);

    void make_octahedron(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);
    void make_sphere_to_octahedron(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity);

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

    RenderComponent make_shape_colored(MeshVectorComponent details,unsigned int frameCount);

    RenderComponent make_cube_mesh(glm::vec3 size);

    



};