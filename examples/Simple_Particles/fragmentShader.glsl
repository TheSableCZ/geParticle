#version 330

in vec3 particlecolor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(particlecolor, 1.0f);
}