# voir ligne 6 pour savoir quels fichiers sont compilés par le makefile. 
# crée un executable nommé "test". 
# faire 'make' pour lancer le makefile
# faire make clean pour nettoyer les .o

CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = main
SRCS = DKP_Data.cpp sacDyna.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	del /Q $(OBJS) $(TARGET).exe

rebuild: clean all