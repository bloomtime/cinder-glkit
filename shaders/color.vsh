uniform mat4 u_mvp_matrix;

attribute vec3 position;

void main(){
    gl_Position = u_mvp_matrix * vec4(position, 1.);
}