#ifndef NOTIFICATION_QUEUE_HPP
#define NOTIFICATION_QUEUE_HPP

#include "notification.hpp"

struct QueueNode
{
    Notification data;
    QueueNode *next;
    QueueNode(const Notification &notif) : data(notif), next(nullptr) {}
};

class QueueIterator
{
private:
    QueueNode *current;

public:
    QueueIterator(QueueNode *node) : current(node) {}
    Notification &operator*() { return current->data; }
    const Notification &operator*() const { return current->data; }

    Notification *operator->() { return &current->data; }

    QueueIterator &operator++()
    {
        if (current)
        {
            current = current->next;
        }
        return *this;
    }

    QueueIterator operator++(int)
    {
        QueueIterator temp = *this;
        ++(*this);
        return temp;
    }
    bool operator==(const QueueIterator &other) const
    {
        return current == other.current;
    }

    bool operator!=(const QueueIterator &other) const
    {
        return current != other.current;
    }
};

class NotificationQueue
{
private:
    QueueNode *head; // первый элемент
    QueueNode *tail; // последний элемент
    int count;       // количество элементов

public:
    NotificationQueue();
    ~NotificationQueue();

    void push(const Notification &notification);
    int size() const;
    Notification pop(); // - возвращает следующее уведомление в порядке First In First Out  и удаляет его из очереди;
    QueueIterator begin();
    QueueIterator end();
};

#endif