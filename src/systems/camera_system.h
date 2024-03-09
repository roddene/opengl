#pragma once
#include "../config.h"
#include "../components/camera_component.h"
#include "../components/transform_component.h"
#include "../view/shader.h"

class CameraSystem{
    public:
    CameraSystem(std::vector<Shader*> shaders, GLFWwindow* window);

    bool update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        unsigned int cameraID,CameraComponent& cameraComponent, float dt);


    private:
    //unsigned int viewLocation;
    glm::vec3 global_up = {0.0f,0.0f,1.0f};
    GLFWwindow* window;
    unsigned int drawMode = 0;
    std::vector<Shader*> shaders;
    
    std::unordered_map<unsigned int, unsigned int> viewLocations;
    std::unordered_map<unsigned int, unsigned int> viewPositions;
};