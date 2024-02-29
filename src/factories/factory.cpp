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

    // int res = 5;

    std::vector<float> vertices = {

     };
    std::vector<unsigned int> indices = {};


        unsigned int res = 100; // number of rows and columns
		float circleStep = glm::two_pi<float>() / (float)res; // angle step between cells
		float heightStep = glm::pi<float>() / (float)res; // height of row

		int row = 0;
		int noVertices = 0;
		float phi = -glm::half_pi<float>();
		float y = glm::sin(phi);
		float radius;

		for (; phi < HALF_PI + heightStep; phi += heightStep, row++) {
        
            
			 //y = glm::sin(phi);
			// radius = glm::cos(phi);
			int cell = 0;
            
            y = glm::sign(glm::sin(phi))* glm::acos(glm::cos(2*phi))/PI;
            radius = glm::sign(glm::cos(phi))*glm::acos(glm::cos(2*phi+PI))/PI;

			for (float th = 0; th < TWO_PI; th += circleStep, cell++) {
        
            
            
			vertices.push_back(radius*glm::sign(glm::cos(th))*glm::acos(glm::cos(2*th+PI))/PI);
            vertices.push_back(y);
           vertices.push_back(radius*glm::sign(glm::sin(th))*glm::acos(glm::cos(th*2))/PI);
       //vertices.push_back(glm::sign(costh));
       //vertices.push_back(th);
       //vertices.push_back(1-phi-th);

            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

				// add indices if not bottom row
				if (row)
				{
					int nextCell = (cell + 1) % res;
					indices.push_back((row - 1) * res + nextCell); // bottom right
					indices.push_back(noVertices - res); // bottom left
					indices.push_back(row * res + nextCell); // top right


					indices.push_back(noVertices - res); // bottom left
					indices.push_back(noVertices); // top left (this vertex)
					indices.push_back(row * res + nextCell); // top right
				}

				noVertices++;
			}
		}




    // float circleStep = glm::two_pi<float>()/(float) res;
    // float heightStep = glm::pi<float>()/(float) res;

    // int row = 0;
    // int vertexCount = 0;
    // float phi = -glm::half_pi<float>();
    // float y = glm::sin(phi);
    // float radius;

    // for (; phi<glm::half_pi<float>()+heightStep;phi+=heightStep,row++){
    //     y = glm::sin(phi);
    //     radius = glm::cos(phi);
    //     int cell = 0;
    //     for (float th = 0;th<glm::two_pi<float>();th+=circleStep,cell++){
    //         vertices.push_back(radius*glm::cos(th));
    //         vertices.push_back(y);
    //         vertices.push_back(radius*glm::sin(th));

    //         vertices.push_back(0.0f);
    //         vertices.push_back(1.0f);
    //         vertices.push_back(0.0f);

    //         if (row){
    //             int nextCell = (cell+1) %res;
    //             indices.push_back(vertexCount-res);
    //             indices.push_back((row-1)*res+nextCell);
    //             indices.push_back(row*res+nextCell);

    //             indices.push_back(vertexCount-res);
    //             indices.push_back(vertexCount);
    //             indices.push_back(row*res+nextCell);


    //         }
    //         vertexCount++;
    //     }



    // }


    


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