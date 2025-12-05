/**
 *  KanoopCommonQt
 *
 *  A Qt port of the KanoopCommon C# library
 *
 *  Lots of useful classes to provide helper functions to Qt
 *
 *  Stephen Punak, July 04 2019
 */
#ifndef KANOOPCOMMON_H
#define KANOOPCOMMON_H
#include <QtCore/qglobal.h>
#include <QMap>
#include <QString>

#if defined(KANOOP_LIBRARY)
#  define KANOOP_EXPORT Q_DECL_EXPORT
#else
#  define KANOOP_EXPORT Q_DECL_IMPORT
#endif

namespace KANOOP {

enum ModelRole
{
    DataRole = Qt::UserRole + 0x1000,
    EntityTypeRole,
    UUidRole,
    EntityMetadataRole,
    MetadataTypeRole,
    NameRole,

    UserRole = DataRole + 0x8000,
};

template <class T>
class EnumToStringMap : public QMap<T, QString>
{
public:
    QString getString(T type, const QString& defaultValue = QString()) const
    {
        auto it = this->find(type);
        return it != this->end()
                   ? it.value()
                   : defaultValue;
    }

    T getType(const QString& name, Qt::CaseSensitivity caseSensitivity, const T defaultValue = T()) const
    {
        bool ok;
        int enumValue = name.toInt(&ok);
        if(ok) {
            return (T)enumValue;
        }

        for(auto it = EnumToStringMap<T>::constBegin();it != EnumToStringMap<T>::constEnd();it++) {
            QString value = it.value();
            if(QString::compare(value, name, caseSensitivity) == 0) {
                return it.key();
            }
        }
        return defaultValue;
    }

    T getType(const QString& name, const T defaultValue = T()) const
    {
        return getType(name, Qt::CaseSensitive, defaultValue);
    }

    QList<T> getTypes() const
    {
        QList<T> result = EnumToStringMap<T>::keys();
        return result;
    }

    bool containsString(const QString& name, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const
    {
        typename EnumToStringMap<T>::const_iterator it = std::find_if(EnumToStringMap<T>::constBegin(), EnumToStringMap<T>::constEnd(), [name, cs](const QString& a)
        {
            return a.compare(name, a, cs) == 0;
        });
        if(it != EnumToStringMap<T>::constEnd()) {
            return true;
        }
        return false;
    }

};

}   // namespace

#endif // KANOOPCOMMON_H
