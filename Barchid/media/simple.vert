#version 130
        
in vec3 position; // position passée par le VBO
in vec4 color; // couleur passée par le VBO

out vec4 fcolor; // couleur qui sera passée au fragment shader (varying)
        
void main() {
    gl_Position=vec4(position,1.0);
    fcolor = color; // on passe juste la couleur qui a été mise dans le VBO
}
