#include "gltf_mesh.h"

GLTFStaticMesh::GLTFStaticMesh(const char *filename, float scale)
{
tinygltf::TinyGLTF loader;
std::string error;
std::string warning;

loader.LoadASCIIFromFile(&model,&error,&warning,filename);

if(!warning.empty() || !error.empty()){
    std::cout << "Warning: " << warning <<std::endl;
    std::cout << "Error: " << error <<std::endl;
}
this->scale = scale;
VAO_and_EBOs = bindModel();

}

GLTFStaticMesh::~GLTFStaticMesh()
{
}

std::pair<unsigned int, std::map<int,unsigned int>> GLTFStaticMesh::bindModel(){
}



void GLTFStaticMesh::bindModelNodes(std::map<int, unsigned int> &EBOs, tinygltf::Node &node)
{
}
void GLTFStaticMesh::bindMesh(std::map<int, unsigned int> &EBOs, tinygltf::Mesh &mesh)
{
}
