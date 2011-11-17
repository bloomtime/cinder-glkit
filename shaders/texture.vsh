uniform mat4 u_mvp_matrix;

attribute vec3 position;
attribute vec2 texcoord;

varying vec2 v_texcoord;

void main(){
    v_texcoord = texcoord;
    gl_Position = u_mvp_matrix * vec4(position, 1.);
}