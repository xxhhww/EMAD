#version 330 core
in vec4 FragPos; // 世界坐标下像素点的位置

uniform vec3 plLightPos; // 世界坐标下点光源的位置
uniform float far_plane; // 透视投影远平面

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - plLightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}