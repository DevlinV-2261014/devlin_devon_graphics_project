#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture1;
uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform float constant;
uniform float linear;
uniform float quadratic;

void main()
{
    // Ambient Light
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specualar Light
    float specularStrength = 1.0;
    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Attenuation
    float distance = length(lightPosition - FragPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Grab everything and put it all together
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);
}