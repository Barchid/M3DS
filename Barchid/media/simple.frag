#version 130
in vec4 fcolor; // varying : couleur qui a été passée par le vertex shader (calculée par interpolation)
out vec4 fragColor;
        
void main() {
    fragColor=fcolor;
}
