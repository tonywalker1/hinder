#pragma once

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
#include <chrono>
#include <condition_variable>
#include <hinder/exception/exception.h>
#include <limits>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

namespace hinder {

    // Exception type for queue errors (e.g., invalid construction arguments).
    HINDER_DEFINE_EXCEPTION(queue_error, generic_error);

    // Concept: Container must have the front/pop/empty/size interface provided by std::queue<T>.
    // std::stack and std::priority_queue use top() instead of front() and do not satisfy this
    // concept; use a dedicated type for those semantics.
    template <typename C>
    concept queue_container = requires(C con) {
        con.front();
        con.pop();
        con.empty();
        con.size();
    };

    // Generic bounded, thread-safe queue supporting multiple producers/consumers.
    //
    // Capabilities:
    // - Store any move-constructible type T
    // - Use any container satisfying queue_container (default: std::queue<T>)
    // - Bounded capacity (default: std::numeric_limits<std::size_t>::max())
    // - push() blocks until space is available or times out; returns false on timeout or close
    // - pop() blocks until an item is available or times out; returns nullopt on timeout
    // - After close(): push() returns false immediately; pop() drains remaining items then nullopt
    //
    // Invariants:
    // - Bound is positive and constant after construction
    // - Queue size is always >= 0 and <= bound
    //
    // Thread safety: all public operations are thread-safe.
    template <typename T, queue_container Container = std::queue<T>>
    class queue {
    public:
        // Use steady_clock to avoid time-travel if the system clock is adjusted.
        using clock_t    = std::chrono::steady_clock;
        using duration_t = clock_t::duration;

        // Default timeout for push() and pop(): 60 seconds.
        static constexpr duration_t default_timeout = std::chrono::seconds(60);

        // Construct an instance with the given upper bound on queue size.
        // Precondition: bound > 0
        explicit queue(std::size_t bound = std::numeric_limits<std::size_t>::max())
        : m_bound {bound}
        {
            HINDER_EXPECTS(bound > 0, queue_error);
        }

        ~queue()                                = default;
        queue(const queue&)                     = delete;
        auto operator=(const queue&) -> queue&  = delete;
        queue(queue&&)                          = delete;
        auto operator=(queue&&) -> queue&       = delete;

        // Returns true if close() has been called.
        [[nodiscard]] auto closed() const noexcept -> bool { return m_closed.load(); }

        // Close the queue.
        // After close(): push() returns false immediately; pop() drains any remaining items then
        // returns nullopt. All blocked producers and consumers are woken.
        // Idempotent: calling close() on an already-closed queue is harmless.
        auto close() -> void
        {
            {
                std::unique_lock<std::mutex> lck(m_highlander);
                m_closed.store(true);
            }
            m_not_full.notify_all();
            m_not_empty.notify_all();
        }

        // Lock-free atomic snapshot of the current queue depth.
        // Accurate for monitoring and profiling; may lag one step behind concurrent push/pop.
        [[nodiscard]] auto size() const noexcept -> std::size_t { return m_size.load(); }

        // Lock-free atomic snapshot of whether the queue is empty.
        [[nodiscard]] auto empty() const noexcept -> bool { return m_size.load() == 0; }

        // Push a value onto the queue.
        // Blocks until space is available or the timeout elapses.
        // Returns true on success; false on timeout or if the queue is closed.
        template <typename V>
        [[nodiscard]] auto push(V&& val, duration_t timeout = default_timeout) -> bool
        {
            std::unique_lock<std::mutex> lck(m_highlander);
            if (m_closed.load()) {
                return false;
            }
            if (!m_not_full.wait_for(lck, timeout,
                    [this]() -> bool { return m_closed.load() || m_queue.size() < m_bound; })) {
                return false;
            }
            if (m_closed.load()) {
                return false;
            }
            m_queue.push(std::forward<V>(val));
            m_size.fetch_add(1);
            m_not_empty.notify_one();
            return true;
        }

        // Pop a value from the queue via move.
        // Blocks until an item is available or the timeout elapses.
        // Returns the item on success; nullopt on timeout or if the queue is closed and empty.
        [[nodiscard]] auto pop(duration_t timeout = default_timeout) -> std::optional<T>
        {
            std::unique_lock<std::mutex> lck(m_highlander);
            if (!m_not_empty.wait_for(lck, timeout,
                    [this]() -> bool { return m_closed.load() || !m_queue.empty(); })) {
                return std::nullopt;
            }
            if (m_queue.empty()) {
                return std::nullopt;    // closed with empty queue
            }
            auto val = std::move(m_queue.front());
            m_queue.pop();
            m_size.fetch_sub(1);
            m_not_full.notify_one();
            return std::make_optional(std::move(val));
        }

    private:
        Container                m_queue {};
        const std::size_t        m_bound;
        std::atomic<std::size_t> m_size {0};
        std::atomic<bool>        m_closed {false};
        std::mutex               m_highlander;    // there can be only one
        std::condition_variable  m_not_empty;     // the queue is no longer empty
        std::condition_variable  m_not_full;      // the queue is no longer full
    };

}  // namespace hinder
