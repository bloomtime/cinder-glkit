#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord;

void main(){
    gl_FragColor = vec4(v_texcoord, 0.,1.);
}