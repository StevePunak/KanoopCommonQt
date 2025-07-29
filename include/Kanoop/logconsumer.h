#ifndef LOGCONSUMER_H
#define LOGCONSUMER_H

#include <QDateTime>
#include <QObject>
#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>
#include <Kanoop/logentry.h>

class KANOOP_EXPORT LogConsumer : public QObject
{
    Q_OBJECT
public:
    explicit LogConsumer(QObject *parent = nullptr);
    virtual ~LogConsumer();

    void addLogEntry(const Log::LogEntry& entry);

signals:
    void logEntry(const Log::LogEntry& entry);
};

#endif // LOGCONSUMER_H
