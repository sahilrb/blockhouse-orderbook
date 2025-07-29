#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

struct Order {
    double price;
    int size;
    char side; // 'B' or 'A'
};

class OrderBook {
public:
    void add_order(long order_id, double price, int size, char side);
    void cancel_order(long order_id);
    void handle_trade(double price, int size, char side);
    std::string snapshot(const std::string& timestamp);

private:
    std::map<double, int, std::greater<double>> bids; // price -> size
    std::map<double, int> asks;
    std::unordered_map<long, Order> order_map;
    void update_book(char side, double price, int size);
};
