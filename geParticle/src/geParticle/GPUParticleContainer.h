#pragma once

#include <geParticle/ComponentSystemContainer.h>
#include <unordered_map>
#include <geGL/Buffer.h>
#include <geGL/VertexArray.h>

namespace ge {
	namespace particle {

		class GPUParticleContainer : public ComponentSystemContainer {
		public:
			enum StorageDestination {
				CPU_GPU,
				GPU_ONLY,
				CPU_GPU_PERSISTENT_MAPPED
			};

			enum SyncDirection {
				CPU_TO_GPU,
				GPU_TO_CPU
			};

			GPUParticleContainer(int maxParticleCount, bool registerLifeData = true, StorageDestination storageDestination = CPU_GPU);

			void bindComponentBase(const char* componentName, GLuint index);
			void addComponentVertexAttrib(
				const char* componentName,
				std::shared_ptr<ge::gl::VertexArray> vertexArray,
				GLuint index,
				GLint nofComponents,
				GLenum type,
				GLsizei stride,
				GLintptr offset
			);
			void sync(SyncDirection direction);
			void getBufferData(const char* componentName, void *data);
			void setBufferData(const char* componentName, const void *data);

			template <typename T>
			void registerComponent(bool syncFlag = false);
			template <typename T>
			void bindComponentBase(GLuint index);
			template <typename T>
			void addComponentVertexAttrib(
				std::shared_ptr<ge::gl::VertexArray> vertexArray, 
				GLuint index, 
				GLint nofComponents, 
				GLenum type,
				GLsizei stride,
				GLintptr offset
			);
			template <typename T>
			void syncComponent(SyncDirection direction);

			template <typename T>
			void getBufferData(std::vector<T> &data);
			template <typename T>
			void setBufferData(std::vector<T> const &data);

		protected:
			StorageDestination storageDestination;
			std::unordered_map<const char *, std::shared_ptr<ge::gl::Buffer>> buffers;
			std::unordered_map<const char *, void *> bufferPointers;
			std::unordered_map<const char *, bool> syncFlags;
		};

	}
}

template<typename T>
inline void ge::particle::GPUParticleContainer::registerComponent(bool syncFlag)
{
	ComponentSystemContainer::registerComponent<T>();

	const char* typeName = typeid(T).name();
	auto component = components.find(typeName);

	auto buffer = std::make_shared<ge::gl::Buffer>(maxParticles * sizeof(T));
	buffer->setData(component->second->data());

	if (storageDestination == GPU_ONLY) {
		component->second->clear();
	}

	buffers.insert({ typeName, buffer });
	syncFlags.insert({ typeName, syncFlag });
}

template<typename T>
inline void ge::particle::GPUParticleContainer::bindComponentBase(GLuint index)
{
	const char* typeName = typeid(T).name();
	bindComponentBase(typeName, index);
}

template<typename T>
inline void ge::particle::GPUParticleContainer::addComponentVertexAttrib(std::shared_ptr<ge::gl::VertexArray> vertexArray, GLuint index, GLint nofComponents, GLenum type, GLsizei stride, GLintptr offset)
{
	const char* typeName = typeid(T).name();
	addComponentVertexAttrib(typeName, vertexArray, index, nofComponents, type, stride, offset);
}

template<typename T>
inline void ge::particle::GPUParticleContainer::syncComponent(SyncDirection direction)
{
	assert(storageDestination != GPU_ONLY && "Data is only on GPU side, nothing to sync.");

	const char* typeName = typeid(T).name();
	auto component = components.find(typeName);

	assert(component != components.end() && "Component not found.");

	if (direction == CPU_TO_GPU) {
		setBufferData(component.first, component.second->data());
	}

	if (direction == GPU_TO_CPU) {
		getBufferData(component.first, component.second->data());
	}
}

template<typename T>
inline void ge::particle::GPUParticleContainer::getBufferData(std::vector<T>& data)
{
	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	size_t size = buffer->second->getSize() / sizeof(T);

	data.resize(size);
	getBufferData(typeName, data.data());
}

template<typename T>
inline void ge::particle::GPUParticleContainer::setBufferData(std::vector<T> const & data)
{
	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	setBufferData(typeName, data.data());
}

/*
template<typename T>
inline std::vector<T> ge::particle::GPUParticleContainer::getBufferData()
{
	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	std::cout << "getBufferData: buffer size: " << buffer->second->getSize() << std::endl;

	T *ptr;

	if (storageDestination == CPU_GPU_PERSISTENT_MAPPED) {
		ptr = (T *) bufferPointers[typeName];
	} else {
		ptr = (T *) buffer->second->map(GL_READ_ONLY);
	}

	std::vector<T> vect;
	vect.reserve(size());

	for (unsigned int i = 0; i < size(); i++) {
		vect.push_back(ptr[i]);
	}

	buffer->second->unmap();

	//buffer->second->getData<T>(vect);

	return vect;
}
*/
