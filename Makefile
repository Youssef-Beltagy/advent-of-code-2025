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

# Helpful references:
# https://stackoverflow.com/questions/1814270/gcc-g-option-to-place-all-object-files-into-separate-directory
# https://makefiletutorial.com/
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
# https://www.gnu.org/software/make/manual/html_node/Selective-Search.html
