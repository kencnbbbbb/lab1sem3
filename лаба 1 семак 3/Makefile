CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Istr
TARGET = dbms
SRC = main.cpp str/array.cpp str/linked_list.cpp str/one_linked_list.cpp str/stack.cpp str/queue.cpp str/tree.cpp
HEADERS = $(wildcard str/*.h)

all: $(TARGET)
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

go:
	cd golang && go build -o ../dbms-go .

test: all
	python3 tests/test_cli.py ./dbms
	$(CXX) $(CXXFLAGS) tests/test_tree.cpp str/tree.cpp -o test-tree
	./test-tree

test-go: go
	cd golang && go test ./...
	python3 tests/test_cli.py ./dbms-go

clean:
	rm -f dbms dbms-go test-tree *.o str/*.o

.PHONY: all go test test-go clean
