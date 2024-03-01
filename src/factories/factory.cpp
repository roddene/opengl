#include "factory.h"


Factory::Factory(
    std::unordered_map<unsigned int, TransformComponent> &transformComponents,
    std::unordered_map<unsigned int, RenderComponent> &renderComponents,
    std::unordered_map<unsigned int, PhysicsComponent> &physicsComponents) : transformComponents(transformComponents),
                                                                             renderComponents(renderComponents),
                                                                             physicsComponents(physicsComponents)
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
   
    RenderComponent render = make_shape_colored({vertexVector,octVectors.indices},2*frameCount-frameIndex);
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
    std::cout << EBO << " ECO\n";
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void *)12);
    glEnableVertexAttribArray(1);

    RenderComponent record = {};
    record.VAO = VAO;
    record.VBO = VBO;
    record.EBO = EBO;
    record.frameCount = frameCount;
    record.vertexCount = details.vertices.size();
    record.indexCount = details.indices.size();
    return record;
}

RenderComponent Factory::make_circle_mesh(glm::vec3 size)
{

    int tCount = 10;

    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // center

    };
    std::vector<unsigned int> indices = {};
    for (int i = 0; i <= tCount; i++)
    {
        float x = i / (float)tCount;
        float y = 1 - x;
        glm::vec2 normal = glm::normalize(glm::vec2{x, y});
        x = normal[0];
        y = normal[1];
        std::cout << x << "," << y << "\n";
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // z
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

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

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs.push_back(EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    std::cout << EBO << " ECO\n";
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void *)12);
    glEnableVertexAttribArray(1);

    RenderComponent record = {};
    record.VAO = VAO;
    record.EBO = EBO;
    record.vertexCount = 3;
    record.indexCount = indices.size();
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

    RenderComponent record = {};
    record.VAO = VAO;
    record.vertexCount = 36;
    return record;
}