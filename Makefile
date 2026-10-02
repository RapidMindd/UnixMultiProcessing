CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
SRC = main.cpp utils.cpp
OUT = a.out
ARGS ?=

.PHONY: run fmt clean

run: $(OUT)
	./$(OUT) $(ARGS)

$(OUT): $(SRC) child
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

child: child.cpp
	$(CXX) $(CXXFLAGS) child.cpp -o child

fmt:
	clang-format -i $(SRC)

clean:
	@rm -f $(OUT)
