CC := clang++ -O3 -std=c++2a -Xpreprocessor -fopenmp -lomp -F/Library/Frameworks -framework SDL2
CFLAGS := -c -Wall
DIR := src/
ALL_OBJECT_LIST := main.o population.o survival_function.o selection.o crossover.o mutation.o functions.o print.o
OBJECTS := $(addprefix $(DIR), $(ALL_OBJECT_LIST))
GA := genetic_algorithm


all: $(GA)

genetic_algorithm: $(OBJECTS)
	$(CC) $(notdir $(OBJECTS)) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o $(GA)
