#version 330 core
in vec3 posInView;
in vec3 normalInView;
in vec2 texcoord;

out vec4 FragColor;

// 平行光源信息
struct DirectLight{
    vec3 direction;
    float ambient;
    vec3 diffuse;
    float specular;
};
uniform DirectLight directLight;

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

// 函数声明
vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal);
vec3 calcDirecLight(DirectLight light, vec3 pos, vec3 normal);

void main()
{
    vec3 plColor = calcPointLight(pointLight, posInView, normalInView);
    vec3 dlColor = calcDirecLight(directLight, posInView, normalInView);

    //FragColor = texture(material.diffuse, texcoord);
    FragColor = vec4((plColor + dlColor), 1.0f);
}

vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal){
    float attConstant = 1.0f;
    float attLinear = 0.09f;
    float attQuadratic = 0.032f;

    // calculate attenuation(衰减)
    float distance = length(light.posInView - pos);
    float attenuation = 1.0f / (attConstant + attLinear * distance + attQuadratic * (distance * distance));
    
    // calculate ambient
    vec3 ambientColor = light.ambient * texture(material.diffuse, texcoord).rgb * attenuation;
    
    
    // calculate diffuse
    vec3 normalizeN = normalize(normal);
    vec3 lightDir = normalize(light.posInView - pos);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * light.diffuse * texture(material.diffuse, texcoord).rgb * attenuation;
    
    
    // calculate specular
    vec3 specularColor = vec3(0.0f, 0.0f, 0.0f);
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    specularColor = specularFactor * light.specular * texture(material.specular, texcoord).rgb * attenuation;

    return ambientColor + diffuseColor + specularColor;
}

vec3 calcDirecLight(DirectLight light, vec3 pos, vec3 normal){
    // calculate ambient
    vec3 ambientColor = light.ambient * texture(material.diffuse, texcoord).rgb;
    
    // calculate diffuse
    vec3 normalizeN = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * light.diffuse * texture(material.diffuse, texcoord).rgb;
    
    // calculate specular
    vec3 specularColor = vec3(0.0f, 0.0f, 0.0f);
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    specularColor = specularFactor * light.specular * texture(material.specular, texcoord).rgb;

    return ambientColor + diffuseColor + specularColor;
}