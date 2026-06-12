# Order Book

## Overview

A low-latency limit order book implemented in C++23. The focus is not just the
book itself but the entire path data travels — from a raw socket through a
lock-free handoff into the matching engine. Every design decision is driven by
one question: where does time get lost, and why?

## Project Goals

### What I am trying to accomplish

- Understand and implement the full pipeline from network packet to processed
  book update
- Apply systems knowledge (OS, networking, distributed systems) to a real domain
  problem with measurable outcomes
- Build something I can instrument, profile, and reason about at every layer

### What this implementation focuses on

- The latency path: kernel buffer → userspace → SPSC queue → order book
- Non-blocking I/O with epoll and correct partial read handling
- Lock-free coordination between the network thread and processing thread
- Memory allocation patterns under a high-throughput order stream
- Benchmarks with real numbers — P50/P99/P999 latency at each handoff point

### What it's NOT trying to do

- Production exchange infrastructure
- FIX protocol compliance
- Distributed consensus or replicated state
- A complete trading system

## Architecture

### How Data Flows

```
NIC → Kernel Buffer → epoll → recv() → Feed Handler
         → SPSC Ring Buffer
              → Processing Thread → Order Book → Matching Engine
```

Each arrow is a handoff point. Each handoff point is instrumented.

### Threads

- **Network Thread** — runs epoll loop, reads from socket, handles partial
  reads, writes into SPSC queue
- **Processing Thread** — reads from SPSC queue, updates order book, runs
  matching engine

Two threads. One queue between them. No shared state beyond that queue.

### Key Components

- `FeedHandler` — epoll-based non-blocking socket reader
- `SPSCQueue` — lock-free single-producer single-consumer ring buffer
- `OrderBook` — price level structure with bid/ask maps
- `MatchingEngine` — price-time priority matching
- `ArenaAllocator` — slab allocator for Order objects (profiler-justified)

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
./orderbook
```

## Project Phases

1. **Phase 1: Foundation** — project structure, dummy data source, basic Order
   structs, CMake setup
2. **Phase 2: Order Book Core** — price level data structure, bid/ask maps,
   add/cancel operations
3. **Phase 3: Matching Engine** — price-time priority matching, partial fills,
   MatchResult
4. **Phase 4: Threading Model** — SPSC ring buffer, network thread and
   processing thread, clean shutdown
5. **Phase 5: Feed Handler** — epoll-based socket reader, non-blocking I/O,
   partial read handling
6. **Phase 6: Arena Allocator** — profile first, build second, benchmark
   before/after
7. **Phase 7: Benchmarking** — Google Benchmark integration, P50/P99/P999
   latency, flame graphs
8. **Phase 8: Real Feed** — swap dummy data for real exchange WebSocket/UDP feed

See `docs/` for detailed phase documentation.

## Tech Stack

- C++23
- CMake 3.20+
- epoll (Linux)
- std::atomic with explicit memory ordering
- Google Benchmark (Phase 7+)

## Requirements

- GCC 13+ or Clang 17+
- CMake 3.20+
- Linux (epoll is Linux-specific; dummy feed works on macOS/Windows)
