#include "cube.h"

MeshVectorComponent make_cube_mesh(glm::vec3 size)
{
    float l = size.x;
    float w = size.y;
    float h = size.z;

    std::vector<float> vertices = {
        l, w, -h, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,-1.0f,
        l, -w, -h, 1.0f, 0.0f, 1.0f,0.0f,0.0f,-1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,0.0f,0.0f,-1.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,0.0f,0.0f,-1.0f,
        -l, w, -h, 0.0f, 1.0f, 1.0f,0.0f,0.0f,-1.0f,
        l, w, -h, 1.0f, 1.0f, 1.0f,0.0f,0.0f,-1.0f,

        -l, -w, h, 0.0f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
        l, -w, h, 1.0f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,0.0f,0.0f,1.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,0.0f,0.0f,1.0f,
        -l, w, h, 0.0f, 1.0f, 1.0f,0.0f,0.0f,1.0f,
        -l, -w, h, 0.0f, 0.0f, 1.0f,0.0f,0.0f,1.0f,

        -l, w, h, 1.0f, 1.0f, 1.0f,-1.0f,0.0f,0.0f,
        -l, w, -h, 1.0f, 0.0f, 1.0f,-1.0f,0.0f,0.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,-1.0f,0.0f,0.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,-1.0f,0.0f,0.0f,
        -l, -w, h, 0.0f, 1.0f, 1.0f,-1.0f,0.0f,0.0f,
        -l, w, h, 1.0f, 1.0f, 1.0f,-1.0f,0.0f,0.0f,

        l, -w, -h, 0.0f, 0.0f, 1.0f,1.0f,0.0f,0.0f,
        l, w, -h, 1.0f, 0.0f, 1.0f,1.0f,0.0f,0.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,1.0f,0.0f,0.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,1.0f,0.0f,0.0f,
        l, -w, h, 0.0f, 1.0f, 1.0f,1.0f,0.0f,0.0f,
        l, -w, -h, 0.0f, 0.0f, 1.0f,1.0f,0.0f,0.0f,

        -l, -w, -h, 0.0f, 0.0f, 1.0f,0.0f,-1.0f,0.0f,
        l, -w, -h, 1.0f, 0.0f, 1.0f,0.0f,-1.0f,0.0f,
        l, -w, h, 1.0f, 1.0f, 1.0f,0.0f,-1.0f,0.0f,
        l, -w, h, 1.0f, 1.0f, 1.0f,0.0f,-1.0f,0.0f,
        -l, -w, h, 0.0f, 1.0f, 1.0f,0.0f,-1.0f,0.0f,
        -l, -w, -h, 0.0f, 0.0f, 1.0f,0.0f,-1.0f,0.0f,

        l, w, h, 1.0f, 1.0f, 1.0f,0.0f,1.0f,0.0f,
        l, w, -h, 1.0f, 0.0f, 1.0f,0.0f,1.0f,0.0f,
        -l, w, -h, 0.0f, 0.0f, 1.0f,0.0f,1.0f,0.0f,
        -l, w, -h, 0.0f, 0.0f, 1.0f,0.0f,1.0f,0.0f,
        -l, w, h, 0.0f, 1.0f, 1.0f,0.0f,1.0f,0.0f,
        l, w, h, 1.0f, 1.0f, 1.0f,0.0f,1.0f,0.0f};


    return {vertices,{}};
}