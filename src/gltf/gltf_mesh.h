#pragma once
#include "../config.h"
#include <TinyGLTF/tiny_gltf.h>

class GLTFStaticMesh {
    public:
    tinygltf::Model model;
    std::pair<unsigned int, std::map<int,unsigned int>> VAO_and_EBOs;
    float scale;


    GLTFStaticMesh(const char* filename,float scale);
    ~GLTFStaticMesh();
    std::pair<unsigned int, std::map<int,unsigned int>> bindModel();

    private:
    void bindModelNodes(std::map<int,unsigned int>& EBOs, tinygltf::Node& node);
    void bindMesh(std::map<int,unsigned int>& EBOs, tinygltf::Mesh& mesh);



};