#include "orderbook.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> parse_csv_row(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    bool inside_quotes = false;
    std::string temp;

    for (char c : line) {
        if (c == '"') {
            inside_quotes = !inside_quotes; // Toggle the inside_quotes flag
        } else if (c == ',' && !inside_quotes) {
            fields.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    fields.push_back(temp); // Add the last field
    return fields;
}

void test_parse_csv_row() {
    std::cout << "Running tests for parse_csv_row...\n";

    // Test case 1: Normal CSV row
    std::string line1 = "2025-07-17T08:05:03.360842448Z,2025-07-17T08:05:03.360677248Z,160,2,1108,A,B,5.51,100,0,817593,130,165200,851012,ARL";
    auto fields1 = parse_csv_row(line1);
    std::cout << "Test 1: ";
    for (const auto& field : fields1) std::cout << field << " | ";
    std::cout << "\n";

    // Test case 2: Quoted field with commas
    std::string line2 = "2025-07-17T08:05:03.360842448Z,2025-07-17T08:05:03.360677248Z,160,2,1108,\"A,B\",5.51,100,0,817593,130,165200,851012,ARL";
    auto fields2 = parse_csv_row(line2);
    std::cout << "Test 2: ";
    for (const auto& field : fields2) std::cout << field << " | ";
    std::cout << "\n";

    // Test case 3: Empty field
    std::string line3 = "2025-07-17T08:05:03.360842448Z,,160,2,1108,A,B,5.51,100,0,817593,130,165200,851012,ARL";
    auto fields3 = parse_csv_row(line3);
    std::cout << "Test 3: ";
    for (const auto& field : fields3) std::cout << field << " | ";
    std::cout << "\n";

    // Test case 4: Row with trailing commas
    std::string line4 = "2025-07-17T08:05:03.360842448Z,2025-07-17T08:05:03.360677248Z,160,2,1108,A,B,5.51,100,0,817593,130,165200,851012,ARL,";
    auto fields4 = parse_csv_row(line4);
    std::cout << "Test 4: ";
    for (const auto& field : fields4) std::cout << field << " | ";
    std::cout << "\n";

    // Test case 5: Fully quoted row
    std::string line5 = "\"2025-07-17T08:05:03.360842448Z\",\"2025-07-17T08:05:03.360677248Z\",\"160\",\"2\",\"1108\",\"A\",\"B\",\"5.51\",\"100\",\"0\",\"817593\",\"130\",\"165200\",\"851012\",\"ARL\"";
    auto fields5 = parse_csv_row(line5);
    std::cout << "Test 5: ";
    for (const auto& field : fields5) std::cout << field << " | ";
    std::cout << "\n";

    std::cout << "Tests completed.\n";
}

int main(int argc, char* argv[]) {
    // test_parse_csv_row();
    if (argc < 2) {
        std::cerr << "Usage: ./reconstruction_blockhouse mbo.csv\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
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
        std::string ts = row[1];
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
