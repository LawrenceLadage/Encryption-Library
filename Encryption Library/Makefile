# Cryptography Library Makefile
# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

# Target executable
TARGET = crypto

# Source files
SOURCES = crypto.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program in interactive mode
run: $(TARGET)
	@echo "Starting interactive mode..."
	./$(TARGET)

# Run benchmarks
benchmark: $(TARGET)
	@echo "Running benchmarks..."
	./$(TARGET) benchmark

# Debug build with symbols and no optimization
debug: CFLAGS = -Wall -Wextra -g -std=c11 -DDEBUG
debug: clean $(TARGET)
	@echo "Debug build complete!"

# Release build with maximum optimization
release: CFLAGS = -Wall -Wextra -O3 -std=c11 -DNDEBUG
release: clean $(TARGET)
	@echo "Release build complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build files..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Install to system (requires root/sudo)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	install -m 755 $(TARGET) /usr/local/bin/
	@echo "Installation complete!"

# Uninstall from system
uninstall:
	@echo "Uninstalling $(TARGET)..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete!"

# Run with valgrind for memory leak detection
memcheck: debug
	@echo "Running valgrind memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Static analysis with cppcheck (if installed)
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES)

# Format code (requires clang-format)
format:
	@echo "Formatting code..."
	clang-format -i $(SOURCES)

# Show help
help:
	@echo "Cryptography Library - Available Make Targets:"
	@echo ""
	@echo "  make              - Build the program (default)"
	@echo "  make run          - Build and run in interactive mode"
	@echo "  make benchmark    - Build and run benchmarks"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make release      - Build optimized release version"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make install      - Install to /usr/local/bin (needs sudo)"
	@echo "  make uninstall    - Remove from /usr/local/bin (needs sudo)"
	@echo "  make memcheck     - Run with valgrind memory checker"
	@echo "  make analyze      - Run static code analysis"
	@echo "  make format       - Format code with clang-format"
	@echo "  make help         - Show this help message"
	@echo ""

# Phony targets (not actual files)
.PHONY: all run benchmark debug release clean install uninstall memcheck analyze format help