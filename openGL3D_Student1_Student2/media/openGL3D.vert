#version 130

in vec3 position;
in vec3 color;

out vec3 fColor;

uniform mat4 projection;

void main() {
    vec4 eyePosition = vec4(position, 1); // passage en coordonnées homogènes
    vec4 clipPosition=projection * eyePosition; // transformation par la matrice de projection

    fColor=color;

    gl_Position=clipPosition; // gl_Position est d'office en clip coordinates
}
