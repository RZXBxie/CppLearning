#ifndef CPPLEARNING_MPMS_QUEUE_H
#define CPPLEARNING_MPMS_QUEUE_H

#include <utility>
#include <atomic>
#include <iterator>
#include <memory>

namespace cpplearn {
	template <typename T>
	class MPMCQueue {
	public:
		explicit MPMCQueue(const size_t capacity) : capacity_(capacity + 1) {
			data_ = allocator_.allocate(capacity_);
			head_ = tail_ = 0;
		}

		~MPMCQueue() {
			clear();
			allocator_.deallocate(data_, capacity_);
		}

		template <typename... Args>
		bool enqueue(Args&&... args) {
			size_t tail = tail_.load(std::memory_order_relaxed);
			if (const size_t next_tail = increment(tail); next_tail == head_.load(std::memory_order_acquire)) {
				return false;
			}

			// 生产者不断尝试更新tail的值
			while (!tail_.compare_exchange_weak(tail, increment(tail),
				std::memory_order_release, std::memory_order_relaxed)) {
				// 如果尝试更新失败，则重新获取值并重试
				// 需要注意的是，tail的更新在CAS过程中已经完成了，无论CAS成功与否，tail的值都会被更新为tail_中存储的值
			}
			allocator_.construct(&data_[tail], std::forward<Args>(args)...);
			return true;
		}

		bool dequeue(T& item) {
			size_t head = head_.load(std::memory_order_relaxed);
			if (head == tail_.load(std::memory_order_acquire)) {
				return false;
			}

			// 通过CAS尝试更新head的值
			while (!head_.compare_exchange_weak(head, increment(head),
				std::memory_order_release, std::memory_order_relaxed)) {
				// 如果尝试更新失败，则重新获取值并重试
			}

			item = std::move(data_[head]);
			allocator_.destroy(&data_[head]);
			return true;
		}

		bool empty() const {
			return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
		}

		bool full() const {
			return increment(tail_.load(std::memory_order_acquire)) == head_.load(std::memory_order_acquire);
		}

		void clear() {
			T tmp;
			while (dequeue(tmp)) {}
		}

	private:
		const size_t capacity_;
		T *data_;
		std::allocator<T> allocator_;

		alignas(64) std::atomic<size_t> head_;
		char pad[64 - sizeof(size_t)];
		alignas(64) std::atomic<size_t> tail_;

		size_t increment(const size_t index) const {
			return (index + 1) % capacity_;
		}
	};
}



#endif //CPPLEARNING_MPMS_QUEUE_H