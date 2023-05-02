#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMat;

out vec2 TexCoord;
out vec3 FragPosition;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPosition = vec3(instanceMat * vec4(aPos, 1.0f));
    Normal = aNormal;
    gl_Position = projection * view * instanceMat * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}