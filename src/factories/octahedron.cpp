
#include "octahedron.h";

MeshVectorComponent make_octahedron_mesh(unsigned int res)
{

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
        
            
			//  y = glm::sin(phi);
			//  radius = glm::cos(phi);
			int cell = 0;
            
            y = glm::sign(glm::sin(phi))* glm::acos(glm::cos(2*phi))/PI;
            radius = glm::sign(glm::cos(phi))*glm::acos(glm::cos(2*phi+PI))/PI;

			for (float th = 0; th < TWO_PI; th += circleStep, cell++) {
        
            float vertexA = radius*glm::sign(glm::cos(th))*glm::acos(glm::cos(2*th+PI))/PI;
	   	float vertexB = y;
	   	float vertexC = radius*glm::sign(glm::sin(th))*glm::acos(glm::cos(th*2))/PI;
	   vertices.push_back(vertexA);
       vertices.push_back(vertexB);
       vertices.push_back(vertexC);


            
			// vertices.push_back(radius*glm::sign(glm::cos(th))*glm::acos(glm::cos(2*th+PI))/PI);
        //    vertices.push_back(y);
        //   vertices.push_back(radius*glm::sign(glm::sin(th))*glm::acos(glm::cos(th*2))/PI);
    //    vertices.push_back(radius*glm::cos(th));
    //    vertices.push_back(y);
    //    vertices.push_back(radius*glm::sin(th));

            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

			//normals
			// vertices.push_back(0.0f);
			// vertices.push_back(0.0f);
			// vertices.push_back(0.0f);


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
					


					glm::vec3 normal = (glm::cross(vectorA,vectorB));
					
					vertices.push_back(normal[0]);
					vertices.push_back(normal[1]);
					vertices.push_back(normal[2]);
					
					// vertices.push_back(vertexA);
					// vertices.push_back(vertexB);
					// vertices.push_back(vertexC);


					// std::cout <<"norm:"<< normal[0]<<","<<normal[1]<<"," <<normal[2] <<"\n";
					// std::cout << vertexA<<","<<vertexB<<"," <<vertexC <<"\n";


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
