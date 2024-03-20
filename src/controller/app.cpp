#include "app.h"

App :: App()
{
    set_up_glfw();
}

App::~App()
{
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteVertexArrays(VAOs.size(), VAOs.data());
    for (auto shader:shaders){
    glDeleteProgram(shader->ID);
    }
    delete motionSystem;
    delete cameraSystem;
    delete renderSystem;

    glfwTerminate();
}

unsigned int App::make_entity()
{
    return entity_count++;
}


void App::run()
{
    while (!glfwWindowShouldClose(window))
    {


        motionSystem->update(transformComponents, physicsComponents, renderComponents,16.67f / 1000.0f);

        bool should_close = cameraSystem->update(
            transformComponents, cameraID, *cameraComponent, 16.67f / 1000.0f);

        if (should_close)
        {
            break;
        }
        renderSystem->update(transformComponents, renderComponents,lightComponents);
    }
}

void App::set_up_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
    }
}


void App::set_up_opengl(){
    std::cout << "setup!!!\n";

    glClearColor(1,.5f,1,1);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //glfwSetKeyCallback(window, key_callback);

    Shader* dst_shader = new Shader("./src/shaders/light_dst.vs", "./src/shaders/light_dst.fs");
    Shader* src_shader = new Shader("./src/shaders/light_src.vs", "./src/shaders/light_src.fs");
glm::mat4 projection = glm::perspective(
		45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    shaders.push_back(dst_shader);
   shaders.push_back(src_shader);
    for (auto shader:shaders){
    glUseProgram(shader->ID);
    unsigned int projLocation = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
    }
}

void App::make_systems(){
    std::cout <<"making systems\n";
    motionSystem = new MotionSystem();
    cameraSystem = new CameraSystem(shaders, window);
    renderSystem = new RenderSystem(shaders,window);
}


// void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_D && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[0] += 0.01f;
//     }
//     if (key == GLFW_KEY_A && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[0] += -0.01f;
//     }
//     if (key == GLFW_KEY_W && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[1] += 0.01f;
//     }
//     if (key == GLFW_KEY_S && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[1] += -0.01f;
//         std::cout<< glm::value_ptr(quad_position)[1]<<"\n";
//     }if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[2] += 0.01f;
//     }if (key == GLFW_KEY_E && action == GLFW_REPEAT)
//     {
//         glm::value_ptr(quad_position)[2] += -0.01f;
//     }
// }
