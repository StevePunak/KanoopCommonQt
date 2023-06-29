#include "spanningtree.h"
#include "commonexception.h"
#include "flatgeo.h"

#include <klog.h>

#include "geo/geotypes.h"

using namespace Geo;

SpanningTree::SpanningTree(const Line::List &lines) :
    _lines(lines),
    _origin(nullptr), _destination(nullptr),
    _debugLevel(10)
{
    initializeFromLines();
}

SpanningTree::~SpanningTree()
{
    qDeleteAll(_vertices);
}

void SpanningTree::initializeFromLines()
{
    // Create a vertice object for each line endpoint
    for(const Line& line : _lines) {
        TreePathVertice* v1 = _vertices.value(TreePathVertice::makeHashName(line.p1()), nullptr);
        if(v1 == nullptr) {
            v1 = new TreePathVertice(line.p1(), TreePathVertice::Standard, this);
            _vertices.insert(v1->hashName(), v1);
        }

        TreePathVertice* v2 = _vertices.value(TreePathVertice::makeHashName(line.p2()), nullptr);
        if(v2 == nullptr) {
            v2 = new TreePathVertice(line.p2(), TreePathVertice::Standard, this);
            _vertices.insert(v2->hashName(), v2);
        }

        // add these two as neighbors to each other
        v1->tryAddNeighbor(v2);
        v2->tryAddNeighbor(v1);
    }
}

Line::List SpanningTree::computePath()
{
    Line::List result;
    try
    {
        if(_origin == nullptr) {
            throw CommonException("No origin set");
        }
        if(_destination == nullptr) {
            throw CommonException("No destination set");
        }

        initializeVertices();

        while(cycle()) { }
        result = getPath();
    }
    catch(const CommonException& e)
    {
        debugLog(KLOG_WARNING, e.message());
    }
    return result;
}

void SpanningTree::setOrigin(const QPointF &origin, Direction preferredDirection)
{
    try
    {
        TreePathVertice* result = addAdHocVertice(origin, TreePathVertice::AdHocOrigin, preferredDirection);
        _origin = result;
    }
    catch(CommonException& e)
    {
        debugLog(KLOG_WARNING, e.message());
    }
}

void SpanningTree::setDestination(const QPointF &destination)
{
    try
    {
        TreePathVertice* result = addAdHocVertice(destination, TreePathVertice::AdHocDestination);
        _destination = result;
    }
    catch(CommonException& e)
    {
        debugLog(KLOG_WARNING, e.message());
    }
}

void SpanningTree::initializeVertices()
{
    /** create empty list of visited vertices */
    _visited.clear();

    /** create list of all vertices which we will remove from as we visit */
    _unvisited.clear();
    _unvisited.append(_vertices.values());

    /**
     * Set all initial values for each vertice...
     * origin distance to zero, and all others to infinity
     * state and source
     */
    TreePathVertice::List vertices = _vertices.values();
    for(TreePathVertice* vertice : vertices) {
        if(vertice == _origin) {
            debugLog(KLOG_DEBUG, QString("Set origin vertice to %1").arg(vertice->toString()));
            vertice->setDistance(0);
        }
        else {
            debugLog(KLOG_DEBUG, QString("Set other vertice to %1").arg(vertice->toString()));
            vertice->setDistance(INFINITY);
        }

        vertice->setState(TreePathVertice::Unvisited);
        vertice->setSource(nullptr);
    }
}

TreePathVertice* SpanningTree::addAdHocVertice(const QPointF point, TreePathVertice::VerticeType type, Direction preferredDirection)
{
    TreePathVertice *V1, *V2;
    if((V1 =_vertices.value(TreePathVertice::makeHashName(point))) == nullptr) {
        /**
         * Having ensured that our vertice does not already exist,
         * we are either adding an origin or destination.
         *
         * we need to add the vertice itself (V1), as well as an additional one (V2) on the closest
         * line segment (L) to the new vertice.
         *
         * The vertices at the end of (L) will have each other as neighbors. Since we are
         * essentially splitting (L) into two segments,
         */

        /** clear any that exist of the given type */
        clearAdHocVertices(type);

        /** create our new vertice */
        V1 = new TreePathVertice(point, type, this);
        _vertices.insert(V1->hashName(), V1);

        /** find closest point on the closest line for a second vertice */
        double closestDistance;
        Line L;
        QPointF closestPoint = findClosestPointInLines(point, L, closestDistance, preferredDirection);
        debugLog(KLOG_DEBUG, QString("Added ad-hoc vertice of type %1 at point %2 closest to line [[%3]] at %4 pixels")
                                 .arg(type)
                                 .arg(Geo::string(point))
                                 .arg(L.toString())
                                 .arg(closestDistance));
        if(closestPoint.isNull() == false) {
            /** if the closest vertice doesn't exist yet, we will create it, and split the line L */
            if((V2 = _vertices.value(TreePathVertice::makeHashName(closestPoint))) == nullptr) {
                /** get the vertices on the line we are about to split */
                TreePathVertice* LV1, *LV2;
                if((LV1 = _vertices.value(TreePathVertice::makeHashName(L.p1()))) == nullptr ||
                    (LV2 = _vertices.value(TreePathVertice::makeHashName(L.p2()))) == nullptr) {
                    throw CommonException("Vertices on original lines should exist");
                }

                /** create our new vertice on the split line */
                V2 = new TreePathVertice(closestPoint, type, this);
                _vertices.insert(V2->hashName(), V2);

                /** fix up neighbors */
                LV1->tryReplaceNeighbor(LV2, V2);
                LV2->tryReplaceNeighbor(LV1, V2);

                V2->tryAddNeighbor(LV1);
                V2->tryAddNeighbor(LV2);
                V2->tryAddNeighbor(V1);

                V1->tryAddNeighbor(V2);

                /** replace L with two new lines L1 and L2 */
                Line l1(L.p1(), closestPoint);
                Line l2(closestPoint, L.p2());

                _lines.removeOne(L);
                _lines.append(l1);
                _lines.append(l2);
            }
            else {
                V1->tryAddNeighbor(V2);
                V2->tryAddNeighbor(V1);
            }
        }
    }
    return V1;
}

