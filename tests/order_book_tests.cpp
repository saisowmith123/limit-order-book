#include "lob/order.hpp"
#include "lob/order_book.hpp"

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

TEST(OrderBookTest, StartsEmpty) {
    lob::OrderBook book;

    EXPECT_EQ(book.order_count(), 0);
    EXPECT_FALSE(book.best_bid().has_value());
    EXPECT_FALSE(book.best_ask().has_value());
}

TEST(OrderBookTest, AddsBuyOrderAndTracksBestBid) {
    lob::OrderBook book;

    const bool added = book.add_limit_order(
        1,
        lob::Side::Buy,
        10050,
        10
    );

    ASSERT_TRUE(added);
    ASSERT_TRUE(book.best_bid().has_value());

    const auto best_bid = book.best_bid().value();

    EXPECT_EQ(best_bid.price, 10050);
    EXPECT_EQ(best_bid.total_quantity, 10);
    EXPECT_EQ(best_bid.order_count, 1);
    EXPECT_EQ(book.order_count(), 1);
}

TEST(OrderBookTest, HighestBuyPriceBecomesBestBid) {
    lob::OrderBook book;

    book.add_limit_order(1, lob::Side::Buy, 10000, 10);
    book.add_limit_order(2, lob::Side::Buy, 10100, 5);
    book.add_limit_order(3, lob::Side::Buy, 9950, 20);

    ASSERT_TRUE(book.best_bid().has_value());

    const auto best_bid = book.best_bid().value();

    EXPECT_EQ(best_bid.price, 10100);
    EXPECT_EQ(best_bid.total_quantity, 5);
}

TEST(OrderBookTest, LowestSellPriceBecomesBestAsk) {
    lob::OrderBook book;

    book.add_limit_order(1, lob::Side::Sell, 10500, 10);
    book.add_limit_order(2, lob::Side::Sell, 10300, 5);
    book.add_limit_order(3, lob::Side::Sell, 10600, 20);

    ASSERT_TRUE(book.best_ask().has_value());

    const auto best_ask = book.best_ask().value();

    EXPECT_EQ(best_ask.price, 10300);
    EXPECT_EQ(best_ask.total_quantity, 5);
}

TEST(OrderBookTest, AggregatesQuantityAtSamePrice) {
    lob::OrderBook book;

    book.add_limit_order(1, lob::Side::Buy, 10050, 10);
    book.add_limit_order(2, lob::Side::Buy, 10050, 15);

    ASSERT_TRUE(book.best_bid().has_value());

    const auto best_bid = book.best_bid().value();

    EXPECT_EQ(best_bid.price, 10050);
    EXPECT_EQ(best_bid.total_quantity, 25);
    EXPECT_EQ(best_bid.order_count, 2);
    EXPECT_EQ(book.quantity_at_price(lob::Side::Buy, 10050), 25);
}

TEST(OrderBookTest, RejectsDuplicateOrderId) {
    lob::OrderBook book;

    EXPECT_TRUE(book.add_limit_order(1, lob::Side::Buy, 10050, 10));
    EXPECT_FALSE(book.add_limit_order(1, lob::Side::Sell, 10100, 5));

    EXPECT_EQ(book.order_count(), 1);
}

TEST(OrderBookTest, RejectsInvalidLimitOrderInput) {
    lob::OrderBook book;

    EXPECT_FALSE(book.add_limit_order(0, lob::Side::Buy, 10050, 10));
    EXPECT_FALSE(book.add_limit_order(1, lob::Side::Buy, 0, 10));
    EXPECT_FALSE(book.add_limit_order(2, lob::Side::Buy, 10050, 0));

    EXPECT_EQ(book.order_count(), 0);
}