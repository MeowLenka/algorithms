#include "notification_queue.hpp"
#include <iostream>

NotificationQueue::NotificationQueue() : head(nullptr), tail(nullptr), count(0) {}

NotificationQueue::~NotificationQueue()
{
    while (count != 0)
        pop();
}

void NotificationQueue::push(const Notification &notification)
{
    QueueNode *newNode = new QueueNode(notification);
    if (count == 0)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

int NotificationQueue::size() const { return count; }

Notification NotificationQueue::pop()
{
    if (count == 0)
    {
        throw "Queue is empty!";
    }

    QueueNode *oldHead = head;
    Notification data = oldHead->data;
    head = head->next;

    if (head == nullptr)
        tail = nullptr;

    delete oldHead;
    count--;
    return data;
}

QueueIterator NotificationQueue::begin()
{
    return QueueIterator(head);
}

QueueIterator NotificationQueue::end()
{
    return QueueIterator(nullptr);
}
