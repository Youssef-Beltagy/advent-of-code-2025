CXX      := g++
CPPFLAGS := -Iinclude -g -std=c++17

vpath %.cpp src
SRCS := $(wildcard *.cpp src/*.cpp)

OBJS := $(addprefix bin/,$(notdir $(SRCS:.cpp=.o)))

run: bin/main
	./bin/main

bin/main: $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $@

bin/%.o: %.cpp | bin # bin is an order only prerequisite
	$(CXX) $(CPPFLAGS) -c $< -o $@

bin:
	@mkdir -p $@

clean:
	@rm -rf bin


print:
	echo $(OBJS)
	echo $(SRCS)

.PHONY: clean run print
