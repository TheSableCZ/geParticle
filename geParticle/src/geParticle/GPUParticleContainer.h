#pragma once

#include <geParticle/ComponentSystemContainer.h>
#include <unordered_map>
#include <geGL/Buffer.h>
#include <geGL/VertexArray.h>

namespace ge {
	namespace particle {

		class GPUParticleContainer : public ComponentSystemContainer {
		public:
			GPUParticleContainer(int maxParticleCount);

			template <typename T>
			void registerComponent();

			void initBuffers();
			template <typename T>
			void bindComponentBase(GLuint index);
			template <typename T>
			void addComponentAttrib(
				std::shared_ptr<ge::gl::VertexArray> vertexArray, 
				GLuint index, 
				GLint nofComponents, 
				GLenum type,
				GLsizei stride,
				GLintptr offset
			);
		private:
			bool buffersInited = false;

			std::unordered_map<const char *, std::shared_ptr<ge::gl::Buffer>> buffers;
			std::unordered_map<const char *, std::size_t> componentsSizeOf;
		};

	}
}

inline ge::particle::GPUParticleContainer::GPUParticleContainer(int maxParticleCount)
	: ComponentSystemContainer(maxParticleCount, false)
{
}

template<typename T>
inline void ge::particle::GPUParticleContainer::registerComponent()
{
	assert(buffersInited == false && "Buffers already created & inited.");

	ComponentSystemContainer::registerComponent<T>();

	const char* typeName = typeid(T).name();
	componentsSizeOf.insert({ typeName, sizeof(T) });
}

inline void ge::particle::GPUParticleContainer::initBuffers()
{
	for (auto &component : components) {
		auto sizeOf = componentsSizeOf[component.first];
		//auto buffer = std::make_shared<ge::gl::Buffer>(maxParticles * sizeOf, component.second->data(), GL_STATIC_DRAW);
		auto buffer = std::make_shared<ge::gl::Buffer>(maxParticles * sizeOf);
		buffer->bind(GL_SHADER_STORAGE_BUFFER);
		buffer->setData(component.second->data());

		component.second->clear();

		buffers.insert({ component.first, buffer });
	}

	buffersInited = true;
}

template<typename T>
inline void ge::particle::GPUParticleContainer::bindComponentBase(GLuint index)
{
	assert(buffersInited == true && "Call initBuffers() first.");

	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	std::cout << "Buffer size: " << buffer->second->getSize() << std::endl;

	buffer->second->bindBase(GL_SHADER_STORAGE_BUFFER, index);

	if (buffer->second->getContext().glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not  bind shader storage buffer for particles!");
	}
}

template<typename T>
inline void ge::particle::GPUParticleContainer::addComponentAttrib(std::shared_ptr<ge::gl::VertexArray> vertexArray, GLuint index, GLint nofComponents, GLenum type, GLsizei stride, GLintptr offset)
{
	assert(buffersInited == true && "Call initBuffers() first.");

	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	buffer->second->bind(GL_ARRAY_BUFFER);
	vertexArray->addAttrib(buffer->second, index, nofComponents, type, stride, offset);
}
