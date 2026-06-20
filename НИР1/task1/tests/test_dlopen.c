#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dlfcn.h>
#include <stdbool.h>
#include <unistd.h>
#include <libgen.h>

#define TEST_WORDS 1000000

typedef bool (*palindrome_func_t)(char *, int);
typedef int (*string_len_func_t)(char *);

char* get_library_path() {
    static char lib_path[512];
    char *lib_dir = getenv("PALINDROME_LIB_DIR");
    if (lib_dir != NULL) {
        #ifdef __APPLE__
        snprintf(lib_path, sizeof(lib_path), "%s/libpalindrome.dylib", lib_dir);
        #else
        snprintf(lib_path, sizeof(lib_path), "%s/libpalindrome.so", lib_dir);
        #endif
        return lib_path;
    }
    
    #ifdef __APPLE__
    if (access("./libpalindrome.dylib", F_OK) == 0) {
        snprintf(lib_path, sizeof(lib_path), "./libpalindrome.dylib");
        return lib_path;
    }
    #else
    if (access("./libpalindrome.so", F_OK) == 0) {
        snprintf(lib_path, sizeof(lib_path), "./libpalindrome.so");
        return lib_path;
    }
    #endif
    
    char exe_path[512];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        char *dir = dirname(exe_path);
        #ifdef __APPLE__
        snprintf(lib_path, sizeof(lib_path), "%s/../lib/libpalindrome.dylib", dir);
        if (access(lib_path, F_OK) == 0) {
            return lib_path;
        }
        snprintf(lib_path, sizeof(lib_path), "%s/libpalindrome.dylib", dir);
        #else
        snprintf(lib_path, sizeof(lib_path), "%s/../lib/libpalindrome.so", dir);
        if (access(lib_path, F_OK) == 0) {
            return lib_path;
        }
        snprintf(lib_path, sizeof(lib_path), "%s/libpalindrome.so", dir);
        #endif
        return lib_path;
    }
    
    #ifdef __APPLE__
    snprintf(lib_path, sizeof(lib_path), "libpalindrome.dylib");
    #else
    snprintf(lib_path, sizeof(lib_path), "libpalindrome.so");
    #endif
    return lib_path;
}

void generate_random_string(char *str, int len) {
    for (int i = 0; i < len - 1; i++) {
        str[i] = 'a' + rand() % 26;
    }
    str[len - 1] = '\0';
}

double test_speed(int iterations, int word_len, palindrome_func_t palindrome_func, string_len_func_t len_func) {
    char *word = malloc(word_len);
    clock_t start, end;
    double cpu_time_used;
    bool result;
    
    start = clock();
    
    for (int i = 0; i < iterations; i++) {
        generate_random_string(word, word_len);
        int len = len_func(word);
        result = palindrome_func(word, len);
        (void)result;
    }
    
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    free(word);
    return cpu_time_used;
}

int main(int argc, char *argv[]) {
    void *handle;
    palindrome_func_t palindrome_iter, palindrome_rec;
    string_len_func_t string_len_func;
    char *error;
    
    char *lib_path = get_library_path();
    printf("Попытка загрузить библиотеку: %s\n", lib_path);
    
    handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
        return 1;
    }
    
    printf("Библиотека успешно загружена\n");
    
    // указатели на функции
    palindrome_iter = (palindrome_func_t)dlsym(handle, "palindrome");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Ошибка получения функции palindrome: %s\n", error);
        dlclose(handle);
        return 1;
    }
    
    palindrome_rec = (palindrome_func_t)dlsym(handle, "palindrome_recursive");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Ошибка получения функции palindrome_recursive: %s\n", error);
        dlclose(handle);
        return 1;
    }
    
    string_len_func = (string_len_func_t)dlsym(handle, "string_len");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Ошибка получения функции string_len: %s\n", error);
        dlclose(handle);
        return 1;
    }
    
    int iterations = TEST_WORDS;
    int word_len = 10;
    
    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    if (argc > 2) {
        word_len = atoi(argv[2]) + 1;
    }
    
    printf("\nТестирование скорости (dlopen):\n");
    printf("Количество тестов: %d\n", iterations);
    printf("Длина слова: %d\n", word_len - 1);
    
    // тест итеративной функции
    printf("\nИтеративная функция:\n");
    double time_iter = test_speed(iterations, word_len, palindrome_iter, string_len_func);
    printf("Время выполнения: %.6f секунд\n", time_iter);
    printf("Среднее время на слово: %.9f секунд\n", time_iter / iterations);
    
    // тест рекурсивной функции
    printf("\nРекурсивная функция:\n");
    double time_rec = test_speed(iterations, word_len, palindrome_rec, string_len_func);
    printf("Время выполнения: %.6f секунд\n", time_rec);
    printf("Среднее время на слово: %.9f секунд\n", time_rec / iterations);
    
    printf("\nДемонстрация работы (через dlopen):\n");
    char *test_words[] = {"levellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevel", "hellohellohellohellohellohellohellohellohellohellohello", "bebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebeb", "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa", "friendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriends", "", "A", "aA"};
    int num_tests = 8;
    
    for (int i = 0; i < num_tests; i++) {
        int len = string_len_func(test_words[i]);
        bool result1 = palindrome_iter(test_words[i], len);
        bool result2 = palindrome_rec(test_words[i], len);
        printf("'%s' (len=%d): итеративный=%d, рекурсивный=%d\n", 
               test_words[i], len, result1, result2);
    }
    
    dlclose(handle);    
    return 0;
}
