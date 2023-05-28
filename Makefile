CPP      = g++
SRCS     = $(shell find . -name "*.cpp")
OBJ      = $(addprefix obj/,$(SRCS:%.cpp=%.o)) 
DIR      = $(addprefix obj/,$(shell find src/ -type d)) 
BIN      = montador
RM       = rm -rf

.PHONY: all all-before clean

all: all-before $(BIN)

all-before: 
	mkdir -p obj $(DIR)

clean:
	${RM} $(OBJ) $(BIN) obj

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o montador

obj/%.o: %.cpp
	$(CPP) -c $< -o $@