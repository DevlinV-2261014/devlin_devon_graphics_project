#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture1;
uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = texture(texture1, TexCoord) * vec4((ambient + diffuse) * objectColor, 1.0);
}