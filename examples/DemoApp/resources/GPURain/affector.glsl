R"(
#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct MassPointData {
	vec4 position;
	vec4 velocity;
};

layout(std430, binding=0) buffer Pos {
   MassPointData p[];
};

struct GPULifeData {
	float life;
	bool livingFlag;
};

layout(std430, binding=1) buffer Life {
   GPULifeData life[];
};

uniform float dt;
uniform uint particleCount;

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint gid = gl_GlobalInvocationID.x;

    if(gid < particleCount) {
		p[gid].velocity.xyz += vec3(0.0f, -9.81f, 0.0f) * dt;
		p[gid].position.xyz += p[gid].velocity.xyz * dt;

		life[gid].life -= dt;
		if(life[gid].life <= 0)
			life[gid].livingFlag = false;
    }
}
)"