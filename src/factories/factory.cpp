#include "factory.h"

Factory::Factory(
    std::unordered_map<unsigned int, TransformComponent> &transformComponents,
    std::unordered_map<unsigned int, RenderComponent> &renderComponents,
    std::unordered_map<unsigned int, PhysicsComponent> &physicsComponents,
    std::vector<Shader *> shaders) : transformComponents(transformComponents),
                                     renderComponents(renderComponents),
                                     physicsComponents(physicsComponents), shaders(shaders)
{
}

Factory::~Factory()
{
    glDeleteBuffers(EBOs.size(), EBOs.data());
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
}

unsigned int Factory::make_camera(glm::vec3 position, glm::vec3 eulers)
{

    make_transform_component(entities_made, position, eulers);

    return entities_made++;
}

void Factory::make_sphere(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);

    MeshVectorComponent vectors = make_sphere_mesh(100);
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_DST_SHADER);
    renderComponents[entities_made++] = render;
}

void Factory::make_sphere_test(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);
    MeshVectorComponent vectors = make_sphere_mesh_test(100);
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_DST_SHADER);
    renderComponents[entities_made++] = render;
}

void Factory::make_octahedron(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);

    MeshVectorComponent vectors = make_octahedron_mesh(25);
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_DST_SHADER);
    renderComponents[entities_made++] = render;
}

float Factory::lerp(float a, float b, float t)
{
    return (a * (1.0 - t)) + (b * t);
}

void Factory::make_sphere_to_octahedron(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{

    MeshVectorComponent octVectors = make_octahedron_mesh(100);
    MeshVectorComponent sphereVectors = make_sphere_mesh(100);
    const int frameCount = 24;

    for (int frameIndex = 1; frameIndex <= frameCount; frameIndex++)
    {

        make_transform_component(entities_made, position, eulers);
        make_physics_component(entities_made, eulerVelocity);
        std::vector<float> vertexVector = {};
        for (unsigned int i = 0; i < octVectors.vertices.size(); i++)
        {
            float lerpedVertex = lerp(octVectors.vertices[i], sphereVectors.vertices[i], (float)frameIndex / frameCount);
            vertexVector.push_back(lerpedVertex);
        }

        RenderComponent render = make_shape_colored({vertexVector, octVectors.indices}, frameIndex, LIGHT_DST_SHADER);
        renderComponents[entities_made++] = render;
    }
    // bounce back
    for (int frameIndex = frameCount; frameIndex > 0; frameIndex--)
    {
        make_transform_component(entities_made, position, eulers);
        make_physics_component(entities_made, eulerVelocity);

        std::vector<float> vertexVector = {};
        for (unsigned int i = 0; i < octVectors.vertices.size(); i++)
        {
            float lerpedVertex = lerp(octVectors.vertices[i], sphereVectors.vertices[i], (float)frameIndex / frameCount);
            vertexVector.push_back(lerpedVertex);
        }
        RenderComponent render = make_shape_colored({vertexVector, octVectors.indices}, 2 * frameCount - frameIndex + 1, LIGHT_DST_SHADER);
        renderComponents[entities_made++] = render;
    }
}





RenderComponent Factory::make_shape_colored(MeshVectorComponent details, unsigned int frameCount, unsigned int shaderIndex)
{

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
    unsigned int EBO = 0;
    if (details.indices.size() > 0)
    {
        glGenBuffers(1, &EBO);
        EBOs.push_back(EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, details.indices.size() * sizeof(unsigned int), details.indices.data(), GL_STATIC_DRAW);
    }
    // std::cout << EBO << " ECO\n";
    //  position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void *)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void *)24);
    glEnableVertexAttribArray(2);

    RenderComponent record = {};
    record.VAO = VAO;
    record.VBO = VBO;
    record.EBO = EBO;
    record.frameCount = frameCount;
    record.vertexCount = details.vertices.size();
    record.indexCount = details.indices.size();
    record.shaderId = shaders[shaderIndex]->ID;
    return record;
}
TransformComponent* Factory::make_transform_component(unsigned int entityID, glm::vec3 position, glm::vec3 eulers)
{
    TransformComponent transform;
    transform.position = position;
    transform.eulers = eulers;
    transformComponents[entityID] = transform;
    return &transformComponents[entityID];
}
void Factory::make_physics_component(unsigned int entityID, glm::vec3 eulerVelocity)
{
    PhysicsComponent physics = {};
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physicsComponents[entityID] = physics;
}
void Factory::make_physics_component_soft(unsigned int entityID, glm::vec3 eulerVelocity,TransformComponent* transform)
{
    PhysicsComponent physics = {};
    physics.velocity = {0.0f, 0.0f, 0.0f};
    physics.eulerVelocity = eulerVelocity;
    physics.softbody = new SoftBody(transform);
    physicsComponents[entityID] = physics;
}


RenderComponent Factory::make_shape_colored_soft(MeshVectorComponent details, unsigned int frameCount, unsigned int shaderIndex)
{

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    VAOs.push_back(VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, details.vertices.size() * sizeof(float),
                 details.vertices.data(), GL_DYNAMIC_DRAW);
    unsigned int EBO = 0;
    if (details.indices.size() > 0)
    {
        glGenBuffers(1, &EBO);
        EBOs.push_back(EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, details.indices.size() * sizeof(unsigned int), details.indices.data(), GL_DYNAMIC_DRAW);
    }
    // std::cout << EBO << " ECO\n";
    //  position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void *)0);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void *)12);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void *)24);
    glEnableVertexAttribArray(2);

    
    RenderComponent record = {};
    record.VAO = VAO;
    record.VBO = VBO;
    record.EBO = EBO;
    record.frameCount = frameCount;
    record.vertexCount = details.vertices.size();
    record.indexCount = details.indices.size();
    record.shaderId = shaders[shaderIndex]->ID;
    return record;
}

void Factory::make_ground(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);

    MeshVectorComponent vectors = make_cube_mesh({100.0f, 100.0f, 0.0f});
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_DST_SHADER);
    renderComponents[entities_made++] = render;
}

void Factory::make_cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);

    MeshVectorComponent vectors = make_cube_mesh({0.5f, 0.5f, 0.5f});
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_DST_SHADER);
    renderComponents[entities_made++] = render;
}

void Factory::make_cube_soft(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{
    TransformComponent* transform = make_transform_component(entities_made, position, eulers);
    make_physics_component_soft(entities_made, eulerVelocity,transform);

    MeshVectorComponent vectors = make_cube_mesh_soft({0.5f, 0.5f, 0.5f});
    RenderComponent render = make_shape_colored_soft(vectors, 0, LIGHT_SRC_SHADER);
    renderComponents[entities_made++] = render;
}

void Factory::make_cube_light(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
{

    make_transform_component(entities_made, position, eulers);
    make_physics_component(entities_made, eulerVelocity);

    MeshVectorComponent vectors = make_cube_mesh({0.5f, 0.5f, 0.5f});
    RenderComponent render = make_shape_colored(vectors, 0, LIGHT_SRC_SHADER);
    renderComponents[entities_made++] = render;
}
