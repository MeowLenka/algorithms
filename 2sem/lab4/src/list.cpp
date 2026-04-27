#include "list.hpp"

List::Node::Node()
{
    _pPrev = nullptr;
    _pNext = nullptr;
}

List::Node::Node(Node *prev, Node *next, const Circle &pc) : m_Data(pc), _pPrev(prev), _pNext(next)
{
    if (prev != nullptr)
        prev->_pNext = this;
    if (next != nullptr)
        next->_pPrev = this;
}

List::Node::~Node()
{
    if (_pPrev != nullptr)
        _pPrev->_pNext = this->_pNext;

    if (_pNext != nullptr)
        _pNext->_pPrev = this->_pPrev;
}

List::List()
{
    _head._pNext = &_tail;
    _head._pPrev = nullptr;
    _tail._pPrev = &_head;
    _tail._pNext = nullptr;
    m_size = 0;
}

void List::add_begin(const Circle &rc)
{
    Node *node = new Node(&_head, _head._pNext, rc);
    m_size++;
}

void List::add_end(const Circle &rc)
{
    Node *node = new Node(_tail._pPrev, &_tail, rc);
    m_size++;
}

bool List::remove(const Circle &rc)
{
    Node *cur = _head._pNext;
    while (cur != &_tail)
    {
        Node *next = cur->_pNext;
        if (cur->m_Data == rc)
        {
            delete cur;
            m_size--;
            return true;
        }
        cur = next;
    }
    return false;
}

size_t List::remove_all(const Circle &rc)
{
    size_t count = 0;
    Node *cur = _head._pNext;
    while (cur != &_tail)
    {
        Node *next = cur->_pNext;
        if (cur->m_Data == rc)
        {
            delete cur;
            m_size--;
            count++;
        }
        cur = next;
    }
    return count;
}

void List::clear()
{
    Node *cur = _head._pNext;
    while (cur != &_tail)
    {
        Node *next = cur->_pNext;
        delete cur;
        cur = next;
    }
    m_size = 0;
    _head._pNext = &_tail;
    _tail._pPrev = &_head;
}

List::~List()
{
    clear();
}

void List::sort()
{
    Node *cur = _head._pNext;
    if (m_size <= 1)
        return;
    while (cur->_pNext != &_tail)
    {
        Node *min = cur;
        Node *next = cur->_pNext;
        while (next != &_tail)
        {
            if (next->m_Data._radius < min->m_Data._radius)
                min = next;
            next = next->_pNext;
        }

        if (min != cur)
        {
            if (cur->_pNext == min) //если соседи
            {
                Node *cur_prev = cur->_pPrev;
                Node *min_next = min->_pNext;

                if (cur_prev != nullptr)
                    cur_prev->_pNext = min;
                min->_pPrev = cur_prev;
                min->_pNext = cur;

                cur->_pPrev = min;
                cur->_pNext = min_next;

                if (min_next != nullptr)
                    min_next->_pPrev = cur;
            }
            else
            {
                Node *cur_prev = cur->_pPrev;
                Node *cur_next = cur->_pNext;
                Node *min_prev = min->_pPrev;
                Node *min_next = min->_pNext;

                if (cur_prev != nullptr)
                    cur_prev->_pNext = min;
                min->_pPrev = cur_prev;
                min->_pNext = cur_next;
                if (cur_next != nullptr)
                    cur_next->_pPrev = min;

                if (min_prev != nullptr)
                    min_prev->_pNext = cur;
                cur->_pPrev = min_prev;
                cur->_pNext = min_next;
                if (min_next != nullptr)
                    min_next->_pPrev = cur;
            }
            cur = min;
        }
        cur = cur->_pNext;
    }
}

std::ostream& operator<<(std::ostream& os, const List& lst)
{
    List::Node *cur = lst._head._pNext;
    while (cur != &lst._tail)
    {
        os << cur->m_Data._radius << ' ' << cur->m_Data._center.get_x() << ' ' << cur->m_Data._center.get_y() << '\n';
        cur = cur->_pNext;
    }

    return os;
}


void List::save_to_file(const char* filename) const
{
    std::ofstream fout(filename);
    if (!fout.is_open())
    {
        throw "Open file error";
        return;
    }
    fout << *this; 
    fout.close();
}


void List::load_from_file(const char* filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        throw "Open file error";
        return;
    }
    clear();
    double radius, x, y;
    while (fin >> radius >> x >> y)
    {
        Point center(x, y);
        Circle circle(radius, center);
        add_end(circle);
    }
    fin.close();
}

// std::istream& operator>>(std::istream& is, List& lst)
// {
//     double radius, x, y;
//     while (is >> radius >> x >> y)
//     {
//         Point center(x, y);
//         Circle circle(radius, center);
//         lst.add_end(circle);
//     }
//     return is;
// }