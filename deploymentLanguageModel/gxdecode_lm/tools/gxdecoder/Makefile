CXX = g++

CXXFLAGS = -g -std=c++11 -MMD -Wall -I src -I . -D USE_VARINT -lpthread -msse4.1 

OBJ = src/fst.o src/utils.o

TOOL = tools/fst-init

all: $(TOOL) $(OBJ)

check:
	for file in src/*.h src/*.cc tools/*.cc; do \
		echo $$file; \
        cpplint --filter=-build/header_guard,-readability/check,-build/include_subdir $$file; \
	done

tools/%: tools/%.cc $(OBJ)
	$(CXX) $< $(OBJ) $(CXXFLAGS) -o $@

clean:
	rm -rf $(OBJ); rm -rf $(TEST); rm -rf $(TOOL); \
    rm -rf src/*.d; rm -rf tools/*.d;

-include */*.d
