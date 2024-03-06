#pragma once
#include "../config.h"
#include "../components/render_component.h"
#include "../components/transform_component.h"
#include "../components/light_component.h"
#include "../view/shader.h"


class RenderSystem{
    public:
    RenderSystem(std::vector<Shader*> shaders, GLFWwindow* window);

    void update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        std::unordered_map<unsigned int,RenderComponent> &renderComponents,
        std::unordered_map<unsigned int,LightComponent> &lightComponents
        );


    private:
    //unsigned int modelLocation;
    std::unordered_map<unsigned int, unsigned int> modelLocations;
    GLFWwindow* window;
    std::vector<Shader*> shaders;
};