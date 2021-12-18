#version 450 core
// LightPass.frag的计算处于世界坐标之下
out vec4 FragColor;

in VSOut{
	vec3 fPosition;
	vec2 fTexcoord;
} fsin;

// Uniform
const float PI = 3.14159265359;

const uint MAX_POINTLIGHT_SIZE = 6u;

struct PointLightData{
    vec4 lightColor;
    vec3 lightPos;
    float intensity;
};

layout(std430, binding = 0) buffer LightData1{
    vec3 viewPos;
    int pointLightNums;
    PointLightData pointLightDatas[];
};

layout (binding = 0) uniform sampler2D BaseColor;
layout (binding = 1) uniform sampler2D Position;
layout (binding = 2) uniform sampler2D Normal;
layout (binding = 3) uniform sampler2D Other;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main(){
	// Load Data From Texture
	vec3 baseColor = texture(BaseColor, fsin.fTexcoord).rgb;
    baseColor = pow(baseColor, vec3(2.2f));

	vec3 worldPos = texture(Position, fsin.fTexcoord).rgb;
	vec3 normal = texture(Normal, fsin.fTexcoord).rgb;
	vec3 other = texture(Other, fsin.fTexcoord).rgb;
	float metallic = other.r;
	float roughness = other.g;
	float ao = other.b;

    vec3 N = normalize(normal);
    vec3 V = normalize(vec3(viewPos) - worldPos);

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, baseColor, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < pointLightNums; ++i) 
    {
        // calculate per-light radiance
        vec3 LightPos = pointLightDatas[i].lightPos;
        vec3 LightColor = vec3(pointLightDatas[i].lightColor) * (pointLightDatas[i].intensity);

        vec3 L = normalize(vec3(LightPos) - worldPos);
        vec3 H = normalize(V + L);
        float distance    = length(vec3(LightPos) - worldPos);
        float attenuation = 1.0f / (distance * distance);
        vec3 radiance     = LightColor * attenuation;        

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);
        //vec3 F    = fresnelSchlick(max(dot(N, V), 0.0f), F0); 
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0f), F0);       

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;     

        vec3 numinator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; 
        vec3 specular     = numinator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * baseColor / PI + specular) * radiance * NdotL; 
    }   

    vec3 ambient = vec3(0.03) * baseColor * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
