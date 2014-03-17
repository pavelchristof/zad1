CXXFLAGS= -Wall -Wextra -std=c++11 -O2 -Isrc

DIRS    = build build/src build/tests

TARGET  = zad1.e
MAIN    = src/zad1.cpp
MAINOBJ = $(patsubst %.cpp, build/%.o, $(MAIN))

ALLSRCS = $(wildcard src/*.cpp)
SRCS    = $(subst $(MAIN), , $(ALLSRCS))
OBJS    = $(patsubst %.cpp, build/%.o, $(SRCS))
DEPS    = $(patsubst %.cpp, build/%.d,  $(ALLSRCS))

TSRCS   = $(wildcard tests/*.cpp)
TESTS   = $(patsubst %.cpp, build/%, $(TSRCS))
TDEPS   = $(patsubst %.cpp, build/%.d, $(TSRCS))

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
build/%.d: %.cpp | $(DIRS)
	@$(CXX) -MM -MT '$(patsubst %.cpp, build/%.o, $<)' $(CXXFLAGS) $< > $@

# Unit tests.
tests: $(OBJS) $(TESTS) | $(DIRS)

runtests: tests
	@$(foreach test, $(TESTS), ./$(test);)

$(TESTS): % : %.o $(OBJS) | $(DIRS)
	$(CXX) $(CXXFLAGS) $(OBJS) $< -o $@

# Cleaning.
clean:
	rm -rf $(DIRS) $(TARGET)

# Help
help:
	@echo "Commands:"
	@echo "    all - Build $(TARGET) and tests."
	@echo "    $(TARGET) - Build $(TARGET)."
	@echo "    tests - Build tests."
	@echo "    runtests - Run all tests."
	@echo "    clean - Remove build files and compiled binaries."
	@echo "    help - Display this help."

-include $(DEPS) $(TDEPS)
