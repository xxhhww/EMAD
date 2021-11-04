#version 330 core
in vec3 posInView;
in vec4 posInDLSpace;
in vec3 normalInView;
in vec2 texcoord;

out vec4 FragColor;

// ƽ�й�Դ��Ϣ
struct DirectLight{
    vec3 direction;
    float ambient;
    vec3 diffuse;
    float specular;
};
uniform DirectLight directLight;

// ���Դ��Ϣ
struct PointLight{
    vec3 posInView;
    float ambient;
    vec3 diffuse;
    float specular;
};
uniform PointLight pointLight;

// ���������Ϣ
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform sampler2D dlShadowMap;

// ��������
float calcShadow(vec3 normal, vec3 lightDir);
vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal);
vec3 calcDirecLight(DirectLight light, vec3 pos, vec3 normal);

void main()
{
    //vec3 plColor = calcPointLight(pointLight, posInView, normalInView);
    vec3 dlColor = calcDirecLight(directLight, posInView, normalInView);

    //FragColor = texture(material.diffuse, texcoord);
    FragColor = vec4(dlColor, 1.0f);
}

float calcShadow(vec3 normal, vec3 lightDir){
    // ִ��͸�ӳ���
    vec3 projCoords = posInDLSpace.xyz / posInDLSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // ����
    float closestDepth = texture(dlShadowMap, projCoords.xy).r;
    // ��ǰ���ص��ڶ�����ͶӰ�ռ��µ����
    float currentDepth = projCoords.z;
    // ƫ����
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(dlShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(dlShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    // ������������׶��Զƽ��
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal){
    float attConstant = 1.0f;
    float attLinear = 0.09f;
    float attQuadratic = 0.032f;

    // calculate attenuation(˥��)
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

    return ambientColor +  (1 - calcShadow(normal, lightDir))* (diffuseColor + specularColor);
}