#pragma once
#include "../config.h"

struct TransformComponent{
    glm::vec3 position;
    glm::quat quaternion;
    glm::vec3 eulers;
};

