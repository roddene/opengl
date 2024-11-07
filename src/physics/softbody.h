#pragma once
#include "../config.h"
#include <eigen3/Eigen/Dense>
#include "../components/transform_component.h"
#define ITERATION_COUNT 3
struct PointMass {
glm::vec3 pos;
float mass;
glm::vec3 vel;
glm::vec3 acc;
};


class SoftBody{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    SoftBody(TransformComponent* transform);

    float compute_volume();

    float volume_constraint(float volume, float inital_volume, float lambda,float stiffness,float dt);

    void update(float dt);

    void make_constraints();

    std::vector<float> make_vertices(float dt);



    std::vector<PointMass> points;
    
    
    float size = 1.0f;
    private:
    TransformComponent* transform;
    glm::mat4 model;
    glm::mat4 inverse_model;

    std::vector<float> vertices;
    std::vector<int> indices;
    float spring_constraint(Eigen::Matrix<float, 1, 3> p1, Eigen::Matrix<float, 1, 3> p2,float L);
    Eigen::Matrix<float, 1, 3> spring_constraint_derivative(Eigen::Matrix<float, 1, 3> p1, Eigen::Matrix<float, 1, 3> p2);

    Eigen::Matrix<float, 8, 3> pos;
    Eigen::Matrix<float, 8, 3> old_pos;
    Eigen::Matrix<float,8,3> vel;
    Eigen::Matrix<float,8,3> acc;
    Eigen::Matrix<float,8,8> mass;
    Eigen::Matrix<float,8,8> inverse_mass;

    Eigen::Matrix<float,18,3> constraints;
 
    Eigen::Vector<float,18> lambda;


};