PREF_SRC = ./src/
PREF_BUILD = ./build/
PREF_TESTS = ./tests/
CC = clang++

TARGET_DEBUG = $(PREF_BUILD)debug.out
TARGET_LEAKS = $(PREF_BUILD)leaks.out

DEP_FLAGS=-MMD -MP
FLAGS=$(foreach D, src, -I$(D)) -g $(DEP_FLAGS)

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_BUILD)%.o, $(SRC))
DEP = $(patsubst $(PREF_DIR)%.cpp, $(PREF_BUILD)%.d, $(SRC))

$(TARGET_DEBUG): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET_DEBUG) 

$(TARGET_LEAKS): $(OBJ)
	$(CC) -g -fsanitize=address $(OBJ) -o $(TARGET_LEAKS) 


build/rect.o: $(PREF_SRC)rect.cpp $(PREF_SRC)rect.hpp
	$(CC) -g -c -o $(PREF_BUILD)rect.o $(PREF_SRC)rect.cpp

build/barrel.o: $(PREF_SRC)barrel.cpp $(PREF_SRC)barrel.hpp
	$(CC) -g -c -o $(PREF_BUILD)barrel.o $(PREF_SRC)barrel.cpp

build/matrix.o: $(PREF_SRC)matrix.cpp $(PREF_SRC)matrix.hpp
	$(CC) -g -c -o $(PREF_BUILD)matrix.o $(PREF_SRC)matrix.cpp

build/my_string.o: $(PREF_SRC)my_string.cpp $(PREF_SRC)my_string.hpp
	$(CC) -g -c -o $(PREF_BUILD)my_string.o $(PREF_SRC)my_string.cpp

build/lab1.o	:	$(PREF_SRC)lab1.cpp
	$(CC) -g -c -o $(PREF_BUILD)lab1.o $(PREF_SRC)lab1.cpp 


$(PREF_BUILD)test/test_bounding_rect.out: $(PREF_BUILD)test/test_bounding_rect.o $(PREF_BUILD)rect.o
	$(CC) $(FLAGS) -o $@ $^

$(PREF_BUILD)test/test_rect_basic_methods.out: $(PREF_BUILD)test/test_rect_basic_methods.o $(PREF_BUILD)rect.o
	$(CC) $(FLAGS) -o $@ $^

$(PREF_BUILD)test/test_rect_operations.out: $(PREF_BUILD)test/test_rect_operations.o $(PREF_BUILD)rect.o
	$(CC) $(FLAGS) -o $@ $^

$(PREF_BUILD)test/test_rect_properties.out: $(PREF_BUILD)test/test_rect_properties.o $(PREF_BUILD)rect.o
	$(CC) $(FLAGS) -o $@ $^

$(PREF_BUILD)test/%.o: $(PREF_TESTS)%.cpp
	$(CC)  $(FLAGS) -c $< -o $@

debug: $(TARGET_DEBUG)
	gdb $(TARGET_DEBUG)

leaks: $(TARGET_LEAKS) 
	$(CC) $(TARGET_LEAKS)

test: $(PREF_BUILD)test/test_bounding_rect.out $(PREF_BUILD)test/test_rect_basic_methods.out $(PREF_BUILD)test/test_rect_operations.out $(PREF_BUILD)test/test_rect_properties.out
	$(PREF_BUILD)test/test_bounding_rect.out
	$(PREF_BUILD)test/test_rect_basic_methods.out
	$(PREF_BUILD)test/test_rect_operations.out
	$(PREF_BUILD)test/test_rect_properties.out

	
clean:
	rm $(TARGET_DEBUG) $(PREF_BUILD)*.o $(PREF_BUILD)debug $(PREF_BUILD)test/*.out


.PHONY : clean debug leaks 

-include $(DEP)