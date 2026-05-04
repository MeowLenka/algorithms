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

size_t List::get_size() const { return m_size; };

List::~List()
{
    clear();
}

List::Node *List::get_middle(Node *head) const
{
    if (head == nullptr || head->_pNext == nullptr)
        return head;

    Node *left = head;
    Node *right = head->_pNext;

    while (right != nullptr && right->_pNext != nullptr)
    {
        left = left->_pNext;
        right = right->_pNext->_pNext;
    }

    return left;
}

List::Node *List::merge(Node *left, Node *right)
{
    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;

    Node *result = nullptr;
    if (left->m_Data._radius <= right->m_Data._radius)
    {
        result = left;
        result->_pNext = merge(left->_pNext, right);
    }
    else
    {
        result = right;
        result->_pNext = merge(left, right->_pNext);
    }
    if (result->_pNext)
        result->_pNext->_pPrev = result; // чзх wtf!!!???
    result->_pPrev = nullptr;

    return result;
}

// todo: сделать merge sort прямо на элементах списка
List::Node *List::merge_sort(Node *head)
{
    if (head == nullptr || head->_pNext == nullptr)
        return head;
    Node *mid = get_middle(head);
    Node *mid_next = mid->_pNext;
    mid->_pNext = nullptr;
    if (mid_next)
        mid_next->_pPrev = nullptr;
    Node *left = merge_sort(head);
    Node *right = merge_sort(mid_next);

    return merge(left, right);
}

void List::sort()
{
    if (m_size < 2)
    {
        return;
    }

    Node *first = _head._pNext;
    first->_pPrev = nullptr;

    Node *last_node = _tail._pPrev;
    if (last_node)
        last_node->_pNext = nullptr;

    Node *sorted = merge_sort(first);

    _head._pNext = sorted;
    if (sorted)
        sorted->_pPrev = &_head;

    Node *last = sorted;
    while (last != nullptr && last->_pNext != nullptr)
    {
        last = last->_pNext;
    }

    if (last)
    {
        last->_pNext = &_tail;
        _tail._pPrev = last;
    }
    else
    {
        _head._pNext = &_tail;
        _tail._pPrev = &_head;
    }
}

std::ostream &operator<<(std::ostream &os, const List &lst)
{
    List::Node *cur = lst._head._pNext;
    while (cur != &lst._tail)
    {
        os << cur->m_Data._radius << ' ' << cur->m_Data._center.get_x() << ' ' << cur->m_Data._center.get_y() << '\n';
        cur = cur->_pNext;
    }

    return os;
}

void List::save_to_file(const char *filename) const
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

void List::load_from_file(const char *filename)
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