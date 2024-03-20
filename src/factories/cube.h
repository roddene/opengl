#pragma once
#include "../config.h"

#include "../components/render_component.h"
#include "../components/mesh_vector_component.h"

MeshVectorComponent make_cube_mesh(glm::vec3 size);
MeshVectorComponent make_cube_mesh_soft(glm::vec3 size);