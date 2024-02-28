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

void Factory::make_circle(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity)
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

    RenderComponent render = make_sphere_mesh({1.0f, 1.0f, 1.0f});
    renderComponents[entities_made++] = render;
}

unsigned int Factory::triangle_number(unsigned int n)
{
    return n * (n + 1) / 2;
}

RenderComponent Factory::make_sphere_mesh(glm::vec3 plane)
{

    int tCount = 10;

    std::vector<float> vertices = {

    };
    std::vector<unsigned int> indices = {};
    // upside down
    unsigned int offsetIndex = -1;


    for (int p1 = -1; p1 < 2; (p1 = p1 + 2))
    {
        for (int p2 = -1; p2 < 2; (p2 = p2 + 2))
        {
            for (int p3 = -1; p3 < 2; (p3 = p3 + 2))
            {
                offsetIndex++;
                int offset = offsetIndex*triangle_number(tCount+1);
                std::cout << p1 << p2 << p3 << "\n";
                int sign = p1 * p2 * p3;

                for (int dx = 0; dx < tCount; dx++)
                {
                    for (int dy = 1; dy < tCount - dx; dy++)
                    {
                        int index1, index2, index3;

                        index1 = dy + triangle_number(tCount + 1) - triangle_number(tCount + 1 - dx)+offset;
                        index2 = triangle_number(tCount + 1) - triangle_number(tCount - dx) + dy+offset;
                        index3 = triangle_number(tCount + 1) - triangle_number(tCount - dx) + dy - 1+offset;
                        if (sign < 0)
                        {

                            indices.push_back(index1);
                            indices.push_back(index2);
                            indices.push_back(index3);
                        }
                        else
                        {
                            // reverse
                            indices.push_back(index3);
                            indices.push_back(index2);
                            indices.push_back(index1);
                        }
                    }
                }
                // normal
                for (int dx = 0; dx < tCount; dx++)
                {
                    for (int dy = 0; dy < tCount - dx; dy++)
                    {
                        int index1, index2, index3;

                        index1 = dy + triangle_number(tCount + 1) - triangle_number(tCount + 1 - dx) + offset;
                        index2 = dy + triangle_number(tCount + 1) - triangle_number(tCount + 1 - dx)  + 1 +offset;
                        index3 = triangle_number(tCount + 1) - triangle_number(tCount - dx) + dy+offset;
                        if (sign < 0)
                        {

                            indices.push_back(index1);
                            indices.push_back(index2);
                            indices.push_back(index3);
                        }
                        else
                        {
                            // reverse
                            indices.push_back(index3);
                            indices.push_back(index2);
                            indices.push_back(index1);
                        }
                    }
                }

                for (int dx = 0; dx <= tCount; dx++)
                {
                    for (int dy = 0; dy <= tCount; dy++)
                    {
                        float x = dx / (float)tCount;
                        float y = dy / (float)tCount;
                        float z = 1 - x - y;

                        glm::vec3 normal = glm::normalize(glm::vec3({x, y, z}));
                        x = p1 * normal[0];
                        y = p2 * normal[1];
                        z = p3 * normal[2];

                        if (p3 * z < -0.01f)
                        { // floating point yikes
                            continue;
                        }
                        vertices.push_back(x);
                        vertices.push_back(y);
                        vertices.push_back(z);
                        vertices.push_back(dy/(float)tCount);
                        vertices.push_back(dx/(float)tCount);
                        vertices.push_back(1.0f);
                    }
                }
            }
        }
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