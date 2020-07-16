#version 330

//in vec3 particlecolor;
in vec3 color;

out vec4 fragColor;

void main()
{
    fragColor = vec4(color, 1.0f);
	//fragColor = vec4(1.f, 0.f, 0.f, 1.f);
}