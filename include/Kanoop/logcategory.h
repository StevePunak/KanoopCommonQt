/******************************************************************************************
**
** logcategory.h
**
** Provide categorized logging for use with the default Log object.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#ifndef LOGCATEGORY_H
#define LOGCATEGORY_H

#include <QString>
#include "loggingtypes.h"
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Logging subsystem namespace.
 */
namespace Log
{

/**
 * @brief A named logging category with an associated minimum log level.
 *
 * Categories allow fine-grained control over which log messages are emitted.
 * Each category has a name and a level; messages logged below the category level
 * are suppressed even if they would otherwise pass the global logger level.
 */
class KANOOP_EXPORT LogCategory
{
public:
    /** @brief Default constructor — creates an invalid (unnamed) category at the default level. */
    LogCategory() : _level(DefaultLogLevel) {}

    /**
     * @brief Construct a named category at the default log level.
     * @param name Category name string
     */
    LogCategory(const QString& name) :
        _name(name),
        _level(DefaultLogLevel) {}

    /**
     * @brief Return the category name.
     * @return Category name string
     */
    QString name() const { return _name; }

    /**
     * @brief Return the minimum log level for this category.
     * @return Active LogLevel threshold for this category
     */
    LogLevel level() const { return _level; }

    /**
     * @brief Create a new category by appending a sub-name to this category's name.
     * @param name Sub-name to append (separated by '.')
     * @return New LogCategory with the combined name
     */
    LogCategory append(const QString& name);

    /**
     * @brief Create a new category by prepending a prefix to this category's name.
     * @param name Prefix to prepend (separated by '.')
     * @return New LogCategory with the combined name
     */
    LogCategory prepend(const QString& name);

    /**
     * @brief Extract the parent portion of a dotted category name.
     * @param name Full dotted category name
     * @return Parent name (everything before the last '.')
     */
    static QString parentName(const QString& name);

    /**
     * @brief Return the process-wide default log level used for new categories.
     * @return Current default LogLevel
     */
    static LogLevel defaultLogLevel() { return DefaultLogLevel; }

    /**
     * @brief Set the process-wide default log level used for new categories.
     * @param value New default LogLevel
     */
    static void setDefaultLogLevel(LogLevel value) { DefaultLogLevel = value; }

    /**
     * @brief Test whether this category has a non-empty name.
     * @return true if the name is non-empty
     */
    bool isValid() const { return _name.length() != 0; }

    /** @brief Process-wide default log level applied to newly created categories. */
    static LogLevel DefaultLogLevel;

protected:
    QString _name;
    LogLevel _level;
};

} // namespace Log

#endif // LOGCATEGORY_H
