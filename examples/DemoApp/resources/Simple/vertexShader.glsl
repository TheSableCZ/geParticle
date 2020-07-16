#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec3 color;

out vec3 particlecolor;

out Particle
{
	vec3 position;
} particle;

void main(void){
	gl_Position = vec4(pos, 1.f);
	particle.position = pos;

	particlecolor = color;
}