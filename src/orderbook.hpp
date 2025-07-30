#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <string>
#include <sstream>
#include <functional>

class OrderBook {
public:
    void add_order(long order_id, double price, int size, char side);
    void cancel_order(long order_id);
    void handle_trade(double price, int size, char side);
    std::string snapshot_mbp10();

private:
    struct Order {
        double price;
        int size;
        char side;
    };

    std::map<long, Order> order_map;
    std::map<double, int, std::greater<>> bids;
    std::map<double, int> asks;

    void update_book(char side, double price, int size);
};

#endif // ORDERBOOK_HPP
