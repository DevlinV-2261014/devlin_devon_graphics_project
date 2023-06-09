#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPosition;

struct PointLight {
    vec3 lightPosition;
    
    float constant;
    float linear;
    float quadratic;
};

// Must be the same as getLightPositions().size() in Main.cpp
#define POINT_LIGHTS 4
uniform PointLight lights[POINT_LIGHTS];

vec3 calculateLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float ambientStrength, float specularStrength, vec3 oColor, vec3 lColor);

void main()
{
    // Declare variables
    float ambientStrength = 0.22;
    float specularStrength = 1.0;
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPosition);

    // Calculate the first one so result is not empty
    vec3 result = calculateLight(lights[0], norm, FragPosition, viewDirection, ambientStrength, specularStrength, objectColor, lightColor);

    // Add every light calculation to the result
    for (int i = 1; i < POINT_LIGHTS; i++) {
        result += calculateLight(lights[i], norm, FragPosition, viewDirection, ambientStrength, specularStrength, objectColor, lightColor);
    }

    // Set color
    FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);
}

vec3 calculateLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float ambientStrength, float specularStrength, vec3 oColor, vec3 lColor){
   // Ambient Light
   vec3 ambient = ambientStrength * lColor;

   // Diffuse Light
    vec3 lightDirection = normalize(light.lightPosition - fragPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lColor;

    // Specualar Light
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lColor;

    // Attenuation
    float distance = length(light.lightPosition - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Put it all together
    return (ambient + diffuse + specular) * oColor;;
}