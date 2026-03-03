# hinder::queue

Generic bounded, thread-safe queue supporting multiple producers and consumers (MPMC).

## Capabilities

- Store any move-constructible type `T`
- Use any container with `front`/`push`/`pop`/`size`/`empty` semantics (default: `std::queue<T>`)
- Bounded capacity (default: `std::numeric_limits<std::size_t>::max()`, effectively unbounded)
- `push()` blocks until space is available or a timeout elapses; returns `false` on timeout
- `pop()` blocks until an item is available or a timeout elapses; returns `std::nullopt` on timeout

## Invariants

- Bound is positive and constant after construction
- Queue size is always `>= 0` and `<= bound`
- All blocking operations respect the specified timeout duration

## Usage

### Basic push/pop

```c++
#include <hinder/queue.h>

hinder::queue<int> que;

if (que.push(42)) {
    auto val = que.pop();   // blocks up to 60s by default
    if (val) {
        // *val == 42
    }
}
```

### Custom timeout

```c++
using namespace std::chrono_literals;

hinder::queue<std::string> que;

// Push with a 5-second timeout
if (!que.push("hello", 5s)) {
    // queue was full, value not pushed
}

// Pop with a 500ms timeout
auto val = que.pop(500ms);
if (!val) {
    // timeout: nothing was available
}
```

### Bounded queue

Limit the queue to a fixed number of items. Producers block when full; consumers block when empty.

```c++
hinder::queue<int> que(100);    // capacity: 100 items

// Producers block if the queue is at capacity.
que.push(item);

// Consumers block if the queue is empty.
auto val = que.pop();
```

### MPMC pattern

The queue is safe to use from any number of producer and consumer threads simultaneously.

```c++
hinder::queue<Task> task_queue(200);

// Producer threads
for (auto& producer : producers) {
    producer = std::thread([&] {
        while (auto task = get_next_task()) {
            task_queue.push(*task);
        }
    });
}

// Consumer threads
for (auto& consumer : consumers) {
    consumer = std::thread([&] {
        while (!shutdown.load()) {
            auto task = task_queue.pop(100ms);
            if (task) {
                process(*task);
            }
        }
    });
}
```

### Move-only types

`push()` uses perfect forwarding, so move-only types (e.g., `std::unique_ptr`) work naturally.

```c++
hinder::queue<std::unique_ptr<Widget>> que;

auto widget = std::make_unique<Widget>(args...);
que.push(std::move(widget));    // widget is now null

auto result = que.pop();
if (result) {
    result->use();
}
```

## Design Notes

### Why `std::optional<T>` for `pop()`

The original design used an output parameter (`bool pop(T& val)`), which forces callers to
pre-construct a `T` even on timeout paths, and makes the success/failure state implicit.
`std::optional<T>` makes the "might not have a value" contract explicit in the type, works
naturally with move-only types, and eliminates the pre-construction overhead.

### Why `steady_clock`

`push()` and `pop()` timeouts use `std::chrono::steady_clock` (via `duration_t`). A
monotonically increasing clock is essential for blocking operations: if the system wall clock
jumps backward (e.g., NTP correction, DST), a `system_clock`-based timeout could block
indefinitely or return immediately. `steady_clock` is immune to such adjustments.

### Thread safety of `size()` and `empty()`

`size()` and `empty()` are intentionally unlocked. Taking a lock inside these methods gives
a false sense of safety: the size can change between the call and any subsequent action.
These methods are useful for testing and approximate monitoring only.
