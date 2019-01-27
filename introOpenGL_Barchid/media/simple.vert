#version 130

//uniform float coeff; // coefficient de remise à l'échelle pour l'animation du ring

in vec3 position; // position passée par le VBO
in vec4 color; // couleur passée par le VBO
in vec2 texCoord; // coordonnée de texture

out vec4 fcolor; // couleur qui sera passée au fragment shader (varying)
out vec2 fTexCoord; // coordonnée de texture qui sera passée au fragment shader (varying)

void main() {
    // CHANGER LA POSITION POUR GERER L'ANIMATION
    //vec3 newPosition = position * coeff; // on ne peut pas modifier directement la position car c'est un in
    vec3 newPosition = position;

    gl_Position=vec4(newPosition,1.0);
    fcolor = color; // on passe juste la couleur qui a été mise dans le VBO
    fTexCoord = texCoord;
}
