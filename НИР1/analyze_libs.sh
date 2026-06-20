echo "Анализ разделяемых библиотек : "
echo ""

make clean
make

if [ ! -f "price_db" ]; then
    echo "Ошибка: файл price_db не найден!"
    exit 1
fi

# анализ с ldd
echo "1. Зависимости исполняемого файла:"
ldd price_db
echo ""

echo "2. Анализ библиотек:"
for lib in $(ldd price_db | grep -E "=>" | awk '{print $3}'); do
    if [ -f "$lib" ]; then
        echo "Библиотека: $(basename $lib)"
        echo "  Путь: $lib"
        echo "  Размер: $(du -h $lib | cut -f1)"
        echo "  Тип: $(file -b $lib)"
        echo ""
    fi
done

echo "3. Дерево зависимостей:"
echo "price_db"
ldd price_db | grep -E "=>" | while read line; do
    lib=$(echo $line | awk '{print $1}')
    path=$(echo $line | awk '{print $3}')
    echo "  ├── $lib"
    if [ -f "$path" ]; then
        ldd $path 2>/dev/null | grep -E "=>" | while read line2; do
            lib2=$(echo $line2 | awk '{print $1}')
            echo "  │   └── $lib2"
        done
    fi
done

echo ""
echo "4. Рекурсивное дерево зависимостей :"
print_tree() {
    local lib=$1
    local indent=$2
    local prefix=$3
    
    if [ -z "$lib" ]; then
        return
    fi
    
    echo "${indent}${prefix} ${lib}"
    
    local path=$(ldd $lib 2>/dev/null | grep -E "=>" | head -1 | awk '{print $3}')
    if [ -f "$path" ]; then
        print_tree "$path" "$indent    " "└──"
    fi
}

print_tree "./price_db" "" "└──"
