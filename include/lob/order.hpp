#pragma once
#include "types.hpp"
#include <stdexcept>

namespace lob {
    class Order {
        public:
            Order(
                OrderId id,
                Side side,
                OrderType type,
                Price price,
                Quantity quantity,
                SequenceNumber sequenceNumber
            )
                : id_(id), //initializing private members of the class
                side_(side),
                type_(type),
                price_(price),
                original_quantity_(quantity),
                remaining_quantity_(quantity),
                sequence_number_(sequenceNumber),
                status_(OrderStatus::New)
                {
                    if(id_ == 0) {
                        throw std::invalid_argument("order id must be non zero");
                    }
                    if(quantity <= 0){
                        throw std::invalid_argument("quantity must be positive");
                    }
                    if(type_ == OrderType::Limit && price <= 0) {
                        throw std::invalid_argument("limit order price must be positive");
                    }
                }
                // getter functions
                // We use getter to avoid unnecessary modifications to private members
                // nodiscard warns if return is not specified
                // const specifies that function does not modify the object
                // noexcept means function promises not to throw exceptions
                [[nodiscard]] OrderId id() const noexcept {
                    return id_;
                }
                [[nodiscard]] Side side() const noexcept {
                    return side_;
                }
                [[nodiscard]] OrderType type() const noexcept {
                    return type_;
                }
                [[nodiscard]] Price price() const noexcept {
                    return price_;
                }
                [[nodiscard]] Quantity original_quantity() const noexcept {
                    return original_quantity_;
                }
                [[nodiscard]] Quantity remaining_quantity() const noexcept {
                    return remaining_quantity_;
                }
                [[nodiscard]] SequenceNumber sequence_number() const noexcept {
                    return sequence_number_;
                }
                [[nodiscard]] OrderStatus status() const noexcept {
                    return status_;
                }
                [[nodiscard]] bool is_filled() const noexcept {
                    return remaining_quantity_ == 0;
                }

                void fill(Quantity executed_quantity) {
                    if(executed_quantity <= 0){
                        throw std::invalid_argument("executed quantity must be positive");
                    }

                    if (executed_quantity > remaining_quantity_){
                        throw std::invalid_argument("executed quantity exceeds remaining quantity");
                    }

                    remaining_quantity_ -= executed_quantity;

                    if(remaining_quantity_ == 0){
                        status_ = OrderStatus::Filled;
                    } else {
                        status_ = OrderStatus::PartiallyFilled;
                    }
                }

                void cancel() noexcept {
                    status_ = OrderStatus::Cancelled;
                }
        private:
            OrderId id_;
            Side side_;
            OrderType type_;
            Price price_;
            Quantity original_quantity_;
            Quantity remaining_quantity_;
            SequenceNumber sequence_number_;
            OrderStatus status_;
    };
}