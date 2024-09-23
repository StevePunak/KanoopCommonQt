#ifndef ENTITYMETADATAINFO_H
#define ENTITYMETADATAINFO_H
#include <QString>

class EntityMetadataInfo
{
public:
    EntityMetadataInfo() :
        _id(0), _iconId(0) {}

    EntityMetadataInfo(int id, const QString& name) :
        _id(id), _name(name), _iconId(0) {}

    int id() const { return _id; }
    QString name() const { return _name; }

    int iconId() const { return _iconId; }
    void setIconId(int value) { _iconId = value; }

    bool isValid() const { return _id != 0; }

private:
    int _id;
    QString _name;
    int _iconId;
};

#endif // ENTITYMETADATAINFO_H
