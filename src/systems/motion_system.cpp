#include "motion_system.h"
void MotionSystem::update(
    std::unordered_map < unsigned int, TransformComponent> &transformComponents,
    std::unordered_map < unsigned int, PhysicsComponent> &physicsComponents,
    std::unordered_map <unsigned int , RenderComponent> &renderComponents,
    float dt)
{
    for(std::pair<unsigned int,PhysicsComponent> entity:physicsComponents){

        RenderComponent render = renderComponents[entity.first]; 

        transformComponents[entity.first].position+=entity.second.velocity *dt;
        transformComponents[entity.first].eulers+=entity.second.eulerVelocity * dt;
        if (physicsComponents[entity.first].softbody){
            

            std::vector<float> vertices = physicsComponents[entity.first].softbody->make_vertices(dt); 

            glBindBuffer(GL_ARRAY_BUFFER,render.VBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,vertices.size()*sizeof(float),vertices.data());
            
        }
        if (transformComponents[entity.first].eulers.z >360){

            transformComponents[entity.first].eulers.z-=360;
        }


    }
}
