#include "config.h"
#include "triangle_mesh.h"
#include "linear.h"
#include <filesystem>

    vec3 quad_position = {0.0f, 0.0f, 0.0f};
unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath);
unsigned int make_module(const std::string &filepath, unsigned int module_type);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
int main()
{
    vec3 camera_position = {0.0f,-.1f,2.0f};
    vec3 camera_target = {0.0f, 0.0f, 0.0f};


    std::cout << std::filesystem::current_path() << '\n';

    GLFWwindow *window;

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }
    glClearColor(1, .5, 1, 1);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    TriangleMesh *triangle = new TriangleMesh();

    unsigned int shader = make_shader("./src/shaders/vertex.txt", "./src/shaders/fragment.txt");

    glUseProgram(shader);

    unsigned int model_location = glGetUniformLocation(shader, "model");
    unsigned int view_location  = glGetUniformLocation(shader,"view");  
    unsigned int proj_location  = glGetUniformLocation(shader,"projection");  

    mat4 view = create_look_at(camera_position,camera_target);
    glUniformMatrix4fv(view_location,1,GL_FALSE,view.entries);

    //1.0f represents screen aspect ratio.
    mat4 projection = create_perspective_projection(45.0f,640.0f/480.0f,0.1f,10.0f);
    glUniformMatrix4fv(proj_location,1,GL_FALSE,projection.entries);

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        mat4 model = create_model_transform(10*glfwGetTime(), quad_position);
        //mat4 model = create_model_transform(0, quad_position);

        glUniformMatrix4fv(model_location, 1, GL_FALSE, model.entries);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        triangle->draw();
        glfwSwapBuffers(window);
    }
    glDeleteProgram(shader);
    delete triangle;
    glfwTerminate();

    return 0;
}

unsigned int make_shader(const std::string &vertex_filepath, const std::string &fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    for (int i = 0; i < modules.size(); i++)
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
    std::cout<< std::filesystem::current_path() << "\n";
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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        quad_position.entries[0] += 0.01f;
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        quad_position.entries[0] += -0.01f;
    }
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        quad_position.entries[1] += 0.01f;
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        quad_position.entries[1] += -0.01f;
        std::cout<< quad_position.entries[1]<<"\n";
    }if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
    {
        quad_position.entries[2] += 0.01f;
    }if (key == GLFW_KEY_E && action == GLFW_REPEAT)
    {
        quad_position.entries[2] += -0.01f;
    }
}
