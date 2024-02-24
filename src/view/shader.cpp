#include "shader.h"
unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    for (unsigned int i = 0; i < modules.size(); i++)
        std::cout << modules.at(i) << ' ';
    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules)
    {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader linking Error\n"
                  << errorLog << std::endl;
    }
    for (unsigned int shaderModule : modules)
    {
        glDeleteShader(shaderModule);
    }
    return shader;
}

unsigned int make_module(const std::string &filepath, unsigned int module_type)
{
    printf("l:%s\n", filepath.c_str());
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file, line))
    {
        bufferedLines << line << '\n';
    }
    std::string shaderSource = bufferedLines.str();
    const char *shaderSrc = shaderSource.c_str();
    printf("here:%d \n %s", module_type, shaderSrc);
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Module Compilation Error\n"
                  << errorLog << std::endl;
    }
    return shaderModule;
}