#version 460 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in int aFlag;
layout ( location = 2 ) in int aFaces;

out VS_OUT {
    vec3 pos;
    int flag;
    int faces;
} vs_out;

layout (std140, binding = 0) uniform camera_u {
    vec4 position;
    mat4 vp;
} uCamera;

uniform mat4 uM;

void main() {
    vs_out.pos = (uM * vec4(aPos, 1.0)).xyz;
    vs_out.flag = aFlag;
    vs_out.faces = aFaces;

    gl_Position = (uM * vec4(aPos, 1.0));
}