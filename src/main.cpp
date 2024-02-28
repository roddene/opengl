#include "config.h"
#include <filesystem>
#include "controller/app.h"
#include "components/camera_component.h"
#include "components/transform_component.h"
#include "components/physics_component.h"
#include "components/render_component.h"
#include "factories/factory.h"

int main()
{
    App* app = new App();
    Factory* factory = new Factory(
        app->transformComponents,app->renderComponents,
        app->physicsComponents
        );

    // for (int j =0;j<5;j++){
    // for(int i = 0;i<5;i++){
    //     std::cout << i/4.0f <<"\n";
    // factory->make_cube(
	// 	{3.0f, i/1.5f, j/1.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f});

    // }}
    factory->make_circle({2,0,0},{0,0,0},{0,0,10.0f});
    //factory->make_cube({0,0,0},{0,0,0},{0,0,10.0f});

    unsigned int cameraEntity = factory->make_camera({0.0f,0.0f,-2.0f},{0.0f,180.0f,230.0f});

    CameraComponent* camera = new CameraComponent();
    app->cameraComponent = camera;
    app->cameraID = cameraEntity;


    app->set_up_opengl();
    app->make_systems();

    app->run();
    std::cout<< "finish";
    delete factory;
    delete app;
   return 0;
}


