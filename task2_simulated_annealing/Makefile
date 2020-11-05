CC := clang++ -O3  -std=c++2a -fopenmp=libomp -L/usr/local/Cellar/llvm/11.0.0/lib/
CFLAGS := -c -Wall
DIR := src/
ALL_OBJECT_LIST := main.o parameters.o log.o XDecision.o decrease.o mutation.o tinyxml2.o
GENERATOR_LIST := generator.o genfunctions.o tinyxml2.o
DECREASE := decrease.o
XDECISION := XDecision.o tinyxml2.o
MUTATION := mutation.o
OBJECTS := $(addprefix $(DIR), $(ALL_OBJECT_LIST))
GENOBJECTS := $(addprefix $(DIR), $(GENERATOR_LIST))
DECREASE_OBJECTS := $(addprefix $(DIR), $(DECREASE))
XDECISION_OBJECTS := $(addprefix $(DIR), $(XDECISION))
MUTATION_OBJECTS := $(addprefix $(DIR), $(MUTATION))
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
