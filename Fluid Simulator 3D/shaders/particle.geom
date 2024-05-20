#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 100) out;

uniform mat4 projection;
uniform float radius;
const int num_segments = 32; // Number of segments to approximate the circle

void main()
{
    vec2 center = gl_in[0].gl_Position.xy;
    
    for (int i = 0; i <= num_segments; ++i)
    {
        float theta = float(i) / float(num_segments) * 2.0 * 3.1415926; // Angle in radians
        float x = radius * cos(theta); // x coordinate
        float y = radius * sin(theta); // y coordinate

        vec2 pos = center + vec2(x, y);
        gl_Position = projection * vec4(pos, 0.0, 1.0);
        EmitVertex();
    }

    EndPrimitive();
}
