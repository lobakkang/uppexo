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

layout(binding = 1, std430) buffer MaterialObject {
  Material_T material[10];
};

void main() {
  vec3 lightPos = vec3(0.0f, -8.0f, 0.0f);
  vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);

  vec3 ambient = material[fragMaterial].ambient * lightColour;

  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - lightPos);
  float diff = max(0.0, dot(norm, lightDir));
  vec3 diffuse = diff * lightColour * material[fragMaterial].diffuse;
  
  //vec3 result = ambient * 0.1f + diffuse * 0.1f;
  //vec3 result = material[fragMaterial].ambient;
  vec3 result = material[1].ambient;
  outColor = vec4(result, 1.0f);
}
