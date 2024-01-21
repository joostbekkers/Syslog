#ifndef SYSLOGTHREADSAFE_H
#define SYSLOGTHREADSAFE_H 

#include <mutex>
#include <queue>
#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Syslog.h"

struct SyslogMessage {
    uint16_t pri;
    std::string message;
};

class SyslogThreadSafe: public Syslog {
    protected:
        std::mutex mutex;
        std::queue<SyslogMessage> messageQueue;
        TaskHandle_t senderLoop = nullptr;
        bool stop;

        bool _sendLog(uint16_t pri, const char *message);
        bool _sendLog(uint16_t pri, const __FlashStringHelper *message);

        void begin();
        void end();

        friend void SyslogThreadSafeTask(void *pvParameter);
    public:
        SyslogThreadSafe(UDP &client, uint8_t protocol = SYSLOG_PROTO_IETF):
            Syslog(client,protocol) { begin(); };
        SyslogThreadSafe(UDP &client, const char* server, uint16_t port, const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN, uint8_t protocol = SYSLOG_PROTO_IETF):
            Syslog(client,server,port,deviceHostname,appName,priDefault,protocol) { begin(); }
        SyslogThreadSafe(UDP &client, IPAddress ip, uint16_t port, const char* deviceHostname = SYSLOG_NILVALUE, const char* appName = SYSLOG_NILVALUE, uint16_t priDefault = LOG_KERN, uint8_t protocol = SYSLOG_PROTO_IETF):
            Syslog(client,ip,port,deviceHostname,appName,priDefault,protocol) { begin(); }

        ~SyslogThreadSafe() { end(); }

};

#endif