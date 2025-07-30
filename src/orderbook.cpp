#include "orderbook.hpp"
#include <iomanip>
#include <sstream>

void OrderBook::add_order(long order_id, double price, int size, char side) {
    order_map[order_id] = {price, size, side};
    update_book(side, price, size);
}

void OrderBook::cancel_order(long order_id) {
    if (order_map.find(order_id) == order_map.end()) return;
    Order o = order_map[order_id];
    update_book(o.side, o.price, -o.size);
    order_map.erase(order_id);
}

void OrderBook::handle_trade(double price, int size, char side) {
    char opposite = (side == 'B') ? 'A' : 'B';
    update_book(opposite, price, -size);
}

void OrderBook::update_book(char side, double price, int size) {
    if (side == 'B') {
        bids[price] += size;
        if (bids[price] <= 0) bids.erase(price);
    } else {
        asks[price] += size;
        if (asks[price] <= 0) asks.erase(price);
    }
}

std::string OrderBook::snapshot_mbp10() {
    std::ostringstream out;

    int count = 0;
    for (const auto& it : bids) {
        out << "," << it.first << "," << it.second << ",1";
        if (++count == 10) break;
    }
    for (; count < 10; ++count) out << ",0,0,0";

    count = 0;
    for (const auto& it : asks) {
        out << "," << it.first << "," << it.second << ",1";
        if (++count == 10) break;
    }
    for (; count < 10; ++count) out << ",0,0,0";

    return out.str();
}
