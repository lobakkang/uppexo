#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) flat in uint fragMaterial;

layout(location = 0) out vec4 outColor;

struct Material_T {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 shininess;
};

layout(binding = 1) uniform MaterialObject {
  Material_T material[3];
};

void main() {
  vec3 lightPos = vec3(0.0f, 0.0f, -10.0f);
  vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
  vec3 viewPos = vec3(3.0f, 3.0f, 3.0f);

  vec3 ambient = material[fragMaterial].ambient * lightColour;

  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - lightPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = lightColour * (diff * material[fragMaterial].diffuse);
    
  vec3 viewDir = normalize(fragPos - viewPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material[fragMaterial].shininess.x);
  vec3 specular = lightColour * (spec * material[fragMaterial].specular);
  
  vec3 result = ambient * 0.0f + diffuse * 1.0f + specular * 1.0f;
  outColor = vec4(result, 1.0f);
}
