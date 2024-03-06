#include "config.h"
#include <filesystem>
#include "controller/app.h"
#include "components/camera_component.h"
#include "components/transform_component.h"
#include "components/physics_component.h"
#include "components/render_component.h"
#include "factories/factory.h"
#include "factories/sphere.h"
#include "factories/octahedron.h"
#include "view/shader.h"


int main()
{
    App* app = new App();
    //std::cout <<app->shaders[0]->ID;

    app->set_up_opengl();
    Factory* factory = new Factory(
        app->transformComponents,app->renderComponents,
        app->physicsComponents,
        app->shaders
        );

    // for (int j =0;j<5;j++){
    // for(int i = 0;i<5;i++){
    //     std::cout << i/4.0f <<"\n";
    // factory->make_cube(
	// 	{3.0f, i/1.5f, j/1.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

    // }}
    //factory->make_sphere({2,0,0},{0,0,0},{0,0,10.0f});
    //factory->make_octahedron({4,0,0},{0,0,0},{0,0,10.0f});
    factory->make_sphere_to_octahedron({0,0,0},{0,0,0},{0,0,10.0f});
    factory->make_cube({3.0,1.0,-1.0},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f});
    std::cout <<"make cam"; 
    unsigned int cameraEntity = factory->make_camera({0.0f,0.0f,-2.0f},{0.0f,180.0f,230.0f});

    CameraComponent* camera = new CameraComponent();
    app->cameraComponent = camera;
    app->cameraID = cameraEntity;


    app->make_systems();

    app->run();
    std::cout<< "finish";
    delete factory;
    delete app;
   return 0;
}


