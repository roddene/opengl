
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
       vertices.push_back(radius*glm::cos(th));
       vertices.push_back(y);
       vertices.push_back(radius*glm::sin(th));

            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

			//normals
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);



				// add indices if not bottom row
				if (row)
				{
					int nextCell = (cell + 1) % res;
					indices.push_back((row - 1) * res + nextCell); // bottom right
					indices.push_back(noVertices - res); // bottom left
					indices.push_back(row * res + nextCell); // top right


					indices.push_back(noVertices - res); // bottom left
					indices.push_back(noVertices); // top left (this vertex)
					indices.push_back(row * res + nextCell); // top right
				}

				noVertices++;
			}
		}

	return {vertices,indices};
}
