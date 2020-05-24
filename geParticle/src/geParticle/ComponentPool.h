#pragma once

namespace ge {
	namespace particle {

		class IComponentPool {
		public:
			virtual void clear() = 0;
			virtual void *data() = 0;
		};

		template <typename T>
		class ComponentPool : public IComponentPool {
		public:
			ComponentPool(int size) {
				pool = std::vector<T>(size);
			}

			T &get(int idx) {
				return pool.at(idx);
			}

			void clear() override {
				pool.clear();
			}

			void *data() override {
				return pool.data();
			}

		private:
			std::vector<T> pool;
		};

	}
}