// Renderer3D Cube Shader
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat float v_TexIndex;
layout (location = 4) in flat int v_EntityID;
layout (location = 5) in flat int v_Normal;
layout (location = 6) in flat vec3 v_Pos;

layout(std140, binding = 0) uniform Light
{
	vec3 u_LightPos;
	vec3 u_LightColor;
	vec3 u_CamPos;
}

void main()
{
	vec4 texColor = Input.Color;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(v_Normal);
	vec3 lightDirection = normalize(u_LightPos - v_Pos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(u_CamPos - v_Pos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	color = texColor;

	color2 = v_EntityID;
}