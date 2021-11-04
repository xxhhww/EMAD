#version 330 core
in vec4 FragPos; // �������������ص��λ��

uniform vec3 plLightPos; // ���������µ��Դ��λ��
uniform float far_plane; // ͸��ͶӰԶƽ��

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - plLightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}