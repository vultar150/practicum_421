CC := g++ -O3  -std=c++2a
CFLAGS := -c -Wall
DIR := src/
ALL_OBJECT_LIST := main.o functions.o
OBJECTS := $(addprefix $(DIR), $(ALL_OBJECT_LIST))
FUNC := functions


all: $(FUNC)

functions: $(OBJECTS)
	$(CC) $(notdir $(OBJECTS)) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o $(FUNC)
