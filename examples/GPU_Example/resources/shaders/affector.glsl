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

//uniform vec4 attPos;
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

		//p[gid].position = vec3(particleCount, -1.f, 0.f);

		//p[gid].velocity = vec4(0.f, 0.f, 0.f, 1.f);

        //MassPointData part = p[gid];

		//part.velocity = vec3(0.f, 0.f, 0.f);
		//part.position = vec3(0.f, 0.f, 0.f);

		//part.velocity *= 1 - .99f * dt;
		//part.velocity += vec3(0.0f, -9.81f, 0.0f) * dt;
		//part.position += part.velocity * dt;
		//float _dt = dt;
		//part.position = vec3(0.f, 0.f, 0.f);
		//p[gid] = part;

        // The direction of this vector corresponds to the direction of the gravity.
        // A zero vector will freeze the particles when not interacted with them.
        //vec4 gv = normalize(vec4(0, -1, 0, 0));

        // Direction vector of the acceleration.
        // The length of this vector determines strength of attractor.
        //vec4 a = vec4(0);

        //if(attPos.w >= 0.0f){
            // Take particle's distance from origin as coefficient, so that every
            // particle behaves differently. Change coefficient to change attractor's strength.
        //    a = normalize(attPos - part.currPos) * length(part.currPos.xyz) * 10.f;
        //}

        // Add gravity vector times its strength.
        //a += gv * 0.5f;

        // Slightly modified verlet integration http://lonesock.net/article/verlet.html
        //vec4 tempCurrPos = 1.99f * part.currPos - 0.99f * part.prevPos + a * dt * dt;
        //part.prevPos = part.currPos;
        //part.currPos = tempCurrPos;

        //p[gid] = part;
    }
}