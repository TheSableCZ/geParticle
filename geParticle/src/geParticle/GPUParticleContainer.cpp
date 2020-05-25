#include <geParticle/GPUParticleContainer.h>
#include <cstring>

ge::particle::GPUParticleContainer::GPUParticleContainer(int maxParticleCount, StorageDestination storageDestination)
	: storageDestination(storageDestination), ComponentSystemContainer(maxParticleCount, false)
{
}

void ge::particle::GPUParticleContainer::bindComponentBase(const char* componentName, GLuint index)
{
	auto buffer = buffers.find(componentName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	buffer->second->bind(GL_SHADER_STORAGE_BUFFER);
	buffer->second->bindBase(GL_SHADER_STORAGE_BUFFER, index);

	if (buffer->second->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not bind shader storage buffer.");
	}
}

void ge::particle::GPUParticleContainer::addComponentVertexAttrib(const char * componentName, std::shared_ptr<ge::gl::VertexArray> vertexArray, GLuint index, GLint nofComponents, GLenum type, GLsizei stride, GLintptr offset)
{
	auto buffer = buffers.find(componentName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	buffer->second->bind(GL_ARRAY_BUFFER);
	vertexArray->addAttrib(buffer->second, index, nofComponents, type, stride, offset);

	if (buffer->second->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not create vertex attrib.");
	}
}

void ge::particle::GPUParticleContainer::sync(SyncDirection direction)
{
	assert(storageDestination != GPU_ONLY && "Data is only on GPU side, nothing to sync.");

	for (auto &component : components) {
		if (syncFlags[component.first]) {
			if (direction == CPU_TO_GPU) {
				setBufferData(component.first, component.second->data());
			}

			if (direction == GPU_TO_CPU) {
				getBufferData(component.first, component.second->data());
			}
		}
	}
}

void ge::particle::GPUParticleContainer::getBufferData(const char * componentName, void * data)
{
	auto buffer = buffers.find(componentName);
	assert(buffer != buffers.end() && "Component (buffer) not found.");

	void *ptr;
	if (storageDestination == CPU_GPU_PERSISTENT_MAPPED) {
		ptr = bufferPointers[componentName];
	}
	else {
		ptr = buffer->second->map(GL_READ_ONLY);
	}

	auto buffSize = buffer->second->getSize();
	memcpy(data, ptr, buffSize);

	if (storageDestination != CPU_GPU_PERSISTENT_MAPPED) {
		buffer->second->unmap();
	}
}

void ge::particle::GPUParticleContainer::setBufferData(const char * componentName, const void * data)
{
	auto buffer = buffers.find(componentName);
	assert(buffer != buffers.end() && "Component (buffer) not found.");

	void *ptr;
	if (storageDestination == CPU_GPU_PERSISTENT_MAPPED) {
		ptr = bufferPointers[componentName];
	}
	else {
		ptr = buffer->second->map(GL_WRITE_ONLY);
	}

	auto buffSize = buffer->second->getSize();
	memcpy(ptr, data, buffSize);

	if (storageDestination != CPU_GPU_PERSISTENT_MAPPED) {
		buffer->second->unmap();
	}
}
