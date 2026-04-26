/******************************************************************************************
**
** appsettings.cpp
**
** Author:  Stephen Punak
** Created: Sun Apr 26 2026
**
******************************************************************************************/
#include <Kanoop/utility/appsettings.h>
#include <QCoreApplication>

const QString AppSettings::KEY_APP            = "app";
const QString AppSettings::KEY_LAST_DIRECTORY = "last_dir";

AppSettings* AppSettings::_globalInstance = nullptr;

AppSettings::AppSettings() :
    _settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::instance()->organizationName(), QCoreApplication::instance()->applicationName()),
    _maxRecentFiles(10)
{
}

void AppSettings::sync()
{
    _settings.sync();
}

QString AppSettings::makeObjectKey(const QObject* object)
{
    QString result = object->objectName();
    if(object->parent() != nullptr) {
        result.prepend(object->parent()->objectName());
    }
    return result;
}

QString AppSettings::makeFileTypeKey(const QString& key, const QString& extension)
{
    QString fixed = extension;
    fixed.removeIf([extension](const QChar& it) { return it.isPunct() || it.isSpace(); });
    return QString("%1/%2").arg(key, fixed);
}

QString AppSettings::makeFileTypeKey(const QString& key, int fileType)
{
    return QString("%1/%2").arg(key, QString("type_%1").arg(fileType));
}

QString AppSettings::makeCompoundObjectKey(const QString& key, const QObject* object)
{
    QString objectKey = makeKey(key, makeObjectKey(object));
    return objectKey;
}
