CPP      = g++ -std=c++20
SRCS     = $(shell find src -name "*.cpp")
OBJ      = $(addprefix obj/,$(SRCS:%.cpp=%.o)) 
DIR      = $(addprefix obj/,$(shell find src/ -type d)) 
BIN      = montador
RM       = rm -rf

.PHONY: all all-before clean

all: all-before ligador linker $(BIN)

all-before: 
	mkdir -p obj $(DIR)

clean:
	${RM} $(OBJ) linker $(BIN) obj

obj/%.o: %.cpp
	$(CPP) -c $< -o $@ -g

ligador: $(OBJ)
	$(CPP) ligador.cpp $(OBJ) -o linker -g

$(BIN): $(OBJ)
	$(CPP) $(BIN).cpp $(OBJ) -o $(BIN) -g