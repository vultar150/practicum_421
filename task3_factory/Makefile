CC := clang++  -std=c++2a -lgtest
CFLAGS := -c -Wall
DIR := src/
DIR_TESTS := tests/
ALL_OBJECT_LIST := main.o
ALL_OBJECT_TEST_LIST := functional_test.o
OBJECTS := $(addprefix $(DIR), $(ALL_OBJECT_LIST))
OBJECTS_TEST := $(addprefix $(DIR_TESTS), $(ALL_OBJECT_TEST_LIST))
FUNC := functions
TEST := test


all: $(FUNC) $(TEST)

functions: $(OBJECTS)
	$(CC) $(notdir $(OBJECTS)) -o $@

test: $(OBJECTS_TEST)
	$(CC) $(notdir $(OBJECTS_TEST)) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o $(FUNC) $(TEST)
