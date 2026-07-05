echo "Тестирование оптимизаций компилятора с CMake :"
echo ""
# флаги оптимизации
OPTIMIZATIONS=("O0" "O1" "O2" "Os")
TEST_ITERATIONS=100000000
WORD_LENGTH=100000

NPROC=$(nproc 2>/dev/null || echo 4) 

echo "Используется $NPROC ядер для сборки"
echo ""

# директория для результатов
mkdir -p results

for opt in "${OPTIMIZATIONS[@]}"; do
    echo "Флаг оптимизации: -$opt"
    echo "----------------------------------------"
    
    # директория для сборки
    BUILD_DIR="build_$opt"
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    
    # конфигурация с флагом оптимизации
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-$opt"
    
    # сборка
    make clean > /dev/null 2>&1 # очищаем предыдущую сборку
    make -j$NPROC > /dev/null 2>&1 # собираем проект
    
    echo "Статическая линковка (итеративный):"
    time ./bin/test_palindrome $TEST_ITERATIONS $WORD_LENGTH iter
    
    echo ""
    echo "Статическая линковка (рекурсивный):"
    time ./bin/test_palindrome $TEST_ITERATIONS $WORD_LENGTH recursive
    
    echo ""
    echo "Динамическая линковка (итеративный):"
    time ./bin/test_palindrome_dynamic $TEST_ITERATIONS $WORD_LENGTH iter
    
    echo ""
    echo "Динамическая линковка (рекурсивный):"
    time ./bin/test_palindrome_dynamic $TEST_ITERATIONS $WORD_LENGTH recursive
    
    echo ""
    echo "dlopen (итеративный):"
    export PALINDROME_LIB_DIR=$(pwd)/lib
    time ./bin/test_dlopen $TEST_ITERATIONS $WORD_LENGTH
    
    echo ""
    # сохраняем размеры файлов
    echo "Размеры файлов:" >> ../results/sizes_$opt.txt
    ls -lh bin/test_palindrome bin/test_palindrome_dynamic bin/test_dlopen lib/*.a 2>/dev/null >> ../results/sizes_$opt.txt
    if [ -f "lib/libpalindrome.dylib" ]; then
        ls -lh lib/libpalindrome.dylib >> ../results/sizes_$opt.txt
    fi
    
    cd ..
    echo ""
done
