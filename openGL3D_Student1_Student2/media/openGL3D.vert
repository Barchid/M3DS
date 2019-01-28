#version 130

in vec3 position;
in vec3 normale;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 transform;

void main() {
    vec4 eyePosition = vec4(position, 1); // passage en coordonnées homogènes
    eyePosition = transform * eyePosition; // Q 15 : appliquer la rotation sur eye position
    vec4 clipPosition=projection * eyePosition; // transformation par la matrice de projection

    fColor=normale;

    gl_Position=clipPosition; // gl_Position est d'office en clip coordinates
}
