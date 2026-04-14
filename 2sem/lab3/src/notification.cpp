#include "notification.hpp"
#include <iostream>
#include <cstdio>

Notification Notification::createSystemNotification(const char *message, SeverityLevel severity)
{
    Notification notif;
    notif._time = time(nullptr);
    notif._type = SYSTEM;

    strncpy(notif.data.system.message, message, 255);
    notif.data.system.message[255] = '\0';
    notif.data.system.severity = severity;
    return notif;
}

Notification Notification::createInstantMessage(const char *contact, const char *message)
{
    Notification notif;
    notif._time = time(nullptr);
    notif._type = INSTANT;

    strncpy(notif.data.instant.contact_name, contact, 99);
    notif.data.instant.contact_name[99] = '\0';
    strncpy(notif.data.instant.message, message, 255);
    notif.data.instant.message[255] = '\0';

    return notif;
}

Notification Notification::createAppNotification(const char *app_name, const char *title, const char *message)
{
    Notification notif;
    notif._time = time(nullptr);
    notif._type = APP;

    strncpy(notif.data.app.app_name, app_name, 99);
    notif.data.app.app_name[99] = '\0';
    strncpy(notif.data.app.title, title, 99);
    notif.data.app.title[99] = '\0';
    strncpy(notif.data.app.message, message, 255);
    notif.data.app.message[255] = '\0';

    return notif;
}

void Notification::print() const
{
    char time_str[100];
    getTimeString(time_str, sizeof(time_str));

    std::cout << time_str << '\n';
    switch (_type)
    {
    case SYSTEM:
        std::cout << "System ";
        if (data.system.severity == EMERGENCY)
        {
            std::cout << "Emergency: ";
        }
        else
        {
            std::cout << "Regular: ";
        }
        std::cout << data.system.message << '\n';
        break;

    case INSTANT:
        std::cout << "Instant " << data.instant.contact_name << ": ";
        std::cout << data.instant.message << '\n';
        break;

    case APP:
        std::cout << "Application " << data.app.app_name << " : ";
        std::cout << data.app.title << ": ";
        std::cout << data.app.message << '\n';
        break;
    }
}

void Notification::getTimeString(char *buffer, int size) const
{
    struct tm *time_info = localtime(&_time);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}

int countNotificationsByType(const Notification *notifications, int size, Type type)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (notifications[i]._type == type)
        {
            count++;
        }
    }
    return count;
}