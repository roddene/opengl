#version 330 core

in vec3 fragmentColor;
in vec3 fragmentPos;
in vec3 fragmentNormal;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 screenColor;

void main()
{
    
    float specularStrength = 0.5;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(lightPos-fragmentPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor;

    vec3 viewDir = normalize(viewPos-fragmentPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrength*spec*lightColor;

    vec3 result = (ambient+diffuse+specular) *lightColor*fragmentColor;
    screenColor = vec4(result, 1.0);
}