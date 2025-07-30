#include "orderbook.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Parse CSV line into fields, supporting quoted commas
std::vector<std::string> parse_csv_row(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool in_quotes = false;

    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./reconstruction_blockhouse <mbo.csv>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    std::ofstream out("mbp_output.csv");
    if (!out.is_open()) {
        std::cerr << "Error opening output file.\n";
        return 1;
    }

    std::string line;
    std::getline(file, line); // read header

    // Write original mbo headers
    out << line;

    // Append MBP-10 headers
    for (int i = 0; i < 10; ++i)
        out << ",bid_px_" << (i < 10 ? "0" : "") << i
            << ",bid_sz_" << (i < 10 ? "0" : "") << i
            << ",bid_ct_" << (i < 10 ? "0" : "") << i;

    for (int i = 0; i < 10; ++i)
        out << ",ask_px_" << (i < 10 ? "0" : "") << i
            << ",ask_sz_" << (i < 10 ? "0" : "") << i
            << ",ask_ct_" << (i < 10 ? "0" : "") << i;

    out << ",symbol,order_id\n";

    OrderBook book;

    while (std::getline(file, line)) {
        auto row = parse_csv_row(line);
        if (row.size() < 11) continue;

        std::string ts = row[1];
        std::string action = row[5];
        char side = row[6].empty() ? 'N' : row[6][0];
        double price = row[7].empty() ? 0 : std::stod(row[7]);
        int size = row[8].empty() ? 0 : std::stoi(row[8]);
        long order_id = row[10].empty() ? 0 : std::stol(row[10]);

        if (action == "A") {
            book.add_order(order_id, price, size, side);
        } else if (action == "C") {
            book.cancel_order(order_id);
        } else if (action == "T") {
            if (side == 'N') continue;
            book.handle_trade(price, size, side);
        }

        out << line << "," << book.snapshot_mbp10() << "," << row[3] << "," << order_id << "\n";
    }

    return 0;
}
