#version 330 core
in vec3 posInView;
in vec3 posInWorld;
in vec4 posInDLSpace;
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
uniform vec3 pointLightPos; // 点光源在世界坐标中的位置
uniform float far_plane;

// 物体材质信息
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform sampler2D dlShadowMap;
uniform samplerCube plShadowMap;

// 函数声明
float calcDLShadow(vec3 normal, vec3 lightDir);
float calcPLShadow(vec3 normal, vec3 lightDir);
vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal);
vec3 calcDirecLight(DirectLight light, vec3 pos, vec3 normal);

void main()
{
    vec3 plColor = calcPointLight(pointLight, posInView, normalInView);
    vec3 dlColor = calcDirecLight(directLight, posInView, normalInView);

    //FragColor = texture(material.diffuse, texcoord);
    FragColor = vec4((plColor + dlColor), 1.0f);
}

float calcDLShadow(vec3 normal, vec3 lightDir){
    // 执行透视除法
    vec3 projCoords = posInDLSpace.xyz / posInDLSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // 采样
    float closestDepth = texture(dlShadowMap, projCoords.xy).r;
    // 当前像素点在定向光的投影空间下的深度
    float currentDepth = projCoords.z;
    // 偏移量
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

    // 超出定向光的视锥的远平面
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float calcPLShadow(vec3 normal, vec3 lightDir){
    // Get vector between fragment position and light position
    vec3 fragToLight = posInWorld - pointLightPos;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float shadow = 0.0f;
    float bias = 0.05f; 
    float samples = 4.0f;
    float offset = 0.1f;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5f))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5f))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5f))
            {
                float closestDepth = texture(plShadowMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= far_plane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0f;
            }
        }
    }
    shadow /= (samples * samples * samples);
    return shadow;
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

    return ambientColor + (diffuseColor + specularColor);
    // calcPLShadow的偏移量有些问题
    //return ambientColor + (1 - calcPLShadow(normal, lightDir)) * (diffuseColor + specularColor);
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

    return ambientColor +  (1 - calcDLShadow(normal, lightDir))* (diffuseColor + specularColor);
}