void SpanningTree::clearAdHocVertices(TreePathVertice::VerticeType type)
{
    QStringList deleteList;
    for(TreePathVertice::Map::const_iterator it = _vertices.constBegin();it != _vertices.constEnd();it++) {
        if(it.value()->type() == type) {
            deleteList.append(it.key());
        }
    }

    for(const QString& key : deleteList) {
        _vertices.remove(key);
    }
}

QPointF SpanningTree::findClosestPointInLines(const QPointF &point, Line &resultLine, double &closestDistance, Direction preferredDirection)
{
    closestDistance = INFINITY;
    QPointF closestPoint;

    for(const Line& line : _lines) {
        double	distance;
        QPointF	p = line.closestPointTo(point, distance);
        if(distance < closestDistance) {
            bool success = false;
            switch(preferredDirection)
            {
            case Up:
                success = FlatGeo::isPointAbove(p, point);
                break;
            case Down:
                success = FlatGeo::isPointBelow(p, point);
                break;
            case ToLeft:
                success = FlatGeo::isPointLeftOf(p, point);
                break;
            case ToRight:
                success = FlatGeo::isPointRightOf(p, point);
                break;
            default:
                success = true;
                break;
            }

            if(success) {
                closestDistance = distance;
                closestPoint = p;
                resultLine = line;
            }
        }
    }
    return closestPoint;
}

bool SpanningTree::cycle()
{
    /** get the vertice with the lowest distance from the origin */
    TreePathVertice* current = nullptr;
    for(TreePathVertice* vertice : _unvisited) {
        if(current == nullptr || vertice->distance() <= current->distance()) {
            current = vertice;
        }
    }
    if(current != nullptr) {
        debugLog(KLOG_DEBUG, QString("(Cycle) Set current vertice to %1").arg(current->toString()));
    }

    if(current != nullptr && current->distance() != INFINITY) {
        /** go through each neighbor of the current */
        TreePathVertice::List neighbors = current->neighbors().values();
        for(TreePathVertice* neighbor : neighbors) {
            if(neighbor->state() == TreePathVertice::Unvisited) {
                double distance = FlatGeo::distance(current->position(), neighbor->position());
                if(current->distance() + distance < neighbor->distance()) {
                    neighbor->setDistance(current->distance() + distance);
                    neighbor->setSource(current);
                    debugLog(KLOG_DEBUG, QString("--- Set %1's neighbor %2 to distance of %3")
                                                     .arg(current->toString())
                                                     .arg(neighbor->toString())
                                                     .arg(neighbor->distance()));
                }
            }
        }

        /** move this node from visited to unvisited */
        current->setState(TreePathVertice::Visited);

        _unvisited.removeOne(current);
        _visited.append(current);
    }
    else if(current != nullptr) {
        _unvisited.removeOne(current);
        debugLog(KLOG_WARNING, QString("VERTICE %1 has no solution!").arg(current->toString()));
    }
    else {
        debugLog(KLOG_ERROR, "Should never get here!");
    }

    return _unvisited.count() > 0;
}

Line::List SpanningTree::getPath()
{
    /** now work backwards along source paths */
    QList<QPointF> path;
    TreePathVertice* vertice = _destination;
    TreePathVertice* lastInserted;
    do
    {
        path.insert(0, vertice->position());
        lastInserted = vertice;
        vertice = vertice->source();
    }while(vertice != nullptr && lastInserted != _origin);

    if(lastInserted != _origin) {
        qCritical() << "Never resolved the origin";
    }
    return Line::List::fromPoints(path);
}

void SpanningTree::debugLog(const char *file, int line, int level, const QString &text)
{
    if(level >= _debugLevel) {
        KLog::sysLogText(file, line, (KLog::LogLevel)level, text);
    }
}


