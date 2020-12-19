CC := clang++  -std=c++2a
CFLAGS := -c -Wall
DIR := src/
ALL_OBJECT_LIST := main.o population.o survival_function.o selection.o
OBJECTS := $(addprefix $(DIR), $(ALL_OBJECT_LIST))
GA := genetic_algorithm


all: $(GA)

genetic_algorithm: $(OBJECTS)
	$(CC) $(notdir $(OBJECTS)) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o $(GA)
