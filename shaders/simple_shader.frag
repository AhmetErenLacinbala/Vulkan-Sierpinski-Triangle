#version 450

layout(location = 0) out vec4 outColor;

layout (push_constant) uniform Push {
    mat2 transform;
    vec2 offset;
    vec3 color;
    float alpha;
} push;

void main()
{
    outColor = vec4(push.color, push.alpha);
}
