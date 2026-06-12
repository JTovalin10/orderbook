# Phase 1: Foundation

## Goal

Stand up the project skeleton and get a stream of fake orders flowing through a
callback so every subsequent phase has something to build against. Nothing real
happens here — no matching, no book, no network. The only question this phase
answers is: **what does an order look like, and how does it move from a source
into the rest of the system?**

## Why

### Why start with dummy data instead of a real feed?

A real exchange feed introduces authentication, protocol parsing, network
errors, and rate limits before you've written a single line of order book logic.
Dummy data lets you define the interface your system expects and develop every
downstream phase independently of network availability.

### Why a callback?

The feed shouldn't know what consumes it. A callback means you can swap dummy
data for a real socket feed in Phase 8 without touching the order book code.
This is the seam you're building toward.

## Essential Structs

```cpp
enum class Side { BUY, SELL };
enum class Type { LIMIT, MARKET };

struct Order {
    uint64_t order_id;
    Side     side;
    Type     type;
    double   price;
    uint64_t quantity;
    uint64_t timestamp_ns;  // nanoseconds since epoch
};
```

```cpp
using OrderCallback = std::function<void(const Order&)>;
```

```cpp
class DataSource {
public:
    virtual ~DataSource()                         = default;
    virtual void start()                          = 0;
    virtual void stop()                           = 0;
    virtual void register_callback(OrderCallback) = 0;
};
```

## What to Research

- **Fixed-point vs floating-point for price** — why exchanges represent prices
  as integers (ticks, cents) and what you lose with `double`. Understand it now
  even if you don't change it yet.
- **`uint64_t` for timestamps** — how to get nanosecond resolution in C++.
  Difference between `std::chrono::high_resolution_clock` and
  `clock_gettime(CLOCK_MONOTONIC)` and when each is appropriate.
- **`std::thread` basics** — launch, join vs detach, and why detach is usually
  the wrong choice even when convenient.
- **`std::function` overhead** — it heap-allocates for non-trivial callables.
  Fine here. Know why you'd replace it in a hot path.

## My Design Decisions

> Fill this out as you build. For each decision write: what you chose, what the
> alternatives were, and why you picked this one.

- **Price representation:**

- **Timestamp source:**

- **DummyDataSource emit interval:**

- **Other:**
