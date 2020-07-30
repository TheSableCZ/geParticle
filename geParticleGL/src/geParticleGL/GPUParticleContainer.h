/** @file GPUParticleContainer.h
 *  @brief Particle container which comunicate with GPU (OpenGL, geGL).
 *  @author Jan Sobol xsobol04
 */

#pragma once

#include <geParticle/ComponentSystemContainer.h>
#include <unordered_map>
#include <geGL/Buffer.h>
#include <geGL/VertexArray.h>
#include <functional>
#include <geParticleGL/Export.h>

namespace ge {
	namespace particle {

		/**
		 * @brief Particle container which comunicate with GPU (OpenGL, geGL).
		 */
		class GEPARTICLEGL_EXPORT GPUParticleContainer : public ComponentSystemContainer {
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

			GPUParticleContainer(int maxParticleCount, StorageDestination storageDestination = CPU_GPU, bool fixedSize = true, unsigned int reallocBlockSize = 64);

			bool resize(int reallocBlockCount) override;

			/**
			 * @brief Bind base of component SSBO to index.
			 */
			void bindComponentBase(const char* componentName, GLuint index);

			/**
			 * @brief Add component buffer as vertex attribute.
			 */
			void addComponentVertexAttrib(
				const char* componentName,
				std::shared_ptr<ge::gl::VertexArray> vertexArray,
				GLuint index,
				GLint nofComponents,
				GLenum type,
				GLsizei stride,
				GLintptr offset
			);

			/**
			 * @brief Sync all components which have positive sync flag.
			 */
			void sync(SyncDirection direction);

			/**
			 * @brief Sync positive sync flag components. Test every index with livingParticlePredicate.
			 */
			int syncOnlyAlive(SyncDirection direction);

			
			void getBufferData(const std::string& componentName, void *data);
			void setBufferData(const std::string& componentName, const void *data);
			int setBufferData(const std::string& componentName, const void *data, size_t elementSizeOf, PredicateFunction copyIfPredicate);

			template <typename T>
			void registerComponent(bool syncFlag = false, std::vector<T> initData = {});

			/**
			 * @brief Bind base of component SSBO to index.
			 */
			template <typename T>
			void bindComponentBase(GLuint index);

			/**
			 * @brief Add component buffer as vertex attribute.
			 */
			template <typename T>
			void addComponentVertexAttrib(
				std::shared_ptr<ge::gl::VertexArray> vertexArray, 
				GLuint index, 
				GLint nofComponents, 
				GLenum type,
				GLsizei stride,
				GLintptr offset
			);

			/**
			 * @brief Sync single component.
			 */
			template <typename T>
			void syncComponent(SyncDirection direction);

			template <typename T>
			void getBufferData(std::vector<T> &data);
			template <typename T>
			void setBufferData(std::vector<T> const &data);

		protected:
			void resizeBuffer(std::shared_ptr<gl::Buffer>& buffer, unsigned newSize);
			
			StorageDestination storageDestination;
			std::unordered_map<std::string, std::shared_ptr<ge::gl::Buffer>> buffers;
			std::unordered_map<std::string, void *> bufferPointers;
			std::unordered_map<std::string, bool> syncFlags;
			std::unordered_map<std::string, size_t> componentsSizeOfs;

			std::vector<std::pair<std::shared_ptr<ge::gl::Buffer>, std::pair<std::shared_ptr<gl::VertexArray>, GLuint>>> vertexAttribIndexes;
			std::vector<std::pair<std::shared_ptr<ge::gl::Buffer>, GLuint>> bindIndexes;
		};
	}
}

template<typename T>
inline void ge::particle::GPUParticleContainer::registerComponent(bool syncFlag, std::vector<T> initData)
{
	ComponentSystemContainer::registerComponent<T>(initData);

	const char* typeName = typeid(T).name();
	auto component = components.find(typeName);

	auto buffer = std::make_shared<ge::gl::Buffer>(actSize * sizeof(T));
	buffer->setData(component->second->data());

	if (storageDestination == GPU_ONLY) {
		component->second->clear();
	}

	buffers.insert({ typeName, buffer });
	syncFlags.insert({ typeName, syncFlag });
	componentsSizeOfs.insert({ typeName, sizeof(T) });
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
		setBufferData(component->first, component->second->data());
	}

	if (direction == GPU_TO_CPU) {
		getBufferData(component->first, component->second->data());
	}
}

template<typename T>
inline void ge::particle::GPUParticleContainer::getBufferData(std::vector<T>& data)
{
	const char* typeName = typeid(T).name();
	auto buffer = buffers.find(typeName);

	assert(buffer != buffers.end() && "Component (buffer) not found.");

	size_t size = buffer->second->getSize() / sizeof(T);

	// TODO: replace with fixed geGL function buffer->getData
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
