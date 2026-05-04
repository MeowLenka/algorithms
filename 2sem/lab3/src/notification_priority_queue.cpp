#include "notification_priority_queue.hpp"
#include <iostream>

NotificationPriorityQueue::NotificationPriorityQueue() : head(nullptr), count(0) {}

NotificationPriorityQueue::~NotificationPriorityQueue()
{
    while (count != 0)
        pop();
}

// чем меньше число, тем выше приоритет
int NotificationPriorityQueue::getPriorityValue(const Notification &notif) const
{
    // срочное системное
    if (notif._type == SYSTEM && notif.data.system.severity == EMERGENCY)
        return 0;
    // `мгновенное сообщение` > `обычное системное` > `уведомление от приложения`
    switch (notif._type)
    {
    case INSTANT:
        return 1;
    case SYSTEM:
        return 2;
    case APP:
        return 3;
    default:
        return 4;
    }
}

bool NotificationPriorityQueue::hasHigherPriority(const Notification &a, const Notification &b) const
{
    int priorityA = getPriorityValue(a);
    int priorityB = getPriorityValue(b);

    if (priorityA != priorityB)
    {
        return priorityA < priorityB; 
    }
    return a._time < b._time;
}

void NotificationPriorityQueue::push(const Notification &notification)
{
    PriorityQueueNode *newNode = new PriorityQueueNode(notification);

    // если очередь пуста или новый узел имеет более высокий приоритет, чем head
    if (head == nullptr || hasHigherPriority(notification, head->data))
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        PriorityQueueNode *current = head;
        while (current->next != nullptr &&
               !hasHigherPriority(notification, current->next->data))
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    count++;
}

int NotificationPriorityQueue::size() const
{
    return count;
}

Notification NotificationPriorityQueue::pop()
{
    if (count == 0)
    {
        throw "Priority queue is empty!";
    }
    PriorityQueueNode *oldHead = head;
    Notification data = oldHead->data;

    head = head->next;
    delete oldHead;
    count--;

    return data;
}

PriorityQueueIterator NotificationPriorityQueue::begin()
{
    return PriorityQueueIterator(head);
}

PriorityQueueIterator NotificationPriorityQueue::end()
{
    return PriorityQueueIterator(nullptr);
}