#version 460 core
out vec4 FragColor;

in GS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 color;
} fs_in;


void main() {
    FragColor.rgb = fs_in.color * max(0.1, dot(normalize(fs_in.normal), normalize(vec3(1))));
    FragColor.a = 1.0;
}
