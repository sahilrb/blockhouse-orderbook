# Blockhouse-Orderbook
MBP-10 order book reconstruction from MBO data.

Problem:
Convert Market-by-Order (MBO) feed into Market-by-Price (MBP-10) snapshots.

## 🧠 Thought Process & Design Decisions

### 📌 Core Logic
- **Track each order by ID**: Using an `unordered_map` of `order_id → (side, price, size)`.
- **Maintain current book state**:
  - Two separate maps:
    - `bids`: `std::map<double, int, std::greater<double>>`
    - `asks`: `std::map<double, int>`
- **Update strategy**:
  - On `insert` or `update`: update price level by size difference.
  - On `delete`: subtract size from the level and remove if it becomes zero.
- **Snapshot logic**:
  - Capture top 10 bids and asks whenever a snapshot row appears.

---

### 💡 Why This Approach?
- **Efficient Top-N Access**: Using ordered maps ensures log-time access and iteration over sorted price levels.
- **Memory Efficient**: Only stores active orders and price levels, prunes zero-size levels.
- **Separation of Concerns**: Parsing, book updates, and writing snapshots are modularized.

---

### Key Features:
- Uses maps to keep bid/ask books sorted.
- Efficient parsing, avoids recomputation.
- Supports A, C, T actions.

---

### Instructions:

reconstruction_blockhouse.exe data/mbo.csv

---

### Output:
- mbp_output.csv is written in same format as mbp.csv

---

## Author:
Sahil Bhingradiya
