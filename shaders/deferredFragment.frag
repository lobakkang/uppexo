#version 450

layout (binding = 0) uniform sampler2D samplerposition;
layout (binding = 1) uniform sampler2D samplerNormal;
layout (binding = 2) uniform sampler2D samplerAlbedo;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragcolor;

struct Light {
	vec3 pos;
	float radius;
	vec4 color;
};

layout (binding = 3) uniform UBO {
	Light light;
} ubo;

void main() 
{
	// Get G-Buffer values
	vec3 fragPos = texture(samplerposition, inUV).rgb;
	vec3 normal = texture(samplerNormal, inUV).rgb;
	vec4 albedo = texture(samplerAlbedo, inUV);

  vec3 lightPos = ubo.light.pos;
  vec4 lightColour = ubo.light.color;
  float lightRadius = ubo.light.radius;
	
	#define ambient 0.0
	
	// Ambient part
	vec3 fragcolor  = albedo.rgb * ambient;
	
		// Vector to light
		vec3 L = ubo.light.pos - fragPos;
		float dist = length(L);

		//vec3 V = ubo.viewPos - fragPos;
		vec3 V = vec3(3.0, 3.0, 3.0) - fragPos;
		V = normalize(V);
		
		//if(dist < ubo.light.radius) {
		if(true) {
			// Light to fragment
			L = normalize(L);

			// Attenuation
			float atten = ubo.light.radius / (pow(dist, 2.0) + 1.0);
			//float atten = ubo.light.radius / lightRadius;

			// Diffuse part
			vec3 N = normalize(normal);
			float NdotL = max(0.0, dot(N, L));
			vec3 diff = lightColour.rgb * albedo.rgb * NdotL * atten;

			// Specular part
			vec3 R = reflect(-L, N);
			float NdotR = max(0.0, dot(R, V));
			vec3 spec = lightColour.rgb * albedo.a * pow(NdotR, 16.0) * atten;

			fragcolor += diff + spec;	
		}	

  /*vec3 lightDir = normalize(fragPos - lightPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = lightColour.rgb * (diff * albedo.rgb);
  fragcolor += diffuse;*/
   
  outFragcolor = vec4(fragcolor, 1.0);	
  //outFragcolor = vec4(fragPos, 1.0);
}
