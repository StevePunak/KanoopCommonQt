/**
 * @brief A Dijkstra-based spanning tree for finding shortest paths through a line network.
 */
#ifndef SPANNINGTREE_H
#define SPANNINGTREE_H
#include "treepathvertice.h"
#include "line.h"

/**
 * @brief Computes the shortest path through a network of Line segments using Dijkstra's algorithm.
 *
 * Construct with a Line::List representing the navigable network, set the origin and
 * destination points, then call computePath() to retrieve the ordered list of lines
 * that form the shortest route.
 */
class KANOOP_EXPORT SpanningTree
{
public:
    /** @brief Default constructor — creates an empty, invalid spanning tree. */
    SpanningTree() :
        _origin(nullptr), _destination(nullptr) {}

    /**
     * @brief Construct a SpanningTree from a network of line segments.
     * @param lines Line segments forming the navigable graph
     */
    SpanningTree(const Line::List& lines);

    /** @brief Destructor — frees all dynamically allocated vertex objects. */
    virtual ~SpanningTree();

    /**
     * @brief Compute the shortest path from origin to destination through the line network.
     * @return Ordered list of lines forming the path, or an empty list if no path exists
     */
    Line::List computePath();

    /**
     * @brief Set the origin point from which to route.
     * @param origin Starting point in the line network's coordinate space
     * @param preferredDirection Initial preferred travel direction (default NoDirection)
     */
    void setOrigin(const QPointF& origin, Geo::Direction preferredDirection = Geo::NoDirection);

    /**
     * @brief Set the destination point to route towards.
     * @param destination Target point in the line network's coordinate space
     */
    void setDestination(const QPointF& destination);

    /**
     * @brief Return all vertices in the spanning tree.
     * @return List of all TreePathVertice objects in the graph
     */
    TreePathVertice::List vertices() const
    {
        return TreePathVertice::List(_vertices.values());
    }

    /**
     * @brief Return true if the spanning tree contains at least one line segment.
     * @return true if the line network is non-empty
     */
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
