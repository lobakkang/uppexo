#version 450
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in uint inMaterial;

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
  mat4 norm_model;
} ubo;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) flat out uint fragMaterial;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    fragPos = vec3(ubo.model * vec4(inPosition, 1.0f));
    //fragNormal = vec3(vec4(inNormal, 1.0f));
    fragNormal = vec3(ubo.model * vec4(inNormal, 1.0f));
    fragMaterial = inMaterial;
}
