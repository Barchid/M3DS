#version 130
uniform sampler2D textureUnit; // récupérer les texels de l'image qui va composer la texture
uniform float coeff; // coefficient de changement de couleur


in vec4 fcolor; // varying : couleur qui a été passée par le vertex shader (calculée par interpolation)
in vec2 fTexCoord; // varying : coordonnée de texture interpolée envoyée par le vertex shader

out vec4 fragColor;
        
void main() {
    //fragColor= texture(textureUnit, fTexCoord); // lire la valeur du texel et l'affecter à fragColor
    fragColor=texture(textureUnit,fTexCoord)*fcolor.b; // fColor étant la couleur interpolée aux sommets(questions précédentes)
    fragColor.g*= (1.0-coeff); // modifier la couleur verte de la couleur du pixel suivant le coefficient en uniform
}
