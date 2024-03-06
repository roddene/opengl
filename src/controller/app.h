#pragma once
#include "../config.h"

#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/render_component.h"
#include "../components/transform_component.h"
#include "../components/light_component.h"

#include "../systems/camera_system.h"
#include "../systems/motion_system.h"
#include "../systems/render_system.h"

#include "../view/shader.h"

class App{
    public:
    App();
    ~App();
    void run();
    unsigned int make_entity();
    unsigned int make_mesh(glm::vec3 size);
    unsigned int make_texture(const char* filename);
    void set_up_opengl();
    void make_systems();


    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    //Component
    std::unordered_map<unsigned int,TransformComponent> transformComponents;
    std::unordered_map<unsigned int, PhysicsComponent> physicsComponents;
    CameraComponent* cameraComponent;
    unsigned int cameraID;
    std::unordered_map<unsigned int,RenderComponent> renderComponents;
    std::unordered_map<unsigned int,LightComponent> lightComponents;

    std::vector<Shader*> shaders;


    private:
    void set_up_glfw();

    unsigned int entity_count = 0;

    GLFWwindow* window;

    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    std::vector<unsigned int> textures;

    

    //System

    MotionSystem* motionSystem;
    CameraSystem* cameraSystem;
    RenderSystem* renderSystem;



};