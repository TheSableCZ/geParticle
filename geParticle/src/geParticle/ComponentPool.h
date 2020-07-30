#pragma once

namespace ge {
	namespace particle {

		class ComponentPoolBase {
		public:
			virtual ~ComponentPoolBase() = default;
			virtual void clear() = 0;
			virtual void *data() = 0;
			virtual void resize(unsigned int n) = 0;
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

			void resize(unsigned n) override
			{
				pool.resize(n);
			}

		private:
			std::vector<T> pool;
		};

	}
}