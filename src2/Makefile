# voir ligne 6 pour savoir quels fichiers sont compilés par le makefile. 
# crée un executable nommé "main". 
# faire 'make' pour lancer le makefile
# faire make clean pour nettoyer les .o

CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = main
SRCS = DKP_Data.cpp sacDyna.cpp main.cpp sousGradient.cpp common.cpp
OBJS = $(SRCS:.cpp=.o)

# Détecte le système d'exploitation
ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET_EXT = .exe
else
    RM = rm -f
    TARGET_EXT = 
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@$(TARGET_EXT) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(subst /,\,$(OBJS)) $(TARGET)$(TARGET_EXT)
else
	$(RM) $(OBJS) $(TARGET)$(TARGET_EXT)
endif

rebuild: clean all

.PHONY: all clean rebuild