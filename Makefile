CXX=g++
CXXFLAGS=-Wall -std=c++11 -g
LDFLAGS=

TARGET = bst
SRCS   = main.cpp bst.cpp parser.cpp
OBJS   = $(SRCS:.cpp=.o)
DEPS   = $(SRCS:.cpp=.depends)

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
