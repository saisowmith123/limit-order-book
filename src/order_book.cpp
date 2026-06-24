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
            add_to_levels(bids_);
        } else{
            add_to_levels(asks_);
        }
    }
}