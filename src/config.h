#include <iostream>
#include <iomanip>
#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#define SCREENWIDTH 640
#define SCREENHEIGHT 480
#include <GLFW/glfw3.h>
#include <fstream>
#include<sstream>
#include <string>
#include <vector>
#include <unordered_map>
#define LIGHT_SRC_SHADER 1
#define LIGHT_DST_SHADER 0

#define PI glm::pi<float>()
#define HALF_PI glm::half_pi<float>()
#define TWO_PI glm::two_pi<float>()

