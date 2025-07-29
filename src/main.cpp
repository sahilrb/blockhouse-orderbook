#include "orderbook.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> parse_csv_row(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (getline(ss, field, ',')) fields.push_back(field);
    return fields;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./reconstruction_blockhouse mbo.csv\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    std::ofstream out("mbp_output.csv");
    std::string line;

    getline(file, line); // header
    out << "timestamp";
    for (int i = 0; i < 10; ++i) out << ",bid_px_" << i << ",bid_sz_" << i;
    for (int i = 0; i < 10; ++i) out << ",ask_px_" << i << ",ask_sz_" << i;
    out << "\n";

    getline(file, line); // skip R row

    OrderBook book;

    while (getline(file, line)) {
        auto row = parse_csv_row(line);
        std::string ts = row[2];
        std::string action = row[5];
        char side = row[6][0];
        double price = row[7].empty() ? 0 : std::stod(row[7]);
        int size = std::stoi(row[8]);
        long order_id = std::stol(row[10]);

        if (action == "A") {
            book.add_order(order_id, price, size, side);
        } else if (action == "C") {
            book.cancel_order(order_id);
        } else if (action == "T") {
            if (side == 'N') continue;
            book.handle_trade(price, size, side);
        }

        out << book.snapshot(ts) << "\n";
    }

    return 0;
}