#include "logconsumer.h"

using namespace Log;

LogConsumer::LogConsumer(QObject *parent) :
    QObject(parent)
{
}

LogConsumer::~LogConsumer()
{
    Log::removeConsumer(this);
}

void LogConsumer::addLogEntry(const Log::LogEntry& entry)
{
    emit logEntry(entry);
}
