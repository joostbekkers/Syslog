#ifndef SYSLOGTHREADSAFE_H
#define SYSLOGTHREADSAFE_H 

#include "Syslog.h"
#include <mutex>

class SyslogThreadSafe: public Syslog {
    protected:
        std::mutex mutex;

        bool _sendLog(uint16_t pri, const char *message);
        bool _sendLog(uint16_t pri, const __FlashStringHelper *message);

    public:
        using Syslog::Syslog;
};

#endif