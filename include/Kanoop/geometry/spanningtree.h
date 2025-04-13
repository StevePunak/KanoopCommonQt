#ifndef SPANNINGTREE_H
#define SPANNINGTREE_H
#include "treepathvertice.h"
#include "line.h"

class KANOOP_EXPORT SpanningTree
{
public:
    SpanningTree() :
        _origin(nullptr), _destination(nullptr) {}
    SpanningTree(const Line::List& lines);
    virtual ~SpanningTree();

    Line::List computePath();

    void setOrigin(const QPointF& origin, Geo::Direction preferredDirection = Geo::NoDirection);
    void setDestination(const QPointF& destination);

    TreePathVertice::List vertices() const
    {
        return TreePathVertice::List(_vertices.values());
    }

    bool isValid() const { return _lines.count() > 0; }

private:
    void initializeFromLines();
    void initializeVertices();

    TreePathVertice *addAdHocVertice(const QPointF point, TreePathVertice::VerticeType type, Geo::Direction preferredDirection = Geo::NoDirection);
    void clearAdHocVertices(TreePathVertice::VerticeType type);
    QPointF findClosestPointInLines(const QPointF& point, Line& resultLine, double& closestDistance, Geo::Direction preferredDirection = Geo::NoDirection);

    bool cycle();
    Line::List getPath();

    void debugLog(const char* file, int line, int level, const QString& text);

    Line::List _lines;

    TreePathVertice::Map _vertices;
    TreePathVertice::List _visited;
    TreePathVertice::List _unvisited;
    TreePathVertice* _origin;
    TreePathVertice* _destination;

    int _debugLevel;
};

#endif // SPANNINGTREE_H
