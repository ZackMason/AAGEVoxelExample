#version 460 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aNormal;
layout ( location = 2 ) in vec2 aUV;

out VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} vs_out;

layout (std140, binding = 0) uniform camera_u {
    vec4 position;
    mat4 vp;
} uCamera;

uniform mat4 uM;

void main() {
    vs_out.pos = (uM * vec4(aPos, 1.0)).xyz;
    vs_out.normal = aNormal;
    vs_out.uv = aUV;

    gl_Position = (uCamera.vp * uM * vec4(aPos, 1.0));
}