#include "notification.hpp"
#include "notification_queue.hpp"
#include "notification_priority_queue.hpp"
#include <cassert>
#include <iostream>

int test_notification()
{
    Notification sys = Notification::createSystemNotification("test system", EMERGENCY);
    assert(sys._type == SYSTEM);

    Notification inst = Notification::createInstantMessage("Skebob", "hello");
    assert(inst._type == INSTANT);

    Notification app = Notification::createAppNotification("app", "title", "message");
    assert(app._type == APP);

    Notification notifications[4];
    notifications[0] = Notification::createSystemNotification("sys1", REGULAR);
    notifications[1] = Notification::createInstantMessage("Bob", "hi");
    notifications[2] = Notification::createSystemNotification("sys2", EMERGENCY);
    notifications[3] = Notification::createAppNotification("App", "title", "msg");

    int sys_count = countNotificationsByType(notifications, 4, SYSTEM);
    assert(sys_count == 2);

    int inst_count = countNotificationsByType(notifications, 4, INSTANT);
    assert(inst_count == 1);

    int app_count = countNotificationsByType(notifications, 4, APP);
    assert(app_count == 1);

    return 0;
}

int test_queue()
{
    NotificationQueue queue;
    assert(queue.size() == 0);

    Notification n1 = Notification::createSystemNotification("test1", REGULAR);
    Notification n2 = Notification::createInstantMessage("Scebob", "test2");

    queue.push(n1);
    queue.push(n2);
    assert(queue.size() == 2);
    Notification popped = queue.pop();
    assert(queue.size() == 1);

    return 0;
}

int test_priority_queue()
{
    NotificationPriorityQueue pq;

    Notification regular = Notification::createSystemNotification("regular", REGULAR);
    Notification emergency = Notification::createSystemNotification("emergency", EMERGENCY);
    Notification instant = Notification::createInstantMessage("Bob", "instant");
    Notification app = Notification::createAppNotification("app", "title", "msg");

    pq.push(regular);
    pq.push(instant);
    pq.push(emergency);
    pq.push(app);
    Notification first = pq.pop();
    assert(first._type == SYSTEM);

    Notification second = pq.pop();
    assert(second._type == INSTANT);

    return 0;
}

int main()
{
    std::cout << "\nTesting Notification...\n";

    int result = 0;

    result = test_notification();
    if (result)
        return result;

    result = test_queue();
    if (result)
        return result;

    result = test_priority_queue();
    if (result)
        return result;

    std::cout << "\nAll Notification tests passed\n";
    return 0;
}