#ifndef NOTIFICATION_PRIORITY_QUEUE_HPP
#define NOTIFICATION_PRIORITY_QUEUE_HPP

#include "notification.hpp"


struct PriorityQueueNode {
    Notification data;
    PriorityQueueNode* next;
    
    PriorityQueueNode(const Notification& notif) : data(notif), next(nullptr) {}
};


class PriorityQueueIterator {
private:
    PriorityQueueNode* current;
    
public:
    PriorityQueueIterator(PriorityQueueNode* node = nullptr) : current(node) {}
    
    Notification& operator*() { return current->data; }
    const Notification& operator*() const { return current->data; }
    
    Notification* operator->() { return &current->data; }
    const Notification* operator->() const { return &current->data; }
    
    PriorityQueueIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }
    
    PriorityQueueIterator operator++(int) {
        PriorityQueueIterator temp = *this;
        ++(*this);
        return temp;
    }
    
    bool operator==(const PriorityQueueIterator& other) const {
        return current == other.current;
    }
    
    bool operator!=(const PriorityQueueIterator& other) const {
        return current != other.current;
    }
};

class NotificationPriorityQueue {
private:
    PriorityQueueNode* head;
    int count;
    
    // сравнивает a и b (true усли а больше)
    bool hasHigherPriority(const Notification& a, const Notification& b) const;
    int getPriorityValue(const Notification& notif) const;
    
public:
    NotificationPriorityQueue();
    ~NotificationPriorityQueue();
    

    void push(const Notification& notification);
    int size() const;
    Notification pop();
    PriorityQueueIterator begin();
    PriorityQueueIterator end();
};

#endif