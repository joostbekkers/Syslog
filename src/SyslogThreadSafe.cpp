#include "SyslogThreadSafe.h"

void SyslogThreadSafeTask(void *pvParameter) {
    SyslogThreadSafe *syslog = static_cast<SyslogThreadSafe *>(pvParameter);

    while (!syslog->stop) {
        while (syslog->messageQueue.empty()) {
            ulTaskNotifyTake(pdTRUE,pdMS_TO_TICKS(100));
        }
        SyslogMessage message;
        {
            std::lock_guard<std::mutex> LockGuard(syslog->mutex);
            if (syslog->messageQueue.empty())
                continue;
            message = std::move(syslog->messageQueue.front());
            syslog->messageQueue.pop();
        }
        bool messageWasSent = false;
        uint8_t attempts = 3;
        while (~messageWasSent && attempts--) {
            messageWasSent = syslog->_sendLog(message.pri,message.message.c_str());
            if (!messageWasSent)
                vTaskDelay(1);
        }
    }
    syslog->senderLoop = nullptr;
    vTaskDelete( NULL );
}

void SyslogThreadSafe::begin() {
    if (senderLoop) return;
    xTaskCreate(SyslogThreadSafeTask,"Syslog",2400,(void *) this, 1, &senderLoop);
}

void SyslogThreadSafe::end() {
    stop = true;
    while (senderLoop) {
        vTaskDelay(1);
    }
}

bool SyslogThreadSafe::_sendLog(uint16_t pri, const char *message) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        SyslogMessage msg{pri,message};

        messageQueue.push(msg);
    }
    if (senderLoop) {
       xTaskNotifyGive(senderLoop);
    }
    return true;
}


bool SyslogThreadSafe::_sendLog(uint16_t pri, const __FlashStringHelper *message) {
    // don't need this, too complicated
    return true;
}
