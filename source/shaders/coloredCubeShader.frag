#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec4 vector;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    vec3 lightDir;
    if(light.vector.w == 0.0f){
        lightDir = normalize((-light.vector).rgb);
    }
    else{
        lightDir = normalize(lightPos - FragPos);
    }

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    float distance = length(light.vector.rgb - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}