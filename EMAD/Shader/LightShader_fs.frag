#version 330 core
in vec3 posInView;
in vec3 normalInView;

out vec4 FragColor;

// 点光源信息
uniform vec3 lightPosInView;
uniform vec3 lightColor;
uniform float lightAmbient;
uniform float lightSpecular;

// 物体材质信息
uniform vec3 objectColor;

void main()
{   
    float attConstant = 1.0f;
    float attLinear = 0.09f;
    float attQuadratic = 0.032f;

    // calculate attenuation(衰减)
    float distance = length(lightPosInView - posInView);
    float attenuation = 1.0f / (attConstant + attLinear * distance + attQuadratic * (distance * distance));
    
    // calculate ambient
    vec3 ambientColor = lightAmbient * lightColor;
    

    // calculate diffuse
    vec3 normalizeN = normalize(normalInView);
    vec3 lightDir = normalize(lightPosInView - posInView);
    float diffuseFactor = max(dot(lightDir, normalizeN), 0.0f);
    vec3 diffuseColor = diffuseFactor * lightColor * attenuation;
    
    
    // calculate specular
    vec3 viewDir = normalize(-posInView);
    vec3 reflectDir = reflect(-lightDir, normalizeN);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specularColor = specularFactor * lightSpecular * lightColor * attenuation;
    

    FragColor = vec4((ambientColor + diffuseColor + specularColor) * objectColor, 1.0f);
}