#pragma once
#include "../config.h"

//unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath);
//unsigned int make_module(const std::string &filepath, unsigned int module_type);

class Shader
{
    public:
        unsigned int ID;

        Shader(const std::string &vertex_filepath, const std::string &fragment_filepath);

        

        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:

        unsigned int make_module(const std::string &filepath, unsigned int module_type);

};