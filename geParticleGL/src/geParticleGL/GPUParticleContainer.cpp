#include <geParticleGL/GPUParticleContainer.h>
#include <cstring>

ge::particle::GPUParticleContainer::GPUParticleContainer(int maxParticleCount, StorageDestination storageDestination, bool fixedSize, unsigned int reallocBlockSize)
	: ComponentSystemContainer(maxParticleCount, fixedSize, reallocBlockSize), storageDestination(storageDestination)
{
}

bool ge::particle::GPUParticleContainer::resize(int reallocBlockCount)
{
	if (ComponentSystemContainer::resize(reallocBlockCount)) {
		for (auto &component : buffers) {
			resizeBuffer(component.second, actSize * componentsSizeOfs[component.first]);
			
			// throws irrational exception
			//component.second->realloc(actSize * componentsSizeOfs[component.first], gl::Buffer::KEEP_DATA);
		}
		return true;
	}

	return false;
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

	bindIndexes.emplace_back(std::make_pair(buffer->second, index));
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

	vertexAttribIndexes.emplace_back(std::make_pair(buffer->second, std::make_pair(vertexArray, index) ));
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

int ge::particle::GPUParticleContainer::syncOnlyAlive(SyncDirection direction)
{
	assert(storageDestination != GPU_ONLY && "Data is only on GPU side, nothing to sync.");

	unsigned int livingParticlesCount = 0;

	for (auto &component : components) {
		if (syncFlags[component.first]) {
			if (direction == CPU_TO_GPU) {
				livingParticlesCount = setBufferData(
					component.first, 
					component.second->data(), 
					componentsSizeOfs[component.first],
					//[this](int idx) { return getComponent<LifeData>(idx).livingFlag; }
					liveParticlePredicate
				);
			}

			if (direction == GPU_TO_CPU) {
				assert(false && "GPU_TO_CPU direction not implemented.");
			}
		}
	}

	return livingParticlesCount;
}

void ge::particle::GPUParticleContainer::getBufferData(const char * componentName, void * data)
{
	// TODO: možná použít pro bìžný, nepersistent buffer buffer->getData z GPUE

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
	// TODO: možná použít pro bìžný, nepersistent buffer buffer->setData z GPUE

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

int ge::particle::GPUParticleContainer::setBufferData(const char * componentName, const void * data, size_t elementSizeOf, PredicateFunction copyIfPredicate)
{
	auto buffer = buffers.find(componentName);
	assert(buffer != buffers.end() && "Component (buffer) not found.");

	char *ptr;
	if (storageDestination == CPU_GPU_PERSISTENT_MAPPED) {
		ptr = (char *) bufferPointers[componentName];
	}
	else {
		ptr = (char *) buffer->second->map(GL_WRITE_ONLY);
	}

	auto buffSize = buffer->second->getSize();
	auto elementCount = buffSize / elementSizeOf;

	unsigned int dstIdx = 0;
	for (unsigned int idx = 0; idx < elementCount; idx++) {
		if (copyIfPredicate(idx, *this)) {
			memcpy(&ptr[dstIdx * elementSizeOf], &((char *)data)[idx * elementSizeOf], elementSizeOf);
			dstIdx++;
		}
	}

	if (storageDestination != CPU_GPU_PERSISTENT_MAPPED) {
		buffer->second->unmap();
	}

	return dstIdx;
}

void ge::particle::GPUParticleContainer::resizeBuffer(std::shared_ptr<gl::Buffer>& buffer, unsigned newSize)
{
	auto newBuffer = std::make_shared<ge::gl::Buffer>(newSize);
	newBuffer->copy(*buffer);

	for (auto &vertexAttrib : vertexAttribIndexes)
	{
		if (vertexAttrib.first == buffer)
		{
			auto &vao = vertexAttrib.second.first;
			auto &attribIndex = vertexAttrib.second.second;

			auto type = gl::VertexArray::NONE;
			if (vao->isAttribInteger(attribIndex)) type = gl::VertexArray::I;
			if (vao->isAttribLong(attribIndex)) type = gl::VertexArray::L;
			vao->addAttrib(newBuffer, attribIndex, vao->getAttribSize(attribIndex),
				vao->getAttribType(attribIndex),
				vao->getAttribStride(attribIndex),
				vao->getAttribRelativeOffset(attribIndex),
				vao->isAttribNormalized(attribIndex),
				vao->getAttribDivisor(attribIndex), type);

			vertexAttrib.first = newBuffer;
		}
	}

	for (auto &bindIndex : bindIndexes)
	{
		if (bindIndex.first == buffer)
		{
			bindIndex.first->unbindBase(GL_SHADER_STORAGE_BUFFER, bindIndex.second);
			
			newBuffer->bind(GL_SHADER_STORAGE_BUFFER);
			newBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, bindIndex.second);
			
			bindIndex.first = newBuffer;
		}
	}
	
	buffer.swap(newBuffer);
}
