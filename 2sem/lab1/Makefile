PREF_SRC = ./src/
PREF_BUILD = ./build/
PREF_TESTS = ./tests/
CC = clang++

TARGET_DEBUG = $(PREF_BUILD)debug.out
TARGET_LEAKS = $(PREF_BUILD)leaks.out
SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_BUILD)%.o, $(SRC))
# text_wrapper.cpp

$(TARGET_DEBUG): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET_DEBUG) 

$(TARGET_LEAKS): $(OBJ)
	$(CC) -g -fsanitize=address $(OBJ) -o $(TARGET_LEAKS) 

build/rect.o: src/rect.cpp src/rect.hpp
	$(CC) -g -c -o build/rect.o src/rect.cpp

build/barrel.o: src/barrel.cpp src/barrel.hpp
	$(CC) -g -c -o build/barrel.o src/barrel.cpp

build/matrix.o: src/matrix.cpp src/matrix.hpp
	$(CC) -g -c -o build/matrix.o src/matrix.cpp

build/my_string.o: src/my_string.cpp src/my_string.hpp
	$(CC) -g -c -o build/my_string.o src/my_string.cpp

build/lab1.o	:	src/lab1.cpp
	$(CC) -g -c -o build/lab1.o src/lab1.cpp 

debug: $(TARGET_DEBUG)
	gdb $(TARGET_DEBUG)

leaks: $(TARGET_LEAKS) 
	$(CC) $(TARGET_LEAKS)

clean:
	rm $(TARGET_DEBUG) $(PREF_BUILD)*.o $(PREF_BUILD)debug


.PHONY : clean debug leaks