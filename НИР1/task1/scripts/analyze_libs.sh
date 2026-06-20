echo "Анализ разделяемых библиотек (CMake) :"
echo ""

# сборка в режиме release
mkdir -p build_analysis
cd build_analysis
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc) > /dev/null 2>&1

echo "1. Статическая линковка:"
ldd bin/test_palindrome
echo ""

echo "2. Динамическая линковка:"
ldd bin/test_palindrome_dynamic
echo ""

echo "3. Программа с dlopen:"
ldd bin/test_dlopen
echo ""

echo "4. Зависимости динамической библиотеки:"
ldd lib/libpalindrome.so
echo ""

echo "5. Размеры файлов:"
ls -lh bin/test_palindrome bin/test_palindrome_dynamic bin/test_dlopen lib/*.a lib/*.so
echo ""

cd ..
cd ..
