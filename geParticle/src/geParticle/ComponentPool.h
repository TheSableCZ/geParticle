#pragma once

namespace ge {
	namespace particle {

		class ComponentPoolBase {
		public:
			virtual void clear() = 0;
			virtual void *data() = 0;
		};

		template <typename T>
		class ComponentPool : public ComponentPoolBase {
		public:
			ComponentPool(int size, std::vector<T> initData = {}) {
				pool.resize(size);

				if (!initData.empty()) {
					std::copy(initData.begin(), initData.end(), pool.begin());
				}
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