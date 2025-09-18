#include <iostream>
using namespace std;

int main() {
    float big = 1000000.0f;
    float small = 0.1f;
    
    // Добавляем много маленьких чисел к большому
    for (int i = 0; i < 1000; i++) {
        big += small;
    }
    
    // Ожидаем: 1000000 + 1000*0.1 = 1000100
    cout << "Результат: " << big << endl;
    cout << "Ожидалось: 1000100" << endl;
    cout << "Разница: " << 1000100 - big << endl;
    
    return 0;
}