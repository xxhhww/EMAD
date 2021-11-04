#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec4 PosInDLSpace;
    vec2 TexCoords;
    vec3 TangentFragPos;
    // view
    vec3 TangentViewPos;
    // 光照模型
    vec3 TangentPointLightPos;
    vec3 TangentDirectLightDir;
} fs_in;

// 光信息
struct Light{
    float ambient;
    vec3 diffuse;
    float specular;
};
uniform Light pointLight;
uniform Light directLight;
uniform sampler2D dlShadowMap;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

// 函数声明
float calcShadow(vec3 normal, vec3 lightDir);
vec3 calcPointLight(Light light, vec3 pos, vec3 view, vec3 normal);
vec3 calcDirecLight(Light light, vec3 pos, vec3 view, vec3 normal);

void main()
{
    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // 在切线空间中计算光照
    //vec3 plColor = calcPointLight(pointLight, fs_in.TangentFragPos, fs_in.TangentViewPos, normal);
    vec3 dlColor = calcDirecLight(directLight, fs_in.TangentFragPos, fs_in.TangentViewPos, normal);

    FragColor = vec4(dlColor, 1.0f);
}

float calcShadow(vec3 normal, vec3 lightDir){
    // 执行透视除法
    vec3 projCoords = fs_in.PosInDLSpace.xyz / fs_in.PosInDLSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // 采样
    float closestDepth = texture(dlShadowMap, projCoords.xy).r;
    // 当前像素点在定向光的投影空间下的深度
    float currentDepth = projCoords.z;
    // 偏移量
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
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

vec3 calcPointLight(Light light, vec3 pos, vec3 view, vec3 normal)
{
    float attConstant = 1.0f;
    float attLinear = 0.09f;
    float attQuadratic = 0.032f;

    // calculate attenuation(衰减)
    float distance = length(fs_in.TangentPointLightPos - pos);
    float attenuation = 1.0f / (attConstant + attLinear * distance + attQuadratic * (distance * distance));
    
    // calculate ambient
    vec3 ambientColor = light.ambient * texture(diffuseMap, fs_in.TexCoords).rgb * attenuation;
    
    
    // calculate diffuse
    vec3 normalizeN = normalize(normal);
    vec3 lightDir = normalize(fs_in.TangentPointLightPos - pos);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * light.diffuse * texture(diffuseMap, fs_in.TexCoords).rgb * attenuation;
    
    
    // calculate specular
    vec3 viewDir = normalize(view - pos);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
    vec3 specularColor = specularFactor * light.specular * light.diffuse * attenuation;

    return ambientColor + diffuseColor + specularColor;
}

vec3 calcDirecLight(Light light, vec3 pos, vec3 view, vec3 normal)
{
    // calculate ambient
    vec3 ambientColor = light.ambient * texture(diffuseMap, fs_in.TexCoords).rgb;
    
    // calculate diffuse
    vec3 normalizeN = normalize(normal);
    vec3 lightDir = normalize(-fs_in.TangentDirectLightDir);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * light.diffuse * texture(diffuseMap, fs_in.TexCoords).rgb;
    
    // calculate specular
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
    vec3 specularColor = specularFactor * light.specular * light.diffuse;

    return ambientColor + (1.0f - calcShadow(normal, lightDir)) * (diffuseColor + specularColor);
}