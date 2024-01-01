#include "SyslogThreadSafe.h"

bool SyslogThreadSafe::_sendLog(uint16_t pri, const char *message) {
    std::lock_guard<std::mutex> lock(mutex);
    return Syslog::_sendLog(pri,message);
}
bool SyslogThreadSafe::_sendLog(uint16_t pri, const __FlashStringHelper *message) {
    std::lock_guard<std::mutex> lock(mutex);
    return Syslog::_sendLog(pri,message);
}