// Renderer3D Lighting Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout (location = 2) in vec3 a_Normal;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout (location = 0) out vec3 v_FragPos;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_Color;
layout (location = 3) out flat int v_EntityID;

void main()
{
    v_FragPos = a_Position;
    v_Normal = a_Normal;
    v_Color = a_Color;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout (location = 0) in vec3 v_FragPos;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_Color;
layout (location = 3) in flat int v_EntityID;

layout(std140, binding = 0) uniform Light
{
	vec3 u_LightPos; 
    vec3 u_LightColor;
};

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
            
    vec3 result = (ambient + diffuse) * v_Color;

    o_Color = vec4(result, 1.0);
    o_EntityID = v_EntityID;
} 

