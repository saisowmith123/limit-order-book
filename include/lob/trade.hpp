#pragma once
#include "types.hpp"

namespace lob {
    struct trade
    {
        /* data */
        OrderId aggressive_order_id; //new buy order id
        OrderId resting_order_id; //already existing order id
        Price price;
        Quantity quantity;
        SequenceNumber sequenceNumber;
    };
    
}