#ifndef CPPLEARNING_SPSC_QUEUE_H
#define CPPLEARNING_SPSC_QUEUE_H

#include <atomic>
#include <memory>
#include <utility>

namespace cpplearn {
	template<typename T>
	class SPSCQueue {
	public:
		explicit SPSCQueue(const size_t capacity)
			: capacity_(capacity + 1) {
			data_ = allocator_.allocate(capacity_);
		}

		~SPSCQueue() {
			clear();
			allocator_.deallocate(data_, capacity_);
		}

		// 禁止拷贝和赋值
		SPSCQueue(const SPSCQueue &) = delete;

		SPSCQueue &operator=(const SPSCQueue &) = delete;

		// 入队，完美转发
		template<typename... Args>
		// Args是一个万能应用，配合forward实现完美转发
		bool enqueue(Args &&... args) {
			const size_t tail = tail_.load(std::memory_order_relaxed);
			const size_t next_tail = increment(tail);
			if (next_tail == head_.load(std::memory_order_acquire)) {
				return false; //队列已满
			}

			allocator_.construct(&data_[tail], std::forward<Args>(args)...);
			tail_.store(next_tail, std::memory_order_release);
			return true;
		}

		bool dequeue(T &item) {
			const size_t head = head_.load(std::memory_order_relaxed);
			if (head == tail_.load(std::memory_order_acquire)) {
				return false; //队列已空
			}
			item = std::move(data_[head]);
			allocator_.destroy(&data_[head]);
			head_.store(increment(head), std::memory_order_release);
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
			// 逐个释放每个对象的内存
			while (dequeue(tmp)) {
			}
		}

	private:
		const size_t capacity_;
		T *data_;
		std::allocator<T> allocator_;

		// 由于生产者和消费者频繁读写tail和head，让他们强制以64字节对齐，避免将数据映射到同一cache中导致伪共享等问题。
		alignas(64) std::atomic<size_t> head_{0};
		char pad_[64 - sizeof(std::atomic<size_t>)]; // 仅用于占位，无需初始化
		alignas(64) std::atomic<size_t> tail_{0};

		size_t increment(const size_t idx) const {
			return (idx + 1) % capacity_;
		}
	};
}


#endif //CPPLEARNING_SPSC_QUEUE_H
