#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <ctime>

enum Type {SYSTEM, INSTANT, APP};
enum SeverityLevel {REGULAR, EMERGENCY}; // system 

struct Notification
{
    time_t _time;
    Type _type;

    union {
        struct {
            char message[256];
            SeverityLevel severity;
        } system;
        
        struct {
            char contact_name[100];
            char message[256];
        } instant;
        
        struct {
            char app_name[100];
            char title[100];
            char message[256];
        } app;
    } data;

    static Notification createSystemNotification(const char* message, SeverityLevel severity);
    static Notification createInstantMessage(const char* contact, const char* message);
    static Notification createAppNotification(const char* app_name, const char* title, const char* message);
    
    void print() const;
    void getTimeString(char* buffer, int size) const; 
};

int countNotificationsByType(const Notification* notification, int size, Type type);

#endif