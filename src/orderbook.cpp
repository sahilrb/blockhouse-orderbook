#include "orderbook.hpp"
#include <iomanip>

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
    std::map<double, int>* book = (side == 'B') ? (std::map<double, int>*)&bids : &asks;
    (*book)[price] += size;
    if ((*book)[price] <= 0) {
        book->erase(price);
    }
}


std::string OrderBook::snapshot(const std::string& timestamp) {
    std::ostringstream out;
    out << timestamp;

    int count = 0;
    for (auto& it : bids) {
        double px = it.first;
        int sz = it.second;
        out << "," << px << "," << sz;
        if (++count == 10) break;
    }
    for (; count < 10; ++count) out << ",0,0";

    count = 0;
    for (auto& it : asks) {
        double px = it.first;
        int sz = it.second;
        out << "," << px << "," << sz;
        if (++count == 10) break;
    }
    for (; count < 10; ++count) out << ",0,0";

    return out.str();
}
