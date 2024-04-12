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


    Eigen::Vector<float,3> pos;
    Eigen::Vector<float,3> vel;
    Eigen::Matrix<float,3,3> rotation;
    Eigen::Vector<float,3> angular_vel;

    Eigen::Matrix<float,3,3> inverse_inertia;
    private:
    TransformComponent* transform;
    
    Eigen::Vector<float, 3> rotation_to_eulers(Eigen::Matrix<float, 3, 3>);
    Eigen::Matrix<float, 3, 3> create_inertia_tensor(float a, float b, float c);
    Eigen::Matrix<float, 3, 3> create_cross_matrix(Eigen::Vector<float, 3>);
};




