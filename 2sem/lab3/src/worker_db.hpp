#ifndef WORKER_DB_HPP
#define WORKER_DB_HPP

#include "my_string.hpp"

struct WorkerData
{
    // MyString surname;
    MyString _name;
    unsigned int _age;
    double _salary;

    WorkerData() : _name(""), _age(0), _salary(0.0) {}
    WorkerData(const char *n, int a, double s) : _name(n), _age(a), _salary(s) {}
};

// узeл хэш-таблицы
struct WorkerNode
{
    MyString _key;     // фамилия (ключ)
    WorkerData _value; // данные работника
    WorkerNode *_next; // указатель на следующий элемент (для разрешения коллизий)

    WorkerNode(const MyString &k, const WorkerData &v) : _key(k), _value(v), _next(nullptr) {}
};

class WorkerDb
{
private:
    static const int TABLE_SIZE = 100;
    WorkerNode *_table[TABLE_SIZE]; // массив указателей на узлы

    int hash(const MyString &key) const;

public:
    WorkerDb();
    ~WorkerDb();
    // todo: почитать про бинарные деревья поиска

    class Iterator
    {
        WorkerNode **_table;
        WorkerNode *_current;
        int _table_index; // текущий индекс в таблице
        int _table_size;  // размер таблицы

        // Вспомогательный метод для поиска следующего ненулевого узла
        void find_next_node();

    public:
        Iterator();
        Iterator(WorkerNode **table, int table_size, int start_index = 0);


        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        Iterator &operator++();   // ++it
        Iterator operator++(int); // it++
        WorkerData &operator*();
        WorkerData *operator->();

        MyString key() const;

        friend class WorkerDb;
        // Iterator(WorkerNode *cur) : _current(cur) {};
    };

    WorkerData &operator[](const MyString &key);
    WorkerData &operator[](const char *key);

    Iterator end();
    Iterator begin();
};

void print_db(WorkerDb& db);
double get_avg_age(WorkerDb& db);

#endif