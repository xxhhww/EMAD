#version 330 core
in vec3 posInView;
in vec3 normalInView;
in vec2 texcoord;

out vec4 FragColor;

// 点光源信息
struct PointLight{
    vec3 posInView;
    float ambient;
    vec3 diffuse;
    float specular;
};

uniform PointLight pointLight;

// 物体材质信息
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main()
{   
    float attConstant = 1.0f;
    float attLinear = 0.09f;
    float attQuadratic = 0.032f;

    // calculate attenuation(衰减)
    float distance = length(pointLight.posInView - posInView);
    float attenuation = 1.0f / (attConstant + attLinear * distance + attQuadratic * (distance * distance));
    
    // calculate ambient
    vec3 ambientColor = pointLight.ambient * texture(material.diffuse, texcoord).rgb;
    
    
    // calculate diffuse
    vec3 normalizeN = normalize(normalInView);
    vec3 lightDir = normalize(pointLight.posInView - posInView);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * pointLight.diffuse * texture(material.diffuse, texcoord).rgb * attenuation;
    
    
    // calculate specular
    vec3 viewDir = normalize(-posInView);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specularColor = specularFactor * pointLight.specular * texture(material.specular, texcoord).rgb * attenuation;

    FragColor = vec4((ambientColor + diffuseColor + specularColor), 1.0f);
}