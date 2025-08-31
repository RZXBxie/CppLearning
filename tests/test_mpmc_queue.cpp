#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN   // 提供 main()
#include <doctest/doctest.h>

#include "mpmc_queue.h"   // 你的头文件在 include/ 下，CMake 已保证 include 路径
#include <thread>
#include <vector>
#include <atomic>

using cpplearn::MPMCQueue;

TEST_CASE("single-thread basic") {
	MPMCQueue<int> q(4);
	REQUIRE(q.empty());
	REQUIRE_FALSE(q.full());

	REQUIRE(q.enqueue(1));
	REQUIRE(q.enqueue(2));
	REQUIRE(q.enqueue(3));
	REQUIRE(q.enqueue(4));
	REQUIRE(q.full());

	int v;
	REQUIRE(q.dequeue(v)); REQUIRE(v == 1);
	REQUIRE(q.dequeue(v)); REQUIRE(v == 2);
	REQUIRE(q.dequeue(v)); REQUIRE(v == 3);
	REQUIRE(q.dequeue(v)); REQUIRE(v == 4);
	REQUIRE_FALSE(q.dequeue(v));
	REQUIRE(q.empty());
}

TEST_CASE("single-thread clear") {
	MPMCQueue<int> q(8);
	for (int i = 0; i < 6; ++i) REQUIRE(q.enqueue(i));
	REQUIRE_FALSE(q.empty());
	q.clear();
	REQUIRE(q.empty());
}

TEST_CASE("multi-thread producer-consumer") {
	constexpr size_t kCap        = 1024;
	constexpr int    kPerThread  = 100;
	MPMCQueue<int> q(kCap);

	std::atomic<int> produced{0};
	std::atomic<int> consumed{0};

	auto producer = [&] {
		for (int i = 0; i < kPerThread; ++i) {
			while (!q.enqueue(i)) std::this_thread::yield();
			++produced;
		}
	};
	auto consumer = [&] {
		int v;
		for (int i = 0; i < kPerThread; ++i) {
			while (!q.dequeue(v)) std::this_thread::yield();
			++consumed;
		}
	};

	constexpr int kThreads = 4;
	std::vector<std::thread> ths;
	for (int i = 0; i < kThreads; ++i) ths.emplace_back(producer);
	for (int i = 0; i < kThreads; ++i) ths.emplace_back(consumer);
	for (auto& t : ths) t.join();

	REQUIRE(produced.load() == kThreads * kPerThread);
	REQUIRE(consumed.load()  == kThreads * kPerThread);
	REQUIRE(q.empty());
}