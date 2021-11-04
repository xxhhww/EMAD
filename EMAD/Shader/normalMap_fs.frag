#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos; // 世界坐标下的像素位置
    vec3 plLightPos; // 世界坐标下的点光源位置
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
uniform samplerCube plShadowMap;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform float far_plane;

// 函数声明
float calcDLShadow(vec3 normal, vec3 lightDir);
float calcPLShadow(vec3 normal, vec3 lightDir);

vec3 calcPointLight(Light light, vec3 pos, vec3 view, vec3 normal);
vec3 calcDirecLight(Light light, vec3 pos, vec3 view, vec3 normal);

void main()
{
    // Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // 在切线空间中计算光照
    vec3 plColor = calcPointLight(pointLight, fs_in.TangentFragPos, fs_in.TangentViewPos, normal);
    vec3 dlColor = calcDirecLight(directLight, fs_in.TangentFragPos, fs_in.TangentViewPos, normal);

    FragColor = vec4((plColor + dlColor), 1.0f);
}

float calcDLShadow(vec3 normal, vec3 lightDir){
    // 执行透视除法
    vec3 projCoords = fs_in.PosInDLSpace.xyz / fs_in.PosInDLSpace.w;
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
    vec3 fragToLight = fs_in.FragPos - fs_in.plLightPos;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows 
    float shadow = 0.0;
    float bias = 0.05; 
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(plShadowMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= far_plane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
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

    return ambientColor + (1.0f - calcPLShadow(normal, lightDir)) * (diffuseColor + specularColor);
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

    return ambientColor + (1.0f - calcDLShadow(normal, lightDir)) * (diffuseColor + specularColor);
}