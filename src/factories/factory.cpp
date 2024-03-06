#include "factory.h"


Factory::Factory(
    std::unordered_map<unsigned int, TransformComponent> &transformComponents,
    std::unordered_map<unsigned int, RenderComponent> &renderComponents,
    std::unordered_map<unsigned int, PhysicsComponent> &physicsComponents,
    std::vector<Shader*> shaders) : transformComponents(transformComponents),
                                                                             renderComponents(renderComponents),
                                                                             physicsComponents(physicsComponents),shaders(shaders)
{
}

Factory::~Factory()
{
    glDeleteBuffers(EBOs.size(),EBOs.data());
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
}

unsigned int Factory::make_camera(glm::vec3 position, glm::vec3 eulers)
{

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    return entities_made++;
}

void Factory::make_sphere(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;
    // RenderComponent render = make_sphere_mesh({-1.0f,1.0f,1.0f});
    //     renderComponents[entities_made++] = render;
    //  RenderComponent render1 = make_sphere_mesh({-1.0f,-1.0f,-1.0f});
    //      renderComponents[entities_made++] = render1;
    MeshVectorComponent vectors = make_sphere_mesh(10);
    RenderComponent render = make_shape_colored(vectors,0);
    renderComponents[entities_made++] = render;
}

void Factory::make_octahedron(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;
    // RenderComponent render = make_sphere_mesh({-1.0f,1.0f,1.0f});
    //     renderComponents[entities_made++] = render;
    //  RenderComponent render1 = make_sphere_mesh({-1.0f,-1.0f,-1.0f});
    //      renderComponents[entities_made++] = render1;
    MeshVectorComponent vectors = make_octahedron_mesh(10);
    RenderComponent render = make_shape_colored(vectors,0);
    renderComponents[entities_made++] = render;
}

float Factory::lerp(float a,float b, float t){
    return (a*(1.0-t)) + (b*t);
}


void Factory::make_sphere_to_octahedron(glm::vec3 position,glm::vec3 eulers,glm::vec3 eulerVelocity){


    MeshVectorComponent octVectors = make_octahedron_mesh(100);
    MeshVectorComponent sphereVectors = make_sphere_mesh(100);
    const int frameCount = 24;

    for (int frameIndex = 1; frameIndex<=frameCount;frameIndex++){
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;
    std::vector<float> vertexVector = {};
    for(unsigned int i = 0;i<octVectors.vertices.size();i++){
        float lerpedVertex = lerp(octVectors.vertices[i],sphereVectors.vertices[i],(float)frameIndex/frameCount);
        vertexVector.push_back(lerpedVertex);



    }     

    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;
 
    RenderComponent render = make_shape_colored({vertexVector,octVectors.indices},frameIndex);
    renderComponents[entities_made++] = render;
    }
    //bounce back
    for (int frameIndex = frameCount; frameIndex>0;frameIndex--){
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;
    std::vector<float> vertexVector = {};
    for(unsigned int i = 0;i<octVectors.vertices.size();i++){
        float lerpedVertex = lerp(octVectors.vertices[i],sphereVectors.vertices[i],(float)frameIndex/frameCount);
        vertexVector.push_back(lerpedVertex);
    } 
    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;
   
    RenderComponent render = make_shape_colored({vertexVector,octVectors.indices},2*frameCount-frameIndex+1);
    renderComponents[entities_made++] = render;
    }

}





RenderComponent Factory::make_shape_colored(MeshVectorComponent details,unsigned int frameCount){

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, details.vertices.size() * sizeof(float),
                 details.vertices.data(), GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs.push_back(EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, details.indices.size() * sizeof(unsigned int), details.indices.data(), GL_STATIC_DRAW);
    //std::cout << EBO << " ECO\n";
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void *)12);
    glEnableVertexAttribArray(1);

    RenderComponent record = {};
    record.VAO = VAO;
    record.VBO = VBO;
    record.EBO = EBO;
    record.frameCount = frameCount;
    record.vertexCount = details.vertices.size();
    record.indexCount = details.indices.size();
    record.shaderId = shaders[0]->ID;
    return record;
}


void Factory::make_cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{

    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entities_made] = transform;

    PhysicsComponent physics;
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entities_made] = physics;

    RenderComponent render = make_cube_mesh({.25f, .25f, .25f});
    renderComponents[entities_made++] = render;
}
RenderComponent Factory::make_cube_mesh(glm::vec3 size)
{
    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
        l, w, -h, 1.0f, 1.0f, 1.0f,
        l, -w, -h, 1.0f, 0.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,
        -l, w, -h, 0.0f, 1.0f, 1.0f,
        l, w, -h, 1.0f, 1.0f, 1.0f,

        -l, -w, h, 0.0f, 0.0f, 1.0f,
        l, -w, h, 1.0f, 0.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,
        -l, w, h, 0.0f, 1.0f, 1.0f,
        -l, -w, h, 0.0f, 0.0f, 1.0f,

        -l, w, h, 1.0f, 1.0f, 1.0f,
        -l, w, -h, 1.0f, 0.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,
        -l, -w, h, 0.0f, 1.0f, 1.0f,
        -l, w, h, 1.0f, 1.0f, 1.0f,

        l, -w, -h, 0.0f, 0.0f, 1.0f,
        l, w, -h, 1.0f, 0.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,
        l, -w, h, 0.0f, 1.0f, 1.0f,
        l, -w, -h, 0.0f, 0.0f, 1.0f,

        -l, -w, -h, 0.0f, 0.0f, 1.0f,
        l, -w, -h, 1.0f, 0.0f, 1.0f,
        l, -w, h, 1.0f, 1.0f, 1.0f,
        l, -w, h, 1.0f, 1.0f, 1.0f,
        -l, -w, h, 0.0f, 1.0f, 1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,

        l, w, h, 1.0f, 1.0f, 1.0f,
        l, w, -h, 1.0f, 0.0f, 1.0f,
        -l, w, -h, 0.0f, 0.0f, 1.0f,
        -l, w, -h, 0.0f, 0.0f, 1.0f,
        -l, w, h, 0.0f, 1.0f, 1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f};

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

   
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void *)12);
    glEnableVertexAttribArray(1);

    // unsigned int lightVAO;
    // glGenVertexArrays(1, &lightVAO);
    // glBindVertexArray(lightVAO);

    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);


        glUseProgram(shaders[1]->ID);//maybe not needed?
        int lightColor = glGetUniformLocation(shaders[1]->ID,"lightColor");
        glm::vec3 light = {0.0f,0.0f,0.0f};
        glUniform3fv(lightColor,1,glm::value_ptr(light));



    RenderComponent record = {};
    record.VAO = VAO;
    record.vertexCount = 36;
    std::cout << "here";
    record.shaderId = shaders[1]->ID;
    //std::cout <<this->shaders[0];
    return record;
}