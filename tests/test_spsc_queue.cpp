#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "spsc_queue.h"
#include <string>

struct TestObj {
	int id;
	std::string name;

	template<typename S>
	// S是一个万能引用，配合forward实现完美转发
	TestObj(const int i, S &&name) : id(i), name(std::forward<S>(name)) {
	}

	TestObj() : id(0) {
	}
};

TEST_CASE("SPSCQueue basic int enqueue/dequeue") {
	cpplearn::SPSCQueue<int> q(3);
	CHECK(q.empty());
	CHECK_FALSE(q.full());
	CHECK(q.enqueue(1));
	CHECK(q.enqueue(2));
	CHECK(q.enqueue(3));
	CHECK_FALSE(q.enqueue(4)); // 队列已满

	int val = 0;
	CHECK(q.dequeue(val));
	CHECK(val == 1);
	CHECK(q.dequeue(val));
	CHECK(val == 2);
	CHECK(q.dequeue(val));
	CHECK(val == 3);
	CHECK_FALSE(q.dequeue(val));
	CHECK(q.empty());
}

TEST_CASE("SPSCQueue with custom object") {
	cpplearn::SPSCQueue<TestObj> q(2);
	CHECK(q.empty());
	CHECK_FALSE(q.full());
	CHECK(q.enqueue(1, "Alice"));
	CHECK(q.enqueue(2, "Bob"));
	CHECK_FALSE(q.enqueue(3, "xf"));
	CHECK(q.full());

	TestObj obj;
	CHECK(q.dequeue(obj));
	CHECK(obj.id == 1);
	CHECK(obj.name == "Alice");
	CHECK(q.dequeue(obj));
	CHECK(obj.id == 2);
	CHECK(obj.name == "Bob");
	CHECK_FALSE(q.dequeue(obj));
	CHECK(q.empty());
}

TEST_CASE("SPSCQueue clear()") {
	cpplearn::SPSCQueue<int> q(3);
	CHECK(q.enqueue(10));
	CHECK(q.enqueue(20));
	CHECK_FALSE(q.empty());

	q.clear();
	CHECK(q.empty());
	int val;
	CHECK_FALSE(q.dequeue(val));
}
