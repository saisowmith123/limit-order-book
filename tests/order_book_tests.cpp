#include "lob/order.hpp"

#include <gtest/gtest.h>

TEST(OrderTest, CreatesValidLimitOrder) {
    lob::Order order(
        1,
        lob::Side::Buy,
        lob::OrderType::Limit,
        10050,
        10,
        1
    );

    EXPECT_EQ(order.id(), 1);
    EXPECT_EQ(order.price(), 10050);
    EXPECT_EQ(order.remaining_quantity(), 10);
    EXPECT_EQ(order.status(), lob::OrderStatus::New);
}

TEST(OrderTest, SupportsPartialFill) {
    lob::Order order(
        1,
        lob::Side::Buy,
        lob::OrderType::Limit,
        10050,
        10,
        1
    );

    order.fill(4);

    EXPECT_EQ(order.remaining_quantity(), 6);
    EXPECT_EQ(order.status(), lob::OrderStatus::PartiallyFilled);
}

TEST(OrderTest, SupportsFullFill) {
    lob::Order order(
        1,
        lob::Side::Sell,
        lob::OrderType::Limit,
        10075,
        10,
        1
    );

    order.fill(10);

    EXPECT_EQ(order.remaining_quantity(), 0);
    EXPECT_TRUE(order.is_filled());
    EXPECT_EQ(order.status(), lob::OrderStatus::Filled);
}

TEST(OrderTest, RejectsZeroQuantity) {
    EXPECT_THROW(
        lob::Order(
            1,
            lob::Side::Buy,
            lob::OrderType::Limit,
            10050,
            0,
            1
        ),
        std::invalid_argument
    );
}