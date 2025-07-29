# Blockhouse-Orderbook
MBP-10 order book reconstruction from MBO data

Problem:
Convert Market-by-Order (MBO) feed into Market-by-Price (MBP-10) snapshots.

Key Features:
- Uses maps to keep bid/ask books sorted.
- Efficient parsing, avoids recomputation.
- Supports A, C, T actions.

Instructions:
make
./reconstruction_blockhouse data/mbo.csv

Output:
- mbp_output.csv is written in same format as mbp.csv

Author:
Sahil Bhingradiya
