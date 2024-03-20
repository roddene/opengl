#include "softbody.h"

SoftBody::SoftBody(TransformComponent* transform)


{
    
    this->transform = transform;
    model = glm::mat4(1.0f);
    
    model = glm::translate(model, transform->position);
    model = glm::rotate(model, glm::radians(transform->eulers.z), {0.0f, 0.0f, 1.0f});
    model = glm::rotate(model, glm::radians(transform->eulers.y), {0.0f, 1.0f, 0.0f});
    model = glm::rotate(model, glm::radians(transform->eulers.x), {1.0f, 0.0f, 0.0f});

    inverse_model = glm::inverse(model);
    glm::vec4 raw_acc = {0,0,-.25,0};

    glm::vec4 modified_acc = inverse_model*raw_acc;
    make_constraints();
    mass.setZero();

    int count = 0;
    for (int x = -1; x < 2; x += 2)
    {
        for (int y = -1; y < 2; y += 2)
        {
            for (int z = -1; z < 2; z += 2)
            {
                pos.row(count) << size * x, size * y, size * z;
                vel.row(count) << 0, 0, 0;
                acc.row(count) << modified_acc.x, modified_acc.y, modified_acc.z;
                mass(count, count) = 1;
            

                PointMass p = {{size * x, size * y, size * z}, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -5.0f}};
                points.push_back(p);
                std::cout << x << "," << y << "," << z << "\n";
                count++;
            }
        }
    }

    inverse_mass = mass.inverse();
}


float SoftBody::compute_volume(){
    float volume = 0.0f;
    for(int i = 0;i<12;i++){
        Eigen::Vector<float,3> p0  {vertices[27*i],vertices[27*i+1],vertices[27*i+2]};
        Eigen::Vector<float,3> p1  {vertices[27*i+9],vertices[27*i+10],vertices[27*i+11]};
        Eigen::Vector<float,3> p2 {vertices[27*i+18],vertices[27*i+19],vertices[27*i+20]};
        
        //std::cout<< //(((p0.cross(p1).transpose()).dot(p2))/6.0f);
        //std::cout << vertices[27*i+20]<<"\n";
        float pipette =p0.cross(p1).transpose()*p2;
        volume+= pipette/6.0f;

    }
    return volume;
}


float SoftBody::volume_constraint(float volume,float inital_volume,float lambda,float stiffness,float dt){
    
    Eigen::Matrix<float,8,3> Jj;
    Jj.setZero();
    float Cj = volume - inital_volume;
    if(Cj != 0.0f){
        for (int i = 0;i<12;i++){
            Eigen::Vector<float,3> p0 = {vertices[27*i],vertices[27*i+1],vertices[27*i+2]};
            Eigen::Vector<float,3> p1 = {vertices[27*i+9],vertices[27*i+10],vertices[27*i+11]};
            Eigen::Vector<float,3> p2 = {vertices[27*i+19],vertices[27*i+19],vertices[27*i+20]};

            
            Jj.row(indices[3*i])+=p1.cross(p2)/6.0f;
            Jj.row(indices[3*i+1])+=p2.cross(p0)/6.0f;
            Jj.row(indices[3*i+2])+=p0.cross(p1)/6.0f;
        }
        float denom = stiffness/(dt*dt);

        for (int i = 0; i<pos.rows();i++){
            denom+=inverse_mass(i,i)*Jj.row(i).dot(Jj.row(i));

        }

        if (denom !=0){
            float delta_lambda = (-Cj-stiffness/(dt*dt)*lambda)/denom;

            for(int i = 0;i<pos.rows();i++){
                pos.row(i) += inverse_mass(i,i)*delta_lambda*Jj.row(i);
            }

            return lambda+delta_lambda;
        }


    }
    return lambda;
}



float SoftBody::spring_constraint(Eigen::Matrix<float,1,3> p1,Eigen::Matrix<float,1,3> p2,float L){
    float magnitude =glm::sqrt((p1-p2)*(p1-p2).transpose()); 
    return magnitude- L;
}

Eigen::Matrix<float,1,3> SoftBody::spring_constraint_derivative(Eigen::Matrix<float,1,3> p1,Eigen::Matrix<float,1,3> p2){
    return (p1-p2)/glm::sqrt((p1-p2)*(p1-p2).transpose());
}

