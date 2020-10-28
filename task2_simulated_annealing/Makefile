CC := g++ -std=c++17 -O3
CFLAGS := -c -Wall
DIR := src/
# OBJECTS_LIST := main.o classes.o functions.o tinyxml2.o
GENERATOR_LIST := generator.o genfunctions.o tinyxml2.o
# OBJECTS := $(addprefix $(DIR), $(OBJECTS_LIST))
GENOBJECTS := $(addprefix $(DIR), $(GENERATOR_LIST))
ALG := simulated_annealing_alg
GEN := generate


# all: $(ALG) $(GEN)
all: $(GEN)

# simulated_annealing_alg: $(OBJECTS)
# 	$(CC) $(notdir $(OBJECTS)) -o $@ 


generate: $(GENOBJECTS)
	$(CC) $(notdir $(GENOBJECTS)) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) $^


clean:
	rm -rf  *.o $(ALG) $(GEN)
