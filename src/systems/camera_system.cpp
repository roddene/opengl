#include "camera_system.h"

CameraSystem::CameraSystem(std::vector<Shader*> shaders,GLFWwindow* window){
    this->window = window;
    this->shaders = shaders;
    for(auto shader:shaders){
    glUseProgram(shader->ID);
    int viewLocation = glGetUniformLocation(shader->ID,"view");
    viewLocations[shader->ID] = viewLocation;
    int viewPosition = glGetUniformLocation(shader->ID,"viewPos");
    viewPositions[shader->ID] = viewPosition;
    }
}




bool CameraSystem::update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        unsigned int cameraID,CameraComponent& cameraComponent, float dt){
            //std::cout << transformComponents[cameraID].position.x<<"\n";
            //std::cout << transformComponents[cameraID].position.y<<"\n";
            //std::cout << transformComponents[cameraID].position.z<<"\n";

            glm::vec3& pos = transformComponents[cameraID].position;
            //glm::quat& rotation = transformComponents[cameraID].quaternion;
            glm::quat& rotation = transformComponents[cameraID].quaternion;




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
            for (auto shader:shaders){
            glUseProgram(shader->ID);//maybe not needed?
            glUniformMatrix4fv(viewLocations[shader->ID],1,GL_FALSE,glm::value_ptr(view));
            glUniform3fv(viewPositions[shader->ID],1,glm::value_ptr(pos));
            }



    //Keys
    glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(window,GLFW_KEY_P) == GLFW_PRESS){
        std::cout 
        << pos[0]<< ","
        << pos[1]<< ","
        << pos[2]<< " "

         <<eulers[0] <<","
         <<eulers[1] <<","
         <<eulers[2] <<"\n";
    }

    if (glfwGetKey(window,GLFW_KEY_M) == GLFW_PRESS){

            if (drawMode==0 ){
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                drawMode = 1;
            }else if (drawMode == 1){

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            drawMode = 0;
            }

    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dPos.x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dPos.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dPos.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dPos.y += 1.0f;
    }
    if (glm::length(dPos) > 0.1f) {
        dPos = glm::normalize(dPos);
        pos += 0.1f * dPos.x * forwards;
        pos += 0.1f * dPos.y * right;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return true;
    }
    
    glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwSetCursorPos(window, 320.0, 240.0);
    glfwPollEvents();

    dEulers.z = -0.1f * static_cast<float>(mouse_x - SCREENWIDTH/2);
    dEulers.y = -0.1f * static_cast<float>(mouse_y - SCREENHEIGHT/2);

    glm::quat rotateZ =  glm::angleAxis(dEulers.z*.01f,glm::vec3{0.0f,0.0f,1.0f});
    glm::quat rotateY =  glm::angleAxis(dEulers.y*.01f,glm::vec3{0.0f,1.0f,0.0f});

    rotation*=rotateY*rotateZ;

    //std::cout <<glm::to_string(eulers);
    //std::cout << "rot: " << glm::to_string(360.0f*glm::eulerAngles(rotation)/TWO_PI) << "\n";


    eulers.y = fminf(89.0f, fmaxf(-89.0f, eulers.y + dEulers.y));

    eulers.z += dEulers.z;
    if (eulers.z > 360) {
        eulers.z -= 360;
    }
    else if (eulers.z < 0) {
        eulers.z += 360;
    }

    return false;
    //Mouse
    // glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
    // double mouse_x, mouse_y;
    // glfwGetCursorPos(window, &mouse_x, &mouse_y);
    // glfwSetCursorPos(window, 320.0, 240.0);
    // glfwPollEvents();

    // dEulers.z = -0.1f * static_cast<float>(mouse_x - 320.0);
    // dEulers.y = -0.1f * static_cast<float>(mouse_y - 240.0);

    // eulers.y = fminf(89.0f, fmaxf(-89.0f, eulers.y + dEulers.y));

    // eulers.z += dEulers.z;
    // if (eulers.z > 360) {
    //     eulers.z -= 360;
    // }
    // else if (eulers.z < 0) {
    //     eulers.z += 360;
    // }

        }


