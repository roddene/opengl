#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

out vec3 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection* view*model*vec4(vertexPos, 1.0);
    fragmentColor = vertexColor;
    fragmentPos = vec3(model*vec4(vertexPos,1.0));
    fragmentNormal = vertexPos;
}
