#ifndef TREEPATHVERTICE_H
#define TREEPATHVERTICE_H

#include <QMap>
#include <QPointF>
#include <QString>
#include "Kanoop/kanoopcommon.h"

class SpanningTree;
class KANOOP_EXPORT TreePathVertice
{
public:
    enum VerticeType
    {
        Standard,
        AdHocOrigin,
        AdHocDestination,
        AdHocOriginProximal,
        AdHocDestinationProximal,
    };

    enum VerticeState
    {
        Unvisited,
        Visited
    };

    TreePathVertice(const QPointF &position, VerticeType type, SpanningTree* parent);

    class Map : public QMap<QString, TreePathVertice*>
    {
    };

    class List : public QList<TreePathVertice*>
    {
    public:
        List() :
            QList<TreePathVertice*>() {}
        List(const QList<TreePathVertice*> source) {
            append(source);
        }
    };

    void tryAddNeighbor(TreePathVertice* neighbor);
    void tryReplaceNeighbor(TreePathVertice* neighbor, TreePathVertice* newNeighbor);

    VerticeType type() const { return _type; }
    QPointF position() const { return _position; }

    double distance() const { return _distance; }
    void setDistance(double value) { _distance = value; }

    VerticeState state() const { return _state; }
    void setState(VerticeState value) { _state = value; }

    QString name() const { return _name; }
    void setName(const QString& value) { _name = value; }

    TreePathVertice* source() const { return _source; }
    void setSource(TreePathVertice* value) { _source = value; }

    Map neighbors() const { return _neighbors; }

    QString hashName() const { return _hashName; }
    QString toString() const { return _name; }

    static QString makeHashName(const QPointF &point);

private:
    VerticeType _type;
    QPointF _position;
    double _distance;
    VerticeState _state;
    QString _name;
    QString _hashName;
    SpanningTree* _spanningTree;

    TreePathVertice* _source;
    Map _neighbors;
};

#endif // TREEPATHVERTICE_H
