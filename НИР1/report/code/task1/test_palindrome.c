#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "palindrome.h"

#define TEST_WORDS 1000000

void generate_random_string(char *str, int len)
{
    for (int i = 0; i < len - 1; i++)
        str[i] = 'a' + rand() % 26;
    str[len - 1] = '\0';
}

double test_speed(int iterations, int word_len, bool use_recursive)
{
    char *word = malloc(word_len);
    clock_t start, end;
    double cpu_time_used;
    bool result;

    start = clock();

    for (int i = 0; i < iterations; i++)
    {
        generate_random_string(word, word_len);
        int len = string_len(word);

        if (use_recursive)
            result = palindrome_recursive(word, len);
        else
            result = palindrome(word, len);
        (void)result;
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    free(word);
    return cpu_time_used;
}

int main(int argc, char *argv[])
{
    int iterations = TEST_WORDS;
    int word_len = 10000;
    bool use_recursive = false;

    if (argc > 1)
        iterations = atoi(argv[1]);
    if (argc > 2)
        word_len = atoi(argv[2]) + 1; // +1 для '\0'
    if (argc > 3 && strcmp(argv[3], "recursive") == 0)
        use_recursive = true;

    printf("Тестирование скорости:\n");
    printf("Количество тестов: %d\n", iterations);
    printf("Длина слова: %d\n", word_len - 1);
    printf("Метод: %s\n", use_recursive ? "рекурсивный" : "итеративный");
    printf("Тип линковки: %s\n",
#ifdef STATIC_BUILD
           "статическая"
#else
           "динамическая"
#endif
    );

    double time_spent = test_speed(iterations, word_len, use_recursive);
    printf("Время выполнения: %.6f секунд\n", time_spent);
    printf("Среднее время на слово: %.9f секунд\n", time_spent / iterations);

    printf("\nДемонстрация работы:\n");
    char *test_words[] = {"levellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevellevel", "hellohellohellohellohellohellohellohellohellohellohello", "bebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebeb", "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa", "friendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriendsfriends", "A", "aA", ""};
    int num_tests = 8;

    for (int i = 0; i < num_tests; i++)
    {
        int len = string_len(test_words[i]);
        bool result1 = palindrome(test_words[i], len);
        bool result2 = palindrome_recursive(test_words[i], len);
        printf("'%s' (len=%d): итеративный=%d, рекурсивный=%d\n", test_words[i], len, result1, result2);
    }

    return 0;
}
