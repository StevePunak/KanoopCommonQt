#include "treepathvertice.h"

#include <float.h>

TreePathVertice::TreePathVertice(const QPointF& position, VerticeType type, SpanningTree *parent) :
    _type(type),
    _position(position),
    _distance(INFINITY),
    _state(Unvisited),
    _name(makeHashName(position)),
    _spanningTree(parent),
    _source(nullptr)
{
    _hashName = makeHashName(position);
}

void TreePathVertice::tryAddNeighbor(TreePathVertice *neighbor)
{
    if(_neighbors.contains(neighbor->_hashName) == false) {
        _neighbors.insert(neighbor->_hashName, neighbor);
    }
}

void TreePathVertice::tryReplaceNeighbor(TreePathVertice *neighbor, TreePathVertice *newNeighbor)
{
    if(_neighbors.contains(neighbor->_hashName) == true) {
        _neighbors.remove(neighbor->_hashName);
    }
    _neighbors.insert(neighbor->_hashName, newNeighbor);
}

QString TreePathVertice::makeHashName(const QPointF& point)
{
    return QString("%1, %2").arg(point.x()).arg(point.y());
}
