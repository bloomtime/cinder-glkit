#ifdef GL_ES
precision highp float;
#endif

uniform vec4 u_color;

void main(){
    gl_FragColor = vec4(u_color);
}