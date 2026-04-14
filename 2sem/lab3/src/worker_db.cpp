#include <iostream>
#include "worker_db.hpp"

WorkerDb::WorkerDb()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        _table[i] = nullptr;
    }
}

WorkerDb::~WorkerDb()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        WorkerNode *current = _table[i];
        while (current != nullptr)
        {
            WorkerNode *next = current->_next;
            delete current;
            current = next;
        }
    }
}

int WorkerDb::hash(const MyString &key) const
{
    int hash_value = 0;
    // хэш - сумма ASCII кодов
    for (int i = 0; i < key.get_str_size(); i++)
    {
        hash_value += key.get(i);
    }
    return hash_value % TABLE_SIZE;
}

WorkerData &WorkerDb::operator[](const MyString &key)
{
    int index = hash(key);
    // ищем существующий элемент
    WorkerNode *current = _table[index];
    while (current != nullptr)
    {
        if (current->_key == key)
        {
            return current->_value;
        }
        current = current->_next;
    }

    // если не нашли, то новый
    WorkerNode *new_node = new WorkerNode(key, WorkerData());
    new_node->_next = _table[index];
    _table[index] = new_node;

    return new_node->_value;
}

WorkerData &WorkerDb::operator[](const char *key)
{
    MyString key_str(key);
    return operator[](key_str);
}

///////////////////

WorkerDb::Iterator::Iterator()
    : _table(nullptr), _current(nullptr), _table_index(0), _table_size(0)
{
}

WorkerDb::Iterator::Iterator(WorkerNode **table, int table_size, int start_index)
    : _table(table), _table_index(start_index), _table_size(table_size)
{
    if (start_index < table_size)
    {
        _current = _table[start_index];
        if (_current == nullptr)
        {
            find_next_node();
        }
    }
    else
        _current = nullptr;
}

void WorkerDb::Iterator::find_next_node()
{
    while (_current == nullptr && _table_index < _table_size)
    {
        _current = _table[_table_index];
        if (_current == nullptr)
        {
            _table_index++;
        }
    }
}

bool WorkerDb::Iterator::operator==(const Iterator &other) const
{
    return (_current == other._current && _table_index == other._table_index);
}

bool WorkerDb::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

WorkerDb::Iterator &WorkerDb::Iterator::operator++()
{
    if (_current != nullptr)
    {
        _current = _current->_next;

        if (_current == nullptr)
        {
            _table_index++;
            find_next_node();
        }
    }
    return *this;
}

WorkerDb::Iterator WorkerDb::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

WorkerData &WorkerDb::Iterator::operator*()
{
    if (_current == nullptr)
        throw "End iterator(*)";
    return _current->_value;
}

WorkerData *WorkerDb::Iterator::operator->()
{
    if (_current == nullptr)
        throw "End iterator(->)";
    return &(_current->_value);
}

MyString WorkerDb::Iterator::key() const
{
    if (_current == nullptr)
        throw "Nullptr key";
    return _current->_key;
}

WorkerDb::Iterator WorkerDb::begin()
{
    return Iterator(_table, TABLE_SIZE, 0);
}

WorkerDb::Iterator WorkerDb::end()
{
    // return nullptr;
    return Iterator(_table, TABLE_SIZE, TABLE_SIZE);
}

void print_db(WorkerDb &db)
{
    int count = 0;
    for (auto it = db.begin(); it != db.end(); ++it)
    {
        count++;
        std::cout << it.key() << " -> " << it->_name
                  << " age: " << it->_age
                  << ", salary: " << it->_salary << '\n';
    }
    if (count == 0)
    {
        std::cout << "Database is empty\n";
    }
}

double get_avg_age(WorkerDb &db)
{
    int count = 0, sum = 0;
    for (auto it = db.begin(); it != db.end(); ++it)
    {
        count++;
        sum += it->_age;
    }
    return (double) sum /count;
}