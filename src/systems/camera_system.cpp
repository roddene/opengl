#include "camera_system.h"

CameraSystem::CameraSystem(unsigned int shader,GLFWwindow* window){
    this->window = window;

    glUseProgram(shader);
    viewLocation = glGetUniformLocation(shader,"view");
}




bool CameraSystem::update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        unsigned int cameraID,CameraComponent& cameraComponent, float dt){
            //std::cout << transformComponents[cameraID].position.x<<"\n";
            //std::cout << transformComponents[cameraID].position.y<<"\n";
            //std::cout << transformComponents[cameraID].position.z<<"\n";

            glm::vec3& pos = transformComponents[cameraID].position;
            glm::vec3& eulers = transformComponents[cameraID].eulers;
            float theta = glm::radians(eulers.z);
            float phi = glm::radians(eulers.y);

            glm::vec3& right = cameraComponent.right;
            glm::vec3& up = cameraComponent.up;
            glm::vec3& forwards = cameraComponent.forwards;

            glm::vec3 target ={
                0.0f,
                0.0f,
                0.0f
            };

            forwards = {
                glm::cos(theta)*glm::cos(phi),
                glm::sin(theta)*glm::cos(phi),
                glm::sin(phi)
            };
            //std::cout << "hi" << forwards[0] <<forwards[1]<<forwards[2]<<"\n";

            right = glm::normalize(glm::cross(forwards,global_up));
            up = glm::normalize(glm::cross(right,forwards));

            glm::mat4 view = glm::lookAt(pos,pos+forwards,up);

            glUniformMatrix4fv(viewLocation,1,GL_FALSE,glm::value_ptr(view));

            glfwPollEvents();


    return false;

        }


