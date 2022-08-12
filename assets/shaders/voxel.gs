#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in VS_OUT
{
    vec3 pos;
    int flag;
    int faces;
} gs_in[];

out GS_OUT
{
    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 color;
} gs_out;

layout (std140, binding = 0) uniform camera_u {
    vec4 position;
    mat4 vp;
} uCamera;

void emit_face(vec4 pos, vec3 dir) {
    vec3 tangent = dir.yzx;
    vec3 bitangent = normalize(cross(dir, tangent));

    float scale = 0.5;

    gs_out.normal = dir;
    
    gs_out.pos = pos.xyz + dir * scale - tangent * scale + bitangent * scale;
    gs_out.uv = vec2(1,0);
    gl_Position = uCamera.vp * vec4(gs_out.pos, pos.w);

    EmitVertex();

    gs_out.pos = pos.xyz + dir * scale + tangent * scale + bitangent * scale;
    gs_out.uv = vec2(0,0);
    gl_Position = uCamera.vp * vec4(gs_out.pos, pos.w);

    EmitVertex();

    gs_out.pos = pos.xyz + dir * scale - tangent * scale - bitangent * scale;
    gs_out.uv = vec2(0,1);
    gl_Position = uCamera.vp * vec4(gs_out.pos, pos.w);

    EmitVertex();

    gs_out.pos = pos.xyz + dir * scale + tangent * scale - bitangent * scale;
    gs_out.uv = vec2(1,1);
    gl_Position = uCamera.vp * vec4(gs_out.pos, pos.w);

    EmitVertex();

   
}

void emit_cube(vec4 pos) {
    emit_face(pos, vec3(1,0,0));
    emit_face(pos, vec3(0,1,0));
    emit_face(pos, vec3(-1,0,0));
    emit_face(pos, vec3(0,-1,0));
    emit_face(pos, vec3(0,0,1));
    emit_face(pos, vec3(0,0,-1));
}

void main() {
    gs_out.color = vec3(0,1,0);

    //if (gs_in[0].flag != 0) {
        emit_cube(vec4(gs_in[0].pos, 1.0));
    //}
}