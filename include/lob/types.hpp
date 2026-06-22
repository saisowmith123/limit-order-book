#pragma once
#include <cstdint>

namespace lob {
    using OrderId = std::uint64_t;
    using Price = std::int64_t;
    using Quantity = std::int64_t;
    using SequenceNumber = std::uint64_t;

    enum class Side {
        Buy, Sell
    };

    enum class OrderType {
        Limit, Market
    };

    enum class OrderStatus {
        New,
        PartiallyFilled,
        Filled,
        Cancelled,
        Rejected
    };
}
