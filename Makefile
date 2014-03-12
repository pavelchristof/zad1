CXXFLAGS= -Wall -std=c++11 -g -Isrc

DIRS    = build build/src build/tests deps deps/src deps/tests

TARGET  = zad1
MAIN    = src/$(TARGET).cpp
MAINOBJ = build/src/$(TARGET).o

ALLSRCS = $(wildcard src/*.cpp)
SRCS    = $(subst $(MAIN), , $(ALLSRCS))
OBJS    = $(patsubst src/%.cpp, build/src/%.o, $(SRCS))
DEPS    = $(patsubst src/%.cpp, deps/src/%.d,  $(ALLSRCS))

TSRCS   = $(wildcard tests/*.cpp)
TESTS   = $(patsubst tests/%.cpp, build/tests/%, $(TSRCS))
TDEPS   = $(patsubst tests/%.cpp, deps/tests/%.d, $(TSRCS))

.PHONY: clean all tests runtests help

all: $(TARGET) tests

# Build directories.
$(DIRS):
	@mkdir -p $@

# The main target.
$(TARGET): $(MAINOBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $(MAINOBJ) $(OBJS) -o $@

# Object files.
build/%.o: %.cpp | $(DIRS)
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Dependency files.
deps/%.d: %.cpp | $(DIRS)
	$(CXX) -MM -MT '$(patsubst %.cpp, build/%.o, $<)' $(CXXFLAGS) $< > $@

# Unit tests.
tests: $(OBJS) $(TESTS) | $(DIRS)

runtests: tests
	@$(foreach test, $(TESTS), ./$(test);)

build/tests/%: build/tests/%.o $(OBJS) | $(DIRS)
	$(CXX) $(CXXFLAGS) $(OBJS) $< -o $@

# Cleaning.
clean:
	rm -rf $(DIRS) $(TARGET)

# Help
help:
	@echo "Commands:"
	@echo "    all - build $(TARGET) and tests"
	@echo "    $(TARGET) - build $(TARGET)"
	@echo "    tests - build tests"
	@echo "    runtests - run all tests"
	@echo "    clean - remove build files and compiled binaries"
	@echo "    help - display this help"

-include $(DEPS) $(TDEPS)
