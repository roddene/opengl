#include "render_system.h"

    RenderSystem::RenderSystem(unsigned int shader, GLFWwindow* window){
        glUseProgram(shader);
        modelLocation = glGetUniformLocation(shader,"model");;
        this->window = window;
    }

void RenderSystem::update(
        
    
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        std::unordered_map<unsigned int,RenderComponent> &renderComponents){

            int frameRate = 48;
            
            int frame = (int)(glfwGetTime()*frameRate) %frameRate+1;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::pair<unsigned int, RenderComponent> entity: renderComponents){
            TransformComponent& transform = transformComponents[entity.first];
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,transform.position);
            model = glm::rotate(model,glm::radians(transform.eulers.z),{0.0f,0.0f,1.0f});
            glUniformMatrix4fv(modelLocation,1,GL_FALSE,glm::value_ptr(model));
            //std::cout << entity.second.mesh;
        glBindVertexArray(entity.second.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,entity.second.EBO);
        if (entity.second.EBO){
            
            //std::cout <<entity.second.indexCount << "\n";
            if (!entity.second.frameCount){ //if frame count is 0 then always show
            glDrawElements(GL_TRIANGLES,entity.second.indexCount,GL_UNSIGNED_INT,0);
            }else{
                if (entity.second.frameCount == frame){
            glDrawElements(GL_TRIANGLES,entity.second.indexCount,GL_UNSIGNED_INT,0);
                }
            }
        }else{
        glDrawArrays(GL_TRIANGLES,0,entity.second.vertexCount);
        }
        
        //std::cout << model;
        }
        glfwSwapBuffers(window);


        }

