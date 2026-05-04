#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <cstddef>
#include <fstream>
#include "other.hpp"

class List
{
private:
    //todo: вложенный класс (nested). Почитать, какой уровень доступа имеет внешний класс к вложенному (без friend)
    //обычные правила доступа: к public и protected - есть, к private - нет 
    class Node
    {
    private:
        friend class List;
        Node *_pPrev;
        Node *_pNext;
        Circle m_Data;

    public:
        Node();
        Node(Node *prev, Node *next, const Circle &pc);
        friend std::ostream& operator<<(std::ostream& os, const List& lst);
        ~Node();
    };

    Node _head;
    Node _tail;
    size_t m_size;

    Node* get_middle(Node *head) const;
    Node *merge_sort(Node *head);
    Node *merge(Node *left, Node *right);

public:
    List();
    void add_begin(const Circle &pc);
    void add_end(const Circle &pc);
    bool remove(const Circle &rc);
    size_t remove_all(const Circle &rc);
    size_t get_size() const;
    void clear();
    void sort();

    ~List();

    void save_to_file(const char* filename) const;
    void load_from_file(const char* filename);
    
    friend std::ostream& operator<<(std::ostream& os, const List& lst);
    friend std::istream& operator>>(std::istream& is, List& lst);
};


#endif