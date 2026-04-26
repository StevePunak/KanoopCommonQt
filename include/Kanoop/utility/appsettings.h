/******************************************************************************************
**
** appsettings.h
**
** Application-wide settings storage for non-GUI and GUI applications.
** Parent class of GuiSettings (KanoopGuiQt).
**
** Author:  Stephen Punak
** Created: Sun Apr 26 2026
**
******************************************************************************************/
#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#include <QObject>
#include <QSettings>
#include <QUuid>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Persistent application settings storage backed by QSettings.
 *
 * AppSettings provides typed, named accessors for arbitrary key/value persistence
 * and last-used-directory tracking. It has no Qt GUI dependencies and is suitable
 * as a base class for non-GUI applications. Subclass to add application-specific
 * settings; for GUI applications, subclass GuiSettings (in KanoopGuiQt) instead.
 *
 * A process-wide singleton can be registered with setGlobalInstance() and retrieved
 * with globalInstance().
 */
class KANOOP_EXPORT AppSettings : public QObject
{
    Q_OBJECT

public:
    /** @brief Construct an AppSettings object backed by the default QSettings store. */
    AppSettings();

    /**
     * @brief Persist an arbitrary string value.
     * @param key Settings key
     * @param value String to save
     */
    void setStringValue(const QString& key, const QString& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary string value.
     * @param key Settings key
     * @return Saved string, or empty string if not found
     */
    QString getStringValue(const QString& key) const { return _settings.value(key).toString(); }

    /**
     * @brief Persist an arbitrary QVariant value.
     * @param key Settings key
     * @param value Variant to save
     */
    void setVariantValue(const QString& key, const QVariant& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary QVariant value.
     * @param key Settings key
     * @return Saved variant, or invalid QVariant if not found
     */
    QVariant getVariantValue(const QString& key) const { return _settings.value(key); }

    /**
     * @brief Persist an arbitrary byte array value.
     * @param key Settings key
     * @param value Byte array to save
     */
    void setByteArrayValue(const QString& key, const QByteArray& value) { _settings.setValue(key, value); }

    /**
     * @brief Retrieve an arbitrary byte array value.
     * @param key Settings key
     * @return Saved byte array, or empty array if not found
     */
    QByteArray getByteArrayValue(const QString& key) const { return _settings.value(key).toByteArray(); }

    /**
     * @brief Return the maximum number of recent files to track.
     * @return Maximum recent file count
     */
    int maxRecentFiles() const { return _maxRecentFiles; }

    /**
     * @brief Set the maximum number of recent files to track.
     * @param value Maximum recent file count
     */
    void setMaxRecentFiles(int value) { _maxRecentFiles = value; }

    /**
     * @brief Return the last directory used for a given file extension.
     * @param extension File extension (without leading dot)
     * @return Last directory path string
     */
    QString lastDirectory(const QString& extension) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, extension)).toString(); }

    /**
     * @brief Return the last directory used for a given file-type integer.
     * @param fileType Application-defined file type integer
     * @return Last directory path string
     */
    QString lastDirectory(int fileType) const { return _settings.value(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType)).toString(); }

    /**
     * @brief Save the last directory used for a given file extension.
     * @param extension File extension (without leading dot)
     * @param value Directory path to save
     */
    virtual void saveLastDirectory(const QString& extension, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, extension), value); }

    /**
     * @brief Save the last directory used for a given file-type integer.
     * @param fileType Application-defined file type integer
     * @param value Directory path to save
     */
    virtual void saveLastDirectory(int fileType, const QString& value) { _settings.setValue(makeFileTypeKey(KEY_LAST_DIRECTORY, fileType), value); }

    /**
     * @brief Synchronize settings file to disk and reload any changed values.
     */
    void sync();

    /**
     * @brief Return the process-wide AppSettings singleton.
     * @return Global instance, or nullptr if not set.
     */
    static AppSettings* globalInstance() { return _globalInstance; }

    /**
     * @brief Set the process-wide AppSettings singleton.
     * @param value Instance to register as global
     */
    static void setGlobalInstance(AppSettings* value) { _globalInstance = value; }

signals:
    /** @brief Emitted when any setting value changes. */
    void settingsChanged();

protected:
    /** @brief Build a settings key prefixed with the application key. */
    static QString makeStandardKey(const QString& key) { return QString("%1/%2").arg(KEY_APP).arg(key); }
    /** @brief Build a compound settings key from two parts. */
    static QString makeKey(const QString& key, const QString& subKey) { return QString("%1/%2").arg(key).arg(subKey); }
    /** @brief Build a settings key based on a QObject's identity. */
    static QString makeObjectKey(const QObject* object);
    /** @brief Build a settings key based on a file extension. */
    static QString makeFileTypeKey(const QString& key, const QString& extension);
    /** @brief Build a settings key based on a file-type integer. */
    static QString makeFileTypeKey(const QString& key, int fileType);
    /** @brief Build a compound settings key from a base key and a QObject. */
    static QString makeCompoundObjectKey(const QString& key, const QObject* object);

    /** @brief Override to ensure sane default values on first run. */
    virtual void ensureValidDefaults() {}

    /** @brief Underlying QSettings storage. */
    QSettings _settings;

    /** @brief Settings key for the application group. */
    static const QString KEY_APP;
    /** @brief Settings key for the last-used directory. */
    static const QString KEY_LAST_DIRECTORY;

private:
    int _maxRecentFiles;

    static AppSettings* _globalInstance;
};

#endif // APPSETTINGS_H
