#pragma once

#include "../config.h"
#include <Eigen/Dense>
#include "../../lib/eigen-3.4.0/Eigen/Dense"
#include "../components/transform_component.h"

class RigidBody{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    RigidBody(TransformComponent* transform);

    void update(float dt);


    glm::vec3 pos;
    glm::vec3 vel;
    glm::mat3 rotation;
    glm::vec3 angular_vel;

    glm::mat3 inverse_inertia;
    private:
    TransformComponent* transform;
    
    glm::mat3 create_inertia_tensor(float a, float b, float c);
    glm::mat3 create_cross_matrix(glm::vec3);
};




