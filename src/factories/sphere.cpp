
#include "sphere.h";

MeshVectorComponent make_sphere_mesh(unsigned int res)
{

    // int res = 5;

    std::vector<float> vertices = {

     };
    std::vector<unsigned int> indices = {};

		float circleStep = glm::two_pi<float>() / (float)res; // angle step between cells
		float heightStep = glm::pi<float>() / (float)res; // height of row

		int row = 0;
		int noVertices = 0;
		float phi = -glm::half_pi<float>();
		float y = glm::sin(phi);
		float radius;

		for (; phi < HALF_PI + heightStep; phi += heightStep, row++) {
        
            
			 y = glm::sin(phi);
			 radius = glm::cos(phi);
			int cell = 0;
            
            //y = glm::sign(glm::sin(phi))* glm::acos(glm::cos(2*phi))/PI;
            //radius = glm::sign(glm::cos(phi))*glm::acos(glm::cos(2*phi+PI))/PI;

			for (float th = 0; th < TWO_PI; th += circleStep, cell++) {
        
            
            
			//vertices.push_back(radius*glm::sign(glm::cos(th))*glm::acos(glm::cos(2*th+PI))/PI);
    //        vertices.push_back(y);
      //     vertices.push_back(radius*glm::sign(glm::sin(th))*glm::acos(glm::cos(th*2))/PI);
		float vertexA = radius*glm::cos(th);
	   	float vertexB = y;
	   	float vertexC = radius*glm::sin(th);
	   vertices.push_back(vertexA);
       vertices.push_back(vertexB);
       vertices.push_back(vertexC);

			glm::vec3 normed = glm::normalize(glm::vec3 {vertexA,vertexB,vertexC});	
            // vertices.push_back((phi+HALF_PI)/PI);
            vertices.push_back(0.0);
            vertices.push_back(1.0);
            vertices.push_back(0.0);
            // vertices.push_back(th/TWO_PI);

			//normals
			
		

				// add indices if not bottom row
				if (row)
				{
					
					int nextCell = (cell + 1) % res;

					int bottomLeft = noVertices-res;
					int bottomRight = (row - 1) * res + nextCell;
					int topRight = row*res+nextCell;
					int topLeft = noVertices;

					indices.push_back(bottomRight); // bottom right
					indices.push_back(bottomLeft); // bottom left
					indices.push_back(topRight); // top right

					glm::vec3 vectorA = {-vertices[9*topLeft] + vertices[9*topRight],
					-vertices[9*topLeft+1]+vertices[9*topRight+1],
					-vertices[9*topLeft+2]+vertices[9*topRight+2]}; 
					float s = glm::sign(phi);
					vectorA = vectorA * glm::vec3 {s,s,s};
					
					

					glm::vec3 vectorB = {vertices[9*topRight] - vertices[9*bottomRight],
					vertices[9*topRight+1]-vertices[9*bottomRight+1],
					vertices[9*topRight+2]-vertices[9*bottomRight+2]}; 


					glm::vec3 normal = glm::normalize(glm::cross(vectorB,vectorA));

					
					// vertices.push_back(normal[0]);
					// vertices.push_back(normal[1]);
					// vertices.push_back(normal[2]);
					
					vertices.push_back(vertexA);
					vertices.push_back(vertexB);
					vertices.push_back(vertexC);


					//std::cout <<"norm:"<< normal[0]<<","<<normal[1]<<"," <<normal[2] <<"\n";
					//std::cout << vertexA<<","<<vertexB<<"," <<vertexC <<"\n";


					indices.push_back(bottomLeft); // bottom left
					indices.push_back(topLeft); // top left (this vertex)
					indices.push_back(topRight); // top right
				}else{
					vertices.push_back(0.0f);
					vertices.push_back(-1.0f);
					vertices.push_back(0.0f);
				}

				noVertices++;
			}
		}

	return {vertices,indices};
}


