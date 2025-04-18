CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = test
SRCS = DKP_Data.cpp sacDyna.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

gradient.o: gradient.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../src/readfile.o: ../src/readfile.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q gradient.o $(TARGET).exe
	del /Q ..\src\readfile.o
else
	rm -f gradient.o $(TARGET)
	rm -f ../src/readfile.o
endif

rebuild: clean all