#ifdef GL_ES
precision highp float;
#endif

//uniform float     u_time;
//uniform vec2      u_mouse;
//uniform vec2      u_resolution;
//uniform sampler2D u_texture_prev;
//
//float getSpring(float b, vec2 pos, float power){
//    return (texture2D(u_texture_prev, pos).b - b) * power;
//}

varying vec2 v_texcoord;

void main(){
//    vec2 pos = gl_FragCoord.xy / u_resolution;
//    vec2 pixel = 1. / u_resolution;
//    float aspect = u_resolution.x / u_resolution.y;
//    
//    vec4 texel_prev = texture2D(u_texture_prev, pos);
//    
//    float prev = texel_prev.b;
//    float power = .5;
//    
//    float vel = texel_prev.a - .5;
//    vel += getSpring(prev, pos + pixel * vec2(2, 3), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, 3), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, 3), 0.005681818181818182 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, 3), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, 3), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(3, 2), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, 2), 0.0066566640639421 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, 2), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, 2), 0.011363636363636364 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, 2), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, 2), 0.0066566640639421 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-3, 2), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(3, 1), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, 1), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, 1), 0.014691968395607415 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, 1), 0.017045454545454544 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, 1), 0.014691968395607415 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, 1), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-3, 1), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(3, 0), 0.005681818181818182 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, 0), 0.011363636363636364 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, 0), 0.017045454545454544 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, 0), 0.017045454545454544 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, 0), 0.011363636363636364 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-3, 0), 0.005681818181818182 * power);
//    vel += getSpring(prev, pos + pixel * vec2(3, -1), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, -1), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, -1), 0.014691968395607415 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, -1), 0.017045454545454544 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, -1), 0.014691968395607415 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, -1), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-3, -1), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(3, -2), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, -2), 0.0066566640639421 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, -2), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, -2), 0.011363636363636364 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, -2), 0.010022341036933013 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, -2), 0.0066566640639421 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-3, -2), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(2, -3), 0.0022411859348636983 * power);
//    vel += getSpring(prev, pos + pixel * vec2(1, -3), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(0, -3), 0.005681818181818182 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-1, -3), 0.004759786021770571 * power);
//    vel += getSpring(prev, pos + pixel * vec2(-2, -3), 0.0022411859348636983 * power);
//    vel += (.25 - prev) * .025 * power; 
//    
//    vel += max(0., .1 * (1. - (length((pos - u_mouse) * vec2(aspect, 1.)) * (16. + sin(u_time) * 4.))));
//    
//    gl_FragColor = vec4(texel_prev.rgb + vel, vel * .98 + .5);
    gl_FragColor = vec4(v_texcoord, 0.,1.);
}