#include "../include/spsc_queue.h"
#include <iostream>
#include <thread>
#include <cassert>

int main() {
	constexpr size_t N = 1000000;
	cpplearn::SPSCQueue<int> q(1024);

	std::thread producer([&]() {
		for (int i = 0; i < N; ++i) {
			while (!q.enqueue(i)) {
				std::this_thread::yield();
			}
		}
	});

	std::thread consumer([&]() {
		int expected = 0, val;
		while (expected < N) {
			if (q.dequeue(val)) {
				assert(val == expected);
				++expected;
			} else {
				std::this_thread::yield();
			}
		}
	});

	producer.join();
	consumer.join();

	std::cout << "Multithreaded SPSC test passed! All " << N << " items dequeued.\n";
	return 0;
}