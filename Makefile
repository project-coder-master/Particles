CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3
TARGET   := Particles

SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRCS     := $(wildcard *.cpp)
OBJS     := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
