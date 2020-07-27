R"(
#version 330

//in vec3 particlecolor;
in vec4 color;

out vec4 fragColor;

void main()
{
    fragColor = color;
	//fragColor = vec4(1.f, 0.f, 0.f, 0.f);
}
)"