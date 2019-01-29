#version 130

in vec3 position;
in vec3 normale;

out vec3 fColor;

uniform vec3 lightPosition;
uniform mat4 projection;
uniform mat4 transform;
uniform vec3 diffuseColor;

void main() {
    vec3 L = lightPosition - position;

    L = normalize(L);
    vec4 normale4 = vec4(normale, 0);
    normale4 = transform * normale4;
    vec3 N = vec3(normale4.xyz);
    N = normalize(N);

    float intensite = max(dot(N,L), 0.0);
    fColor = vec3(intensite, intensite, intensite) * diffuseColor;
    gl_Position = projection * transform * vec4(position, 1.0);

    // Question 23 : on commente mdr
//    vec4 eyePosition = vec4(position, 1); // passage en coordonnées homogènes
//    eyePosition = transform * eyePosition; // Q 15 : appliquer la rotation sur eye position
//    vec4 clipPosition=projection * eyePosition; // transformation par la matrice de projection

//    fColor=normale;

//    gl_Position=clipPosition; // gl_Position est d'office en clip coordinates
}
