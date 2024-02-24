#include "factory.h"


Factory::Factory(
    std::unordered_map<unsigned int,TransformComponent>& transformComponents,
    std::unordered_map<unsigned int,RenderComponent>& renderComponents,
    std::unordered_map<unsigned int,PhysicsComponent>& physicsComponents):
    transformComponents(transformComponents),
    renderComponents(renderComponents),
    physicsComponents(physicsComponents)
    {



    } 

Factory::~Factory(){
    glDeleteBuffers(VBOs.size(),VBOs.data());
    glDeleteVertexArrays(VAOs.size(),VAOs.data());

}

unsigned int Factory::make_camera(glm::vec3 position,glm::vec3 eulers){

TransformComponent transform;
transform.position = position;
transform.eulers = eulers;
transformComponents[entities_made] = transform;

return entities_made++;

}


void Factory::make_cube(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity){

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = {0.0f,0.0f,0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;

    RenderComponent render = make_cube_mesh({.25f,.25f,.25f});
    renderComponents[entities_made++] = render;

}
RenderComponent Factory::make_cube_mesh(glm::vec3 size){
    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
         l,  w, -h, 1.0f, 1.0f,1.0f,
         l, -w, -h, 1.0f, 0.0f,1.0f,
        -l, -w, -h, 0.0f, 0.0f,1.0f,
        -l, -w, -h, 0.0f, 0.0f,1.0f,
        -l,  w, -h, 0.0f, 1.0f,1.0f,
         l,  w, -h, 1.0f, 1.0f,1.0f,

        -l, -w,  h, 0.0f, 0.0f,1.0f,
         l, -w,  h, 1.0f, 0.0f,1.0f,
         l,  w,  h, 1.0f, 1.0f,1.0f,
         l,  w,  h, 1.0f, 1.0f,1.0f,
        -l,  w,  h, 0.0f, 1.0f,1.0f,
        -l, -w,  h, 0.0f, 0.0f,1.0f,

        -l,  w,  h, 1.0f, 1.0f,1.0f,
        -l,  w, -h, 1.0f, 0.0f,1.0f,
        -l, -w, -h, 0.0f, 0.0f,1.0f,
        -l, -w, -h, 0.0f, 0.0f,1.0f,
        -l, -w,  h, 0.0f, 1.0f,1.0f,
        -l,  w,  h, 1.0f, 1.0f,1.0f,

         l, -w, -h, 0.0f, 0.0f,1.0f,
         l,  w, -h, 1.0f, 0.0f,1.0f,
         l,  w,  h, 1.0f, 1.0f,1.0f,
         l,  w,  h, 1.0f, 1.0f,1.0f,
         l, -w,  h, 0.0f, 1.0f,1.0f,
         l, -w, -h, 0.0f, 0.0f,1.0f,

        -l, -w, -h, 0.0f, 0.0f,1.0f,
         l, -w, -h, 1.0f, 0.0f,1.0f,
         l, -w,  h, 1.0f, 1.0f,  1.0f,
         l, -w,  h, 1.0f, 1.0f,1.0f,
        -l, -w,  h, 0.0f, 1.0f,1.0f,
        -l, -w, -h, 0.0f, 0.0f,1.0f,

         l,  w,  h, 1.0f, 1.0f,1.0f,
         l,  w, -h, 1.0f, 0.0f,1.0f,
        -l,  w, -h, 0.0f, 0.0f, 1.0f,
        -l,  w, -h, 0.0f, 0.0f,1.0f,
        -l,  w,  h, 0.0f, 1.0f,1.0f,
         l,  w,  h, 1.0f, 1.0f,1.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1,&VAO);
    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
        vertices.data(), GL_STATIC_DRAW);

        //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
    glEnableVertexAttribArray(1);

    RenderComponent record;
    record.VAO = VAO;
    record.vertexCount = 36;
    return record;

}