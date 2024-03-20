#include "render_system.h"

RenderSystem::RenderSystem(std::vector<Shader *> shaders, GLFWwindow *window)
{
    for (auto shader : shaders)
    {
        glUseProgram(shader->ID);
        int modelLocation = glGetUniformLocation(shader->ID, "model");
        modelLocations[shader->ID] = modelLocation;
    }
    this->window = window;
    this->shaders = shaders;
}

void RenderSystem::update(

    std::unordered_map<unsigned int, TransformComponent> &transformComponents,
    std::unordered_map<unsigned int, RenderComponent> &renderComponents,
    std::unordered_map<unsigned int, LightComponent> &lightComponents)
{

    int frameRate = 48;

    int frame = (int)(glfwGetTime() * frameRate / 2) % frameRate + 1;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::pair<unsigned int, RenderComponent> entity : renderComponents)
    {
        TransformComponent &transform = transformComponents[entity.first];
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, glm::radians(transform.eulers.z), {0.0f, 0.0f, 1.0f});
        model = glm::rotate(model, glm::radians(transform.eulers.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(transform.eulers.x), {1.0f, 0.0f, 0.0f});

        glUseProgram(shaders[0]->ID); // maybe not needed?
        int lightColor = glGetUniformLocation(shaders[0]->ID, "lightColor");
        glm::vec3 light = {1.0f, 1.0f, 1.0f};
        glUniform3fv(lightColor, 1, glm::value_ptr(light));
        glm::vec3 lightPos = {3.0f,2.0f,3.0f};
        shaders[0]->setVec3("lightPos",lightPos);

        int shaderId = entity.second.shaderId;
        glUseProgram(shaderId);
        glUniformMatrix4fv(modelLocations[shaderId], 1, GL_FALSE, glm::value_ptr(model));
        // std::cout << entity.second.mesh;
        glBindVertexArray(entity.second.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity.second.EBO);
        if (entity.second.EBO)
        {

            // std::cout <<entity.second.indexCount << "\n";
            if (!entity.second.frameCount)
            { // if frame count is 0 then always show
                glDrawElements(GL_TRIANGLES, entity.second.indexCount, GL_UNSIGNED_INT, 0);
            }
            else
            {
                if (entity.second.frameCount == frame)
                {
                    glDrawElements(GL_TRIANGLES, entity.second.indexCount, GL_UNSIGNED_INT, 0);
                }
            }
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, entity.second.vertexCount);
        }

        // std::cout << model;
    }
    glfwSwapBuffers(window);
}
