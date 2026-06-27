#pragma once
#include "order.hpp"
#include "types.hpp"

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <unordered_map>

namespace lob{
    class OrderBook{
        public:
            struct LevelInfo {
                Price price;
                Quantity total_quantity;
                std::size_t order_count;
            };
        bool add_limit_order(
            OrderId order_id,
            Side side,
            Price price,
            Quantity quantity
        );

        [[nodiscard]] std::optional<LevelInfo> best_bid() const; // someone is ready to pay this amout to buy an order
        [[nodiscard]] std::optional<LevelInfo> best_ask() const;

        [[nodiscard]] std::size_t order_count() const noexcept;
        [[nodiscard]] std::size_t price_level_count(Side side) const noexcept; // total number of price level counts(e.g 100,99,101 returns 3)

        [[nodiscard]] bool contains_order(OrderId order_id) const;
        [[nodiscard]] Quantity quantity_at_price(Side side, Price price) const; // returns total quantity available at a specific price

        private:
            struct PriceLevel{
                Price price;
                std::list<Order> orders;
                Quantity total_quantity;
            }; // stores all the orders which are at same price

            // created when creating an order
            struct OrderLocation{
                Side side;
                Price price;
                std::list<Order>::iterator order_it; // reference of a particular order in the list
            };

            using BidLevels = std::map<Price, PriceLevel, std::greater<Price>>;
            using AskLevels = std::map<Price, PriceLevel, std::less<Price>>;

            BidLevels bids_;
            AskLevels asks_;
            // also created when order is created
            std::unordered_map<OrderId, OrderLocation> order_index_; // maps orderid to exact location in the book
            // order_index_ gives average O(1) lookup by order id.                
            // bids_/asks_ are std::map, so price-level lookup is O(log P).
            // list erase is O(1) when we already have the iterator.

            SequenceNumber next_sequence_number_ = 1;
    };
}
