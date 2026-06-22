# Low-Latency Limit Order Book & Matching Engine

A C++20 limit order book and matching engine project built to explore systems programming, low-latency design, data structures, order matching logic, benchmarking, and Linux-friendly debugging/profiling tools.

> **Status:** This project is currently under active development. Core order models, matching logic, unit tests, benchmarks, and performance profiling are being built incrementally.

## Project Goal

The goal of this project is to implement a clean, interview-defensible limit order book and matching engine similar to the core component used by electronic trading systems.

The engine is designed to support:

* Limit orders
* Market orders
* Cancel orders
* Modify orders
* Partial fills
* Price-time priority
* Order status tracking
* Trade generation
* Unit testing
* Latency and throughput benchmarking

## Tech Stack

* C++20
* CMake
* STL containers
* GoogleTest
* Linux/macOS terminal tooling
* AddressSanitizer
* UndefinedBehaviorSanitizer
* GDB/LLDB
* perf for Linux profiling

## Planned Architecture

```text
Incoming Order
      |
      v
Matching Engine
      |
      v
Order Book
  |          |
Bid Book   Ask Book
  |          |
Price Levels with FIFO order queues
```

## Core Design Ideas

The project uses integer-based prices and quantities to avoid floating-point precision issues in financial calculations.

The planned order book design uses:

* `std::map` for sorted bid and ask price levels
* `std::list` for FIFO order queues within each price level
* `std::unordered_map` for fast order ID lookup during cancel and modify operations
* Sequence numbers to enforce deterministic time priority

## Features In Progress

* [x] CMake project setup
* [x] Core order and trade types
* [x] Basic order validation
* [x] Initial unit tests
* [ ] Resting limit order book
* [ ] Price-time priority matching
* [ ] Market order support
* [ ] Cancel order support
* [ ] Modify order support
* [ ] Trade reporting
* [ ] Benchmark harness
* [ ] Sanitizer build scripts
* [ ] Linux perf profiling
* [ ] Final architecture documentation

## Build Instructions

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
```

## Run Tests

```bash
ctest --test-dir build/debug --output-on-failure
```

## Sanitizer Build

```bash
cmake -S . -B build/asan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLOB_ENABLE_SANITIZERS=ON

cmake --build build/asan
ctest --test-dir build/asan --output-on-failure
```

## Benchmarking Plan

The benchmark module will measure:

* Throughput in operations per second
* Average latency
* p95 latency
* p99 latency

Planned benchmark scenarios:

* Mostly add orders
* Add/cancel mix
* Market order sweeps
* Randomized prices around a midpoint
* Multi-level book matching

Benchmark results will be added once the matching engine implementation is complete.

## Complexity Goals

Expected operation complexity:

| Operation                   | Target Complexity                |
| --------------------------- | -------------------------------- |
| Best bid/ask lookup         | O(1)                             |
| Insert new price level      | O(log P)                         |
| Add order to existing level | O(1)                             |
| Cancel by order ID          | O(1) average lookup + O(1) erase |
| Match against best level    | O(number of matched orders)      |

Where `P` is the number of active price levels.

## Future Improvements

* Custom memory pool for order allocation
* Intrusive linked list for better cache behavior
* Flat price ladder for bounded tick ranges
* Binary protocol input simulation
* Replay-based benchmark from generated order streams
* Multi-threaded market data publisher
* Lock-free queue experiments

## Resume Summary

Built a low-latency C++20 limit order book and matching engine supporting limit, market, cancel, and modify orders with price-time priority, partial fills, and performance benchmarks.
