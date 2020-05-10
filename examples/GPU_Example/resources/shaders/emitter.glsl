#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct GPULifeData {
	float life;
	bool livingFlag;
};

layout(std430, binding=1) buffer Life {
   GPULifeData life[];
};

struct MassPointData {
	vec4 position;
	vec4 velocity;
};

layout(std430, binding=0) buffer Pos {
   MassPointData pos[];
};

uniform uint particleCount;
uniform uint newParticles;

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint gid = gl_GlobalInvocationID.x;

    if(gid < newParticles) {
		int i;
		for (i = 0; i < particleCount; i++) {
			if (life[i].livingFlag == false) {
				break;
			}
		}

		life[i].life = 3.2f;
		life[i].livingFlag = true;
		pos[i].position = vec4(0.f, 1.f, 0.f, 1.f);
		pos[i].velocity = vec4(0.f, 0.f, 0.f, 1.f);
    }
}