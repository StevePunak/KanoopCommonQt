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

/**
 * @brief Core KanoopCommonQt namespace containing shared enumerations and templates.
 */
namespace KANOOP {

/**
 * @brief Model roles extending Qt::UserRole for use in AbstractItemModel subclasses.
 */
enum ModelRole
{
    DataRole = Qt::UserRole + 0x1000,   ///< Primary data payload role
    EntityTypeRole,                      ///< Role carrying an entity type integer
    UUidRole,                            ///< Role carrying a QUuid
    EntityMetadataRole,                  ///< Role carrying an EntityMetadata object
    MetadataTypeRole,                    ///< Role carrying a metadata type integer
    NameRole,                            ///< Role carrying a display name string

    UserRole = DataRole + 0x8000,        ///< First role available for application-specific use
};

/**
 * @brief A QMap subclass providing bidirectional enum-to-string lookups.
 *
 * @tparam T Enumeration type used as the map key
 */
template <class T>
class EnumToStringMap : public QMap<T, QString>
{
public:
    /**
     * @brief Look up the string name for an enum value.
     * @param type Enum value to look up
     * @param defaultValue Value to return if not found
     * @return Registered string name, or defaultValue
     */
    QString getString(T type, const QString& defaultValue = QString()) const
    {
        auto it = this->find(type);
        return it != this->end()
                   ? it.value()
                   : defaultValue;
    }

    /**
     * @brief Look up the enum value for a string name, with case sensitivity control.
     * @param name String to look up
     * @param caseSensitivity Qt::CaseSensitive or Qt::CaseInsensitive
     * @param defaultValue Value returned when no match is found
     * @return Matching enum value, or defaultValue
     */
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

    /**
     * @brief Look up the enum value for a string name using case-sensitive comparison.
     * @param name String to look up
     * @param defaultValue Value returned when no match is found
     * @return Matching enum value, or defaultValue
     */
    T getType(const QString& name, const T defaultValue = T()) const
    {
        return getType(name, Qt::CaseSensitive, defaultValue);
    }

    /**
     * @brief Return all enum values registered in this map.
     * @return List of all registered enum values
     */
    QList<T> getTypes() const
    {
        QList<T> result = EnumToStringMap<T>::keys();
        return result;
    }

    /**
     * @brief Test whether the map contains a given string value.
     * @param name String to search for
     * @param cs Case sensitivity flag (default Qt::CaseInsensitive)
     * @return true if name matches any registered string value
     */
    bool containsString(const QString& name, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const
    {
        typename EnumToStringMap<T>::const_iterator it = std::find_if(EnumToStringMap<T>::constBegin(), EnumToStringMap<T>::constEnd(), [name, cs](const QString& a)
        {
            return a.compare(name, cs) == 0;
        });
        if(it != EnumToStringMap<T>::constEnd()) {
            return true;
        }
        return false;
    }

};

}   // namespace

#endif // KANOOPCOMMON_H
