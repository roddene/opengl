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
        
        float angle = (float) glfwGetTime()*.01f;
        transformComponents[entity.first].quaternion = 

        transformComponents[entity.first].quaternion*
        physicsComponents[entity.first].rotationQuat;

        //std::cout <<transformComponents[entity.first].quaternion.wxyz<<"\n"; 

        //glm::conjugate(transformComponents[entity.first].quaternion);
        
        if (physicsComponents[entity.first].softbody){
            

            std::vector<float> vertices = physicsComponents[entity.first].softbody->make_vertices(dt); 

            glBindBuffer(GL_ARRAY_BUFFER,render.VBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,vertices.size()*sizeof(float),vertices.data());
            
        }

        if(physicsComponents[entity.first].rigidbody){
            physicsComponents[entity.first].rigidbody->update(dt);
        }
        



    }
}
