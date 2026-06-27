#include "lob/order_book.hpp"
#include <iterator>

namespace lob{
    bool OrderBook::add_limit_order(
        OrderId id,
        Side side,
        Price price,
        Quantity quantity
    ) {
        if(id==0 || price <= 0 || quantity <= 0){
            return false;
        };
        if(order_index_.contains(id)){
            return false;
        }
        auto add_to_levels = [&](auto& levels) {
            auto [level_it, inserted] = levels.try_emplace(
                price,
                PriceLevel{
                    price,
                    std::list<Order>{},
                    0
                }
            );
            // the above will emplace the order into new price level, if the price level didnt exist already
            // returns level_it which is pointer to the price level, and inserted is true if price level is newly created or else it is false if already exists

            auto& level = level_it->second; // first is key and second is value <Price, PriceLevel>
            level.orders.emplace_back(
                id,
                side, 
                OrderType::Limit,
                price,
                quantity,
                next_sequence_number_++
            ); // emplace back constructs Order directly into the list, it is cleaner and avoids extra temporary data

            auto order_it = std::prev(level.orders.end()); // gets the pointer of previous order
            level.total_quantity += quantity;
            order_index_.emplace(
                id,
                OrderLocation{
                    side,
                    price,
                    order_it
                }
            ); // faster lookup thing we save the pointer to the inserted order
            return true;
        };
        if (side == Side::Buy){
           return add_to_levels(bids_);
        } else{
           return add_to_levels(asks_);
        }
    }

    // best bid and best ask
    std::optional<OrderBook::LevelInfo> OrderBook::best_bid() const{
        if(bids_.empty()){
            return std::nullopt;
        }

        const auto& [price, level] = *bids_.begin();
        return LevelInfo{
            price,
            level.total_quantity,
            level.orders.size()
        };
    }

    std::optional<OrderBook::LevelInfo> OrderBook::best_ask() const{
        if(asks_.empty()){
            return std::nullopt;
        }

        const auto& [price, level] = *asks_.begin();
        return LevelInfo{
            price,
            level.total_quantity,
            level.orders.size()
        };
    }

    std::size_t OrderBook::order_count() const noexcept{
        return order_index_.size();
    }

    std::size_t OrderBook::price_level_count(Side side) const noexcept{
        if(side == Side::Buy){
            return bids_.size();
        }return asks_.size();
    }

    bool OrderBook::contains_order(OrderId order_id) const{
        return order_index_.contains(order_id);
    }

    Quantity OrderBook::quantity_at_price(Side side, Price price) const{
        if(side == Side::Buy){
            auto it = bids_.find(price);
            if(it == bids_.end()){
                return 0;
            }
            return it->second.total_quantity;
        }

        auto it = asks_.find(price);
        if(it == asks_.end()){
            return 0;
        }
        return it->second.total_quantity;
    }
}