void SoftBody::update(float dt)
{
    float stiffness = .01;
    old_pos = pos;
    for (int i = 0; i < points.size(); i++)
    {

        Eigen::Vector3<float> v_local = vel.row(i) + dt * acc.row(i);

        pos.row(i) += dt * v_local;
    }
    lambda.setZero();

    for (int iters = 0; iters < ITERATION_COUNT; iters++)
    {
        
        for (int j = 0; j < constraints.rows(); j++)
        {
            //std::cout << j<<"\n";
            int i1 = constraints(j,0);
            int i2 = constraints(j,1);
            float constraint = spring_constraint(pos.row(i1),pos.row(i2),constraints(j,2));
            //std::cout << j << " "<< constraint << "\n";
            float delta_lambda = (-constraint-stiffness/(dt*dt)*lambda(j))/(inverse_mass(i1,i1)+inverse_mass(i2,i2)+stiffness/(dt*dt));
            Eigen::Matrix<float,3,1> savedRow1 = pos.row(i1);
            pos.row(i1)+=inverse_mass(i1,i1)*delta_lambda*spring_constraint_derivative(pos.row(i1),pos.row(i2));
            pos.row(i2)+=inverse_mass(i2,i2)*delta_lambda*spring_constraint_derivative(pos.row(i2),savedRow1);
            lambda(j)+=delta_lambda;

            float curVolume = compute_volume();
            //std::cout <<curVolume <<"\n";
            //lambda(j) = volume_constraint(curVolume,8.0f,lambda(j),stiffness,dt);
        }
    }



    for (int i = 0; i < points.size(); i++)
    {
        glm::vec4 model_space_pos = {pos(i,0),pos(i,1),pos(i,2),1.0f};
        glm::vec4 wordspace_pos = model*model_space_pos;
    if (wordspace_pos.z < 0.0f )
        {


            wordspace_pos.z = 0.0f;
            glm::vec4 model_val = inverse_model*wordspace_pos ;
            pos.row(i) << model_val.x,model_val.y,model_val.z;
            
        }
        vel.row(i) = (pos.row(i)-old_pos.row(i))/dt; 
    }
}

// 0:1,2,4;
// 1:

void SoftBody::make_constraints()
{
    constraints.row(0) << 6, 4,2*size;
    constraints.row(1) << 4, 0,2*size;
    constraints.row(2) << 0, 2,2*size;
    constraints.row(3) << 2, 6,2*size;

    constraints.row(4) << 1, 5,2*size;
    constraints.row(5) << 5, 7,2*size;
    constraints.row(6) << 7, 3,2*size;
    constraints.row(7) << 3, 1,2*size;

    constraints.row(8) << 3, 2,2*size;
    constraints.row(9) << 0, 1,2*size;

    constraints.row(10) << 6, 7,2*size;
    constraints.row(11) << 5, 4,2*size;


    //Diagonal 
    constraints.row(12) << 0, 6,2*glm::sqrt(2*size);
    constraints.row(13) << 1, 7,2*glm::sqrt(2*size);
    constraints.row(14) << 0, 3,2*glm::sqrt(2*size);
    constraints.row(15) << 4, 7,2*glm::sqrt(2*size);
    constraints.row(16) << 5, 0,2*glm::sqrt(2*size);
    constraints.row(17) << 7, 2,2*glm::sqrt(2*size);
    


}

std::vector<float> SoftBody::make_vertices(float dt)

{
    indices = {6,4,0,0,2,6, 
    1,5,7,7,3,1,
    3,2,0,0,1,3,
    4,6,7,7,5,4,
    0,4,5,5,1,0,
    7,6,2,2,3,7
    };

     vertices = {
        pos(6, 0), pos(6, 1), pos(6, 2), 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        pos(4, 0), pos(4, 1), pos(4, 2), 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        pos(2, 0), pos(2, 1), pos(2, 2), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        pos(6, 0), pos(6, 1), pos(6, 2), 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

        pos(1, 0), pos(1, 1), pos(1, 2), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        pos(5, 0), pos(5, 1), pos(5, 2), 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        pos(3, 0), pos(3, 1), pos(3, 2), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        pos(1, 0), pos(1, 1), pos(1, 2), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        pos(3, 0), pos(3, 1), pos(3, 2), 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        pos(2, 0), pos(2, 1), pos(2, 2), 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        pos(1, 0), pos(1, 1), pos(1, 2), 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        pos(3, 0), pos(3, 1), pos(3, 2), 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

        pos(4, 0), pos(4, 1), pos(4, 2), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        pos(6, 0), pos(6, 1), pos(6, 2), 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        pos(5, 0), pos(5, 1), pos(5, 2), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        pos(4, 0), pos(4, 1), pos(4, 2), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        pos(4, 0), pos(4, 1), pos(4, 2), 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        pos(5, 0), pos(5, 1), pos(5, 2), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        pos(5, 0), pos(5, 1), pos(5, 2), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        pos(1, 0), pos(1, 1), pos(1, 2), 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        pos(0, 0), pos(0, 1), pos(0, 2), 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        pos(6, 0), pos(6, 1), pos(6, 2), 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        pos(2, 0), pos(2, 1), pos(2, 2), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        pos(2, 0), pos(2, 1), pos(2, 2), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        pos(3, 0), pos(3, 1), pos(3, 2), 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        pos(7, 0), pos(7, 1), pos(7, 2), 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

        this->update(dt);
    return vertices;
}
