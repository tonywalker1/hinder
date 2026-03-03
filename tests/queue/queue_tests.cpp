//
// hinder::queue
//
// MIT License
//
// Copyright (c) 2017-2026  Tony Walker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <atomic>
#include <gtest/gtest.h>
#include <hinder/queue.h>
#include <memory>
#include <queue>
#include <stack>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

// ============================================================================
// Compile-time concept checks
// ============================================================================

static_assert(hinder::queue_container<std::queue<int>>,
    "std::queue<int> must satisfy queue_container");
static_assert(!hinder::queue_container<std::stack<int>>,
    "std::stack<int> must not satisfy queue_container (uses top(), not front())");
static_assert(!hinder::queue_container<std::priority_queue<int>>,
    "std::priority_queue<int> must not satisfy queue_container (uses top(), not front())");

// ============================================================================
// Basic single-threaded round-trip
// ============================================================================

TEST(Queue, PushPop)
{
    hinder::queue<int> que;
    EXPECT_TRUE(que.push(42));
    const auto result = que.pop(100ms);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 42);
}

// ============================================================================
// Bounded queue: push to capacity, then time out
// ============================================================================

TEST(Queue, Bound)
{
    hinder::queue<int> que(2);
    EXPECT_EQ(que.size(), 0);
    EXPECT_TRUE(que.empty());

    EXPECT_TRUE(que.push(1));
    EXPECT_TRUE(que.push(2));
    EXPECT_EQ(que.size(), 2);
    EXPECT_FALSE(que.empty());

    // Queue is full; next push should time out and return false.
    EXPECT_FALSE(que.push(3, 10ms));
}

// ============================================================================
// Timeout: pop from empty queue returns nullopt
// ============================================================================

TEST(Queue, Timeout)
{
    hinder::queue<int> que;
    const auto result = que.pop(10ms);
    EXPECT_FALSE(result.has_value());
}

// ============================================================================
// Move semantics: push and pop a move-only type
// ============================================================================

TEST(Queue, MoveSemantics)
{
    hinder::queue<std::unique_ptr<int>> que;

    auto ptr = std::make_unique<int>(99);
    EXPECT_TRUE(que.push(std::move(ptr)));
    EXPECT_EQ(ptr, nullptr);  // ownership transferred

    const auto result = que.pop(100ms);
    ASSERT_TRUE(result.has_value());
    ASSERT_NE(result->get(), nullptr);
    EXPECT_EQ(**result, 99);
}

// ============================================================================
// close(): blocks new pushes, drains existing items, then returns nullopt
// ============================================================================

TEST(Queue, CloseBlocksPush)
{
    hinder::queue<int> que;
    EXPECT_FALSE(que.closed());
    que.close();
    EXPECT_TRUE(que.closed());
    EXPECT_FALSE(que.push(1));
}

TEST(Queue, CloseDrains)
{
    hinder::queue<int> que;
    EXPECT_TRUE(que.push(1));
    EXPECT_TRUE(que.push(2));
    que.close();

    // Existing items are still retrievable after close.
    const auto val1 = que.pop();
    ASSERT_TRUE(val1.has_value());
    EXPECT_EQ(*val1, 1);

    const auto val2 = que.pop();
    ASSERT_TRUE(val2.has_value());
    EXPECT_EQ(*val2, 2);

    // Queue is now empty and closed: nullopt.
    EXPECT_FALSE(que.pop().has_value());
}

TEST(Queue, CloseIsIdempotent)
{
    hinder::queue<int> que;
    que.close();
    que.close();    // second close should not throw or hang
    EXPECT_TRUE(que.closed());
}

// ============================================================================
// MPMC stress test: N producers and M consumers
// ============================================================================

TEST(Queue, MPMC)
{
    constexpr int num_producers      = 4;
    constexpr int num_consumers      = 4;
    constexpr int items_per_producer = 100;
    constexpr int total              = num_producers * items_per_producer;

    hinder::queue<int> que(50);
    std::atomic<int>   consumed {0};

    // Consumers loop until pop() returns nullopt (closed + drained).
    std::vector<std::thread> consumers;
    consumers.reserve(num_consumers);
    for (int idx = 0; idx < num_consumers; ++idx) {
        consumers.emplace_back([&] {
            while (true) {
                const auto val = que.pop();
                if (!val) {
                    break;
                }
                consumed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    std::vector<std::thread> producers;
    producers.reserve(num_producers);
    for (int idx = 0; idx < num_producers; ++idx) {
        producers.emplace_back([&] {
            for (int jdx = 0; jdx < items_per_producer; ++jdx) {
                EXPECT_TRUE(que.push(jdx));
            }
        });
    }

    for (auto& thd : producers) {
        thd.join();
    }
    // All items are now in the queue or already consumed; signal consumers to drain and exit.
    que.close();
    for (auto& thd : consumers) {
        thd.join();
    }

    EXPECT_EQ(consumed.load(), total);
}

// ============================================================================
// Precondition: bound must be > 0
// ============================================================================

TEST(Queue, ZeroBoundThrows)
{
    EXPECT_THROW(hinder::queue<int>(0), hinder::queue_error);
}
