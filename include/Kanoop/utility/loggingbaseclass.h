/**
 * @brief A non-QObject mixin that provides structured logging methods to any class.
 */
#ifndef LOGGINGBASECLASS_H
#define LOGGINGBASECLASS_H

#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>
#include <Kanoop/logconsumer.h>
#include <Kanoop/kanoopcommon.h>

/** @brief Convenience macro that logs a warning for an unimplemented method. */
#define UNIMPLEMENTED  logText(LVL_WARNING, QString("---------------- %1 UNIMPLEMENTED  ----------------").arg(__FUNCTION__));

/**
 * @brief Mixin base class providing categorized logging at four nesting levels.
 *
 * Classes that inherit LoggingBaseClass gain logText() and logHex() methods that
 * automatically tag messages with the registered log category. The category can
 * be refined into up to three sub-levels (LVL1, LVL2, LVL3) for more granular
 * filtering.
 */
class KANOOP_EXPORT LoggingBaseClass
{
public:
    /**
     * @brief Construct with an optional log category.
     * @param category Base log category (default: uncategorized)
     */
    LoggingBaseClass(const Log::LogCategory& category = Log::LogCategory());

    /**
     * @brief Construct with a category name string.
     * @param category Category name string
     */
    LoggingBaseClass(const QString& category);

    /**
     * @brief Return the base log category.
     * @return Base LogCategory
     */
    Log::LogCategory logCategory() const { return _baseCategory; }

protected:
    /** @brief Return the base (level 0) log category.
     * @return Base log category. */
    Log::LogCategory LVL0() const { return _baseCategory; }
    /** @brief Return the level-1 sub-category.
     * @return Level-1 log category. */
    Log::LogCategory LVL1() const { return _level1; }
    /** @brief Return the level-2 sub-category.
     * @return Level-2 log category. */
    Log::LogCategory LVL2() const { return _level2; }
    /** @brief Return the level-3 sub-category.
     * @return Level-3 log category. */
    Log::LogCategory LVL3() const { return _level3; }

    /**
     * @brief Write a text message to the system logger using the base category.
     * @param file Source filename (use __FILE__ / LVL_* macros)
     * @param lineNumber Source line number
     * @param level Severity level
     * @param text Message text
     */
    void logText(const char* file, int lineNumber, Log::LogLevel level, const QString& text) const;

    /**
     * @brief Write a text message to the system logger using an explicit category.
     * @param file Source filename
     * @param lineNumber Source line number
     * @param level Severity level
     * @param category Explicit log category
     * @param text Message text
     */
    void logText(const char* file, int lineNumber, Log::LogLevel level, const Log::LogCategory& category, const QString& text) const;

    /**
     * @brief Write a hex dump to the system logger.
     * @param file Source filename
     * @param lineNumber Source line number
     * @param level Severity level
     * @param data Bytes to dump
     * @param tag Optional label
     */
    void logHex(const char* file, int lineNumber, Log::LogLevel level, const QByteArray& data, const QString& tag = QString()) const;

    /**
     * @brief Write a hex dump to the system logger using an explicit category.
     * @param file Source filename
     * @param lineNumber Source line number
     * @param level Severity level
     * @param category Explicit log category
     * @param data Bytes to dump
     * @param tag Optional label
     */
    void logHex(const char* file, int lineNumber, Log::LogLevel level, const Log::LogCategory& category, const QByteArray& data, const QString& tag = QString()) const;

    /**
     * @brief Register an external log consumer to receive entries from this class.
     * @param consumer Consumer to add
     */
    void addLogConsumer(LogConsumer* consumer);

    /**
     * @brief Remove a previously added log consumer.
     * @param consumer Consumer to remove
     */
    void removeLogConsumer(LogConsumer* consumer);

private:
    /** @brief Shared initialisation for all constructors. */
    void commonInit();

    Log::LogCategory _baseCategory;
    Log::LogCategory _level1;
    Log::LogCategory _level2;
    Log::LogCategory _level3;
};

#endif // LOGGINGBASECLASS_H
