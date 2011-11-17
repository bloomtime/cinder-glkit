#ifdef GL_ES
precision highp float;
#endif

uniform float     u_time;
uniform vec2      u_mouse;
uniform vec2      u_resolution;
uniform sampler2D u_texture_prev;

varying vec2 v_texcoord;

float getSpring(float b, vec2 pos, float power){
    return (texture2D(u_texture_prev, pos).b - b) * power;
}

void main(){
    vec2 pos = v_texcoord;
    vec2 pixel = 1. / u_resolution;
    float aspect = u_resolution.x / u_resolution.y;
    
    vec4 texel_prev = texture2D(u_texture_prev, pos);
    
    float prev = texel_prev.b;
    float power = .5;
    
    float vel = texel_prev.a - .5;
    vel += getSpring(prev, pos + pixel * vec2(1, 1), 0.03661165235168155 * power);
    vel += getSpring(prev, pos + pixel * vec2(0, 1), 0.0625 * power);
    vel += getSpring(prev, pos + pixel * vec2(-1, 1), 0.03661165235168155 * power);
    vel += getSpring(prev, pos + pixel * vec2(1, 0), 0.0625 * power);
    vel += getSpring(prev, pos + pixel * vec2(-1, 0), 0.0625 * power);
    vel += getSpring(prev, pos + pixel * vec2(1, -1), 0.03661165235168155 * power);
    vel += getSpring(prev, pos + pixel * vec2(0, -1), 0.0625 * power);
    vel += getSpring(prev, pos + pixel * vec2(-1, -1), 0.03661165235168155 * power);
    vel += (.25 - prev) * .025 * power; 

    gl_FragColor = vec4(texel_prev.rgb + vel, vel * .98 + .5);
}