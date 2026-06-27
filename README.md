# Low-Latency Limit Order Book & Matching Engine

A C++20 limit order book and matching engine project built to explore systems programming, low-latency design, data structures, order matching logic, unit testing, benchmarking, and Linux-friendly debugging/profiling tools.

> **Status:** This project is under active development. Core order models, trade types, resting limit order book storage, price level queries, and unit tests have been implemented. Matching logic, cancel/modify support, benchmarks, and profiling are being built incrementally.

## Project Goal

The goal of this project is to implement a clean, interview-defensible limit order book and matching engine similar to the core component used by electronic trading systems.

The current scope focuses on **limit orders only**. Market order handling is intentionally out of scope for this version.

The engine is designed to support:

* Limit orders
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
* GDB/LLDB
* perf for Linux profiling

## Architecture

```text
      Incoming Limit Order
              |
              v
      Matching Engine
              |
              v
           Order Book
        |              |
      Bid Book       Ask Book
        |              |
Price Levels with FIFO order queues
```

## Core Design Ideas

The project uses integer-based prices and quantities to avoid floating-point precision issues in financial calculations.

The order book design uses:

* `std::map` for sorted bid and ask price levels
* `std::list` for FIFO order queues within each price level
* `std::unordered_map` for fast order ID lookup during cancel and modify operations
* Sequence numbers to enforce deterministic time priority

Bid levels are sorted from highest price to lowest price, so the best bid is available at the beginning of the bid book.

Ask levels are sorted from lowest price to highest price, so the best ask is available at the beginning of the ask book.

## Implemented Features

* [x] CMake project setup
* [x] Core type definitions
* [x] Order model
* [x] Trade model
* [x] Basic order validation
* [x] Resting limit order book
* [x] Bid and ask price level storage
* [x] FIFO order queues within each price level
* [x] Best bid lookup
* [x] Best ask lookup
* [x] Order count query
* [x] Price level count query
* [x] Quantity-at-price query
* [x] Fast order ID lookup using `std::unordered_map`
* [x] Initial GoogleTest unit tests

## Features In Progress

* [ ] Price-time priority matching for limit orders
* [ ] Partial fill handling inside the matching engine
* [ ] Trade reporting
* [ ] Cancel order support
* [ ] Modify order support
* [ ] Expanded unit test coverage
* [ ] Benchmark harness
* [ ] Sanitizer build scripts
* [ ] Linux perf profiling
* [ ] Final architecture documentation

## Benchmarking Plan
Planned benchmark scenarios:

* Mostly add limit orders
* Add/cancel mix
* Limit orders around a midpoint
* Randomized prices across multiple price levels
* Crossed limit orders that trigger matching
* Multi-level limit order matching

Benchmark results will be added once the matching engine implementation is complete.

## Complexity Goals

Expected operation complexity:

| Operation                   | Target Complexity                |
| --------------------------- | -------------------------------- |
| Best bid/ask lookup         | O(1)                             |
| Insert new price level      | O(log P)                         |
| Add order to existing level | O(1)                             |
| Quantity at price lookup    | O(log P)                         |
| Order ID lookup             | O(1) average                     |
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
