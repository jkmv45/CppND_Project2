.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build

.PHONY: test
test:
	g++ -std=c++17 -o test/test_parser  test/linux_parser_tester.cpp src/linux_parser.cpp -I./include