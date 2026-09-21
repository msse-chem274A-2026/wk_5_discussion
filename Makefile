# Makefile -- Discussion 5
#
#   make            build everything students need
#   make demo1      what a copy costs
#   make demo2      reference or pointer?
#   make warmup     Warm-up -- pointer syntax
#   make part2     Part 2 -- copy constructors
#   make check      confirm your toolchain works before the session
#   make clean

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-function -g -O2

DEMOS     = demo1 demo2
EXERCISES = warmup part2

all: $(DEMOS) $(EXERCISES)

demo1: demo1_cost.cpp
	$(CXX) $(CXXFLAGS) -o demo1 demo1_cost.cpp

# -O0 so nothing about part (c) is optimized into looking fine.
demo2: demo2_refs_pointers.cpp
	$(CXX) $(CXXFLAGS) -O0 -o demo2 demo2_refs_pointers.cpp

# Same program with the address sanitizer on. Run it as ./demo2-asan -- it
# reports the bug and exits non-zero. (No ASAN_OPTIONS needed.)
demo2-asan: demo2_refs_pointers.cpp
	$(CXX) $(CXXFLAGS) -O0 -fsanitize=address -o demo2-asan demo2_refs_pointers.cpp

warmup: warmup_pointers.cpp
	$(CXX) $(CXXFLAGS) -o warmup warmup_pointers.cpp

part2: part2_copies.cpp
	$(CXX) $(CXXFLAGS) -o part2 part2_copies.cpp

# Run this once before the session. If it prints OK you are ready.
check:
	@echo "--- compiler ---"
	@$(CXX) --version | head -1
	@echo "--- make ---"
	@$(MAKE) --version | head -1
	@echo "--- test compile ---"
	@$(CXX) $(CXXFLAGS) -o .check_build part2_copies.cpp && rm -f .check_build
	@echo ""
	@echo "OK - your toolchain builds this repo."

clean:
	rm -f $(DEMOS) $(EXERCISES) demo2-asan

.PHONY: all check clean
