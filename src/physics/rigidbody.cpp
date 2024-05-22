#include "rigidbody.h"


RigidBody::RigidBody(TransformComponent* transform){

rotation = glm::mat3(1.0f);
this->transform = transform;
pos = {0,0,0};
vel = {0,0,0};
angular_vel = {.0002,0,.1};

inverse_inertia = create_inertia_tensor(.1f,1.0f,2.5f);
inverse_inertia[0][0] = 1/inverse_inertia[0][0];
inverse_inertia[1][1] = 1/inverse_inertia[1][1];
inverse_inertia[2][2] = 1/inverse_inertia[2][2];


}



void RigidBody::update(float dt){
    rotation = glm::toMat4(glm::normalize(glm::quat_cast(rotation)));
    glm::mat3 R = rotation;

    glm::vec3 omega = R*inverse_inertia*glm::transpose(R)*angular_vel;


   rotation+=dt*create_cross_matrix(omega)*R;


    transform->quaternion = glm::quat_cast(rotation);
    std::cout<< "status:\n";
    std::cout <<glm::to_string(create_cross_matrix(omega))<<"\n";
    std::cout << glm::to_string(omega) <<"\n";
    std::cout <<glm::to_string(360.0f*glm::eulerAngles(transform->quaternion)/TWO_PI) << "\n";
    std::cout <<glm::determinant(rotation)<<"\n";
    //std::cout <<omega[0]<<","<<omega[1]<<","<<omega[2] <<"\n";
   // std::cout << eulers[0] << ","<< eulers[1] <<"," <<eulers[2] <<"\n";
}



//could cache for better performance
glm::mat3 RigidBody::create_cross_matrix(glm::vec3 vector){
 glm::mat3 matrix  {
    {0.0f, -vector[2],vector[1]},
    {vector[2],0.0f,-vector[0]},
    {-vector[1],vector[0],0.0f}
 };
return matrix;

} 




glm::mat3 RigidBody::create_inertia_tensor(float a,float b, float c){
    float volume = a*b*c;
    glm::mat3 inertia = glm::mat3(1.0f);
    inertia[0][0] = b*b + c*c;
    inertia[1][1] = a*a + c*c;
    inertia[2][2] = b*b + a*a;
    return volume*inertia/12.0f;


    
}