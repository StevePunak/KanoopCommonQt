/**
 * @brief A QObject-based sink that receives log entries from the logging system.
 */
#ifndef LOGCONSUMER_H
#define LOGCONSUMER_H

#include <QDateTime>
#include <QObject>
#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>
#include <Kanoop/logentry.h>

/**
 * @brief Abstract base class for objects that receive and process log entries.
 *
 * Subclass LogConsumer and implement the logEntry() signal handler, or connect
 * to the logEntry() signal, to receive all log entries delivered to this consumer.
 * Register the consumer with Log::addConsumer().
 */
class KANOOP_EXPORT LogConsumer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Construct a LogConsumer with an optional parent.
     * @param parent Optional QObject parent
     */
    explicit LogConsumer(QObject *parent = nullptr);

    /** @brief Destructor — automatically unregisters from the system logger. */
    virtual ~LogConsumer();

    /**
     * @brief Deliver a log entry to this consumer, emitting the logEntry() signal.
     * @param entry Log entry to deliver
     */
    void addLogEntry(const Log::LogEntry& entry);

signals:
    /**
     * @brief Emitted when a new log entry is delivered to this consumer.
     * @param entry The delivered log entry
     */
    void logEntry(const Log::LogEntry& entry);
};

#endif // LOGCONSUMER_H
