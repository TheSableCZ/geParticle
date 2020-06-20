#version 430

layout(location=0) in vec4 pos;

out Particle
{
	vec4 position;
	bool visible;
} particle;

void main(void){
	gl_Position = pos;
	particle.position = pos;
	particle.visible = true;
}