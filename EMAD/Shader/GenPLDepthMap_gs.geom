#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 plSpaceMats[6];

out vec4 FragPos; // 世界坐标下的顶点位置

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = plSpaceMats[face] * FragPos; // 以点光源为观察者的投影空间下的位置
            EmitVertex();
        }    
        EndPrimitive();
    }
}