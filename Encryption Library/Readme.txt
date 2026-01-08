Cryptography Library
A simple C implementation of classical cipher algorithms with performance benchmarking.
Features
* Caesar Cipher - Shift-based substitution cipher
* Vigenère Cipher - Polyalphabetic substitution with keyword
* Playfair Cipher - Digraph substitution using 5×5 matrix
* Performance Benchmarks - Compare cipher speeds
Usage
Interactive Mode
bash
./crypto
Select from the menu:
1. Caesar Cipher - Encrypt/decrypt with shift value
2. Vigenère Cipher - Encrypt/decrypt with keyword
3. Playfair Cipher - Encrypt with keyword
4. Benchmark - Compare cipher performance
5. Exit
Command Line
bash
./crypto benchmark    # Run performance tests
Build Commands
bash
make           # Build program
make run       # Build and run
make clean     # Remove build files
make debug     # Build with debug symbols
make benchmark # Build and run benchmarks