MeshVectorComponent make_sphere_mesh_test(unsigned int res)
{

    // int res = 5;

    std::vector<float> vertices = {

     };
    std::vector<unsigned int> indices = {};

		float circleStep = glm::two_pi<float>() / (float)res; // angle step between cells
		float heightStep = glm::pi<float>() / (float)res; // height of row

		int row = 0;
		int noVertices = 0;
		float phi = -glm::half_pi<float>();
		float y = glm::sin(phi);
		float radius;

		for (; phi < HALF_PI + heightStep; phi += heightStep, row++) {
        
            
			 y = glm::sin(phi);
			 radius = glm::cos(phi);
			int cell = 0;
            
            //y = glm::sign(glm::sin(phi))* glm::acos(glm::cos(2*phi))/PI;
            //radius = glm::sign(glm::cos(phi))*glm::acos(glm::cos(2*phi+PI))/PI;

			for (float th = 0; th < TWO_PI; th += circleStep, cell++) {
        
            
            
			//vertices.push_back(radius*glm::sign(glm::cos(th))*glm::acos(glm::cos(2*th+PI))/PI);
    //        vertices.push_back(y);
      //     vertices.push_back(radius*glm::sign(glm::sin(th))*glm::acos(glm::cos(th*2))/PI);
		float vertexA = radius*glm::cos(th);
	   	float vertexB = y;
	   	float vertexC = radius*glm::sin(th);
	   vertices.push_back(vertexA);
       vertices.push_back(vertexB);
       vertices.push_back(vertexC);

			glm::vec3 normed = glm::normalize(glm::vec3 {vertexA,vertexB,vertexC});	
            // vertices.push_back((phi+HALF_PI)/PI);
            // vertices.push_back(vertexA);
            // vertices.push_back(vertexB);
            // vertices.push_back(vertexC);
            // vertices.push_back(th/TWO_PI);

			//normals
			
		

				// add indices if not bottom row
				if (row)
				{
					
					int nextCell = (cell + 1) % res;

					int bottomLeft = noVertices-res;
					int bottomRight = (row - 1) * res + nextCell;
					int topRight = row*res+nextCell;
					int topLeft = noVertices;

					indices.push_back(bottomRight); // bottom right
					indices.push_back(bottomLeft); // bottom left
					indices.push_back(topRight); // top right

					glm::vec3 vectorA = {-vertices[9*topLeft] + vertices[9*bottomLeft],
					-vertices[9*topLeft+1]+vertices[9*bottomLeft+1],
					-vertices[9*topLeft+2]+vertices[9*bottomLeft+2]}; 
					
					

					glm::vec3 vectorB = {-vertices[9*bottomRight] + vertices[9*bottomLeft],
					-vertices[9*bottomRight+1]+vertices[9*bottomLeft+1],
					-vertices[9*bottomRight+2]+vertices[9*bottomLeft+2]}; 
					

					glm::vec3 normal = glm::normalize(glm::cross(vectorA,vectorB));
				if (phi>0&&phi<.05 ){
					std::cout <<topLeft<<"," << bottomLeft<<","<<topRight<<","<<bottomRight<<","<<vertices[9*topRight]<<"\n";
					std::cout << topRight << ": " << vertices[9*topRight] << ", " <<vertices[9*85]<<"\n";	
					std::cout<< std::setprecision(2)<<" topLeft: " <<vertices[9*topLeft] << "," <<vertices[9*topLeft+1] <<"," <<vertices[9*topLeft+2]
					<< " bottomLeft: " <<vertices[9*bottomLeft]<< "," <<vertices[9*bottomLeft+1] <<"," <<vertices[9*bottomLeft+2]
					<< " topRight: " <<vertices[9*topRight] << "," <<vertices[9*topRight+1] <<"," <<vertices[9*topRight+2]
					<< " bottomRight: " <<vertices[9*bottomRight] << "," <<vertices[9*bottomRight+1] <<"," <<vertices[9*bottomRight+2]
					<<"\n";
					std::cout <<"norm:"<< normal[0]<<","<<normal[1]<<"," <<normal[2] <<"\n";
					std::cout << vertexA<<","<<vertexB<<"," <<vertexC <<"\n";

					
					}
					
					vertices.push_back(normal[0]);
					vertices.push_back(normal[1]);
					vertices.push_back(normal[2]);
					
					vertices.push_back(vertexA);
					vertices.push_back(vertexB);
					vertices.push_back(vertexC);


					indices.push_back(bottomLeft); // bottom left
					indices.push_back(topLeft); // top left (this vertex)
					indices.push_back(topRight); // top right
				}else{
					vertices.push_back(0.0f);
					vertices.push_back(-1.0f);
					vertices.push_back(0.0f);
					vertices.push_back(0.0f);
					vertices.push_back(0.0f);
					vertices.push_back(0.0f);
				}

				noVertices++;
			}
		}
	for (int i = 0;i<100/9;i+=1){
		for (int j = 0;j<3;j++){
		std::cout<<vertices[i*9+j]<<",";
		}
		std::cout <<"\n";
	}
	return {vertices,indices};
}
