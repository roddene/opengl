#include "rigidbody.h"


RigidBody::RigidBody(TransformComponent* transform){

rotation.setIdentity();
this->transform = transform;
pos << 0,0,0;
vel << 0,0,0;
angular_vel << .002,.2,0;

inverse_inertia = create_inertia_tensor(.1,1,2.5);
inverse_inertia(0,0) = 1/inverse_inertia(0,0);
inverse_inertia(1,1) = 1/inverse_inertia(1,1);
inverse_inertia(2,2) = 1/inverse_inertia(2,2);


}



void RigidBody::update(float dt){
    Eigen::Matrix<float,3,3> R = rotation;
    Eigen::Vector<float,3> omega = R*inverse_inertia*R.transpose()*angular_vel;

   rotation+=dt*create_cross_matrix(omega)*R;
    Eigen::Vector<float,3> eulers = rotation_to_eulers(rotation); 
    //std::cout <<omega(0)<<","<<omega(1)<<","<<omega(2) <<"\n";
   // std::cout << eulers(0) << ","<< eulers(1) <<"," <<eulers(2) <<"\n";
}


//could cache for better performance
Eigen::Matrix<float,3,3> RigidBody::create_cross_matrix(Eigen::Vector<float,3> vector){
 Eigen::Matrix<float,3,3> matrix  {
    {0.0f, -vector(2),vector(1)},
    {vector(2),0.0f,-vector(0)},
    {-vector(1),vector(0),0.0f}
 };
return matrix;

} 



Eigen::Vector<float,3> RigidBody::rotation_to_eulers(Eigen::Matrix<float,3,3> R ){

    float sy = sqrt(R(2,1) * R(2,1) +  R(2,2) * R(2,2) );
 
    bool singular = sy < 1e-6; 
 
    float x, y, z;
    if (!singular)
    {
        x = atan2(R(2,1) , R(2,2));
        y = atan2(-R(2,0), sy);
        z = atan2(R(1,0), R(0,0));
    }
    else
    {
        x = atan2(-R(1,2), R(1,1));
        y = atan2(-R(2,0), sy);
        z = 0;
    }
    Eigen::Vector<float,3> eulers {x,y,z};

    return 360*eulers/(TWO_PI);

}


Eigen::Matrix<float,3,3> RigidBody::create_inertia_tensor(float a,float b, float c){
    float volume = a*b*c;
    Eigen::Matrix<float,3,3> inertia;
    inertia.setZero();
    inertia(0,0) = b*b + c*c;
    inertia(1,1) = a*a + c*c;
    inertia(2,2) = b*b + a*a;
    return volume*inertia/12;


    
}