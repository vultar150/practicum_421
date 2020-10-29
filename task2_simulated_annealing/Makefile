CC := g++ -std=c++2a -O3
CFLAGS := -c -Wall
DIR := src/
OBJECTS_LIST := main.o classes.o tinyxml2.o
GENERATOR_LIST := generator.o genfunctions.o tinyxml2.o
OBJECTS := $(addprefix $(DIR), $(OBJECTS_LIST))
GENOBJECTS := $(addprefix $(DIR), $(GENERATOR_LIST))
ALG := simulatedAnnealingAlg
GEN := generate


all: $(ALG) $(GEN)

simulatedAnnealingAlg: $(OBJECTS)
	$(CC) $(notdir $(OBJECTS)) -o $@ 

generate: $(GENOBJECTS)
	$(CC) $(notdir $(GENOBJECTS)) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o $(ALG) $(GEN)
