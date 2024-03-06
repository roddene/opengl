#version 330 core

in vec3 fragmentColor;
in vec3 fragmentPos;
in vec3 fragmentNormal;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 screenColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(lightPos-fragmentPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor;

    vec3 result = (ambient+diffuse) *lightColor;
    screenColor = vec4(result, 1.0);
}