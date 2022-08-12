#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} fs_in;


void main() {
    FragColor.rgb = vec3(0,1,0) * max(0.1, dot(normalize(fs_in.normal), normalize(vec3(1,2,1.5))));
    FragColor.a = 1.0;
}
