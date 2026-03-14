/**
 * @brief A vertex in a spanning-tree path graph used by PathRouter.
 */
#ifndef TREEPATHVERTICE_H
#define TREEPATHVERTICE_H

#include <QMap>
#include <QPointF>
#include <QString>
#include "Kanoop/kanoopcommon.h"

class SpanningTree;

/**
 * @brief Represents a single vertex in the spanning-tree graph used by PathRouter.
 *
 * Each vertice holds a 2D position, a type (standard, ad-hoc origin/destination, or proximal),
 * a visitation state for graph traversal, and a map of neighbouring vertices.
 */
class KANOOP_EXPORT TreePathVertice
{
public:
    /**
     * @brief Classifies a vertice's role in the path graph.
     */
    enum VerticeType
    {
        Standard,                  ///< Ordinary obstacle-corner vertex
        AdHocOrigin,               ///< Synthetic origin vertex
        AdHocDestination,          ///< Synthetic destination vertex
        AdHocOriginProximal,       ///< Vertex proximal to the origin
        AdHocDestinationProximal,  ///< Vertex proximal to the destination
    };

    /**
     * @brief Tracks whether this vertex has been visited during traversal.
     */
    enum VerticeState
    {
        Unvisited,   ///< Not yet visited
        Visited      ///< Already processed by the traversal algorithm
    };

    /**
     * @brief Construct a vertice at the given position with a type and parent tree.
     * @param position 2D position of this vertex
     * @param type Role of this vertex in the path graph
     * @param parent Owning SpanningTree
     */
    TreePathVertice(const QPointF &position, VerticeType type, SpanningTree* parent);

    /** @brief A map from hash-name string to TreePathVertice pointer. */
    class Map : public QMap<QString, TreePathVertice*>
    {
    };

    /** @brief A list of TreePathVertice pointers. */
    class List : public QList<TreePathVertice*>
    {
    public:
        /** @brief Default constructor. */
        List() :
            QList<TreePathVertice*>() {}
        /** @brief Construct from a QList of TreePathVertice pointers.
         * @param source Source list to copy */
        List(const QList<TreePathVertice*> source) {
            append(source);
        }
    };

    /**
     * @brief Add a neighbour if not already present.
     * @param neighbor Vertice to add as a neighbour
     */
    void tryAddNeighbor(TreePathVertice* neighbor);

    /**
     * @brief Replace an existing neighbour with a different vertice.
     * @param neighbor Neighbour to replace
     * @param newNeighbor Replacement vertice
     */
    void tryReplaceNeighbor(TreePathVertice* neighbor, TreePathVertice* newNeighbor);

    /**
     * @brief Return the role of this vertex.
     * @return VerticeType value
     */
    VerticeType type() const { return _type; }

    /**
     * @brief Return the 2D position of this vertex.
     * @return Position as a QPointF
     */
    QPointF position() const { return _position; }

    /**
     * @brief Return the accumulated path distance to this vertex.
     * @return Distance value used by shortest-path traversal
     */
    double distance() const { return _distance; }

    /**
     * @brief Set the accumulated path distance.
     * @param value New distance value
     */
    void setDistance(double value) { _distance = value; }

    /**
     * @brief Return the visitation state.
     * @return VerticeState value
     */
    VerticeState state() const { return _state; }

    /**
     * @brief Set the visitation state.
     * @param value New VerticeState
     */
    void setState(VerticeState value) { _state = value; }

    /**
     * @brief Return the human-readable name of this vertex.
     * @return Name string
     */
    QString name() const { return _name; }

    /**
     * @brief Set the human-readable name of this vertex.
     * @param value New name string
     */
    void setName(const QString& value) { _name = value; }

    /**
     * @brief Return the predecessor vertex in the shortest path.
     * @return Pointer to the source vertex, or nullptr if this is the origin
     */
    TreePathVertice* source() const { return _source; }

    /**
     * @brief Set the predecessor vertex in the shortest path.
     * @param value Pointer to the new source vertex
     */
    void setSource(TreePathVertice* value) { _source = value; }

    /**
     * @brief Return the map of neighbouring vertices.
     * @return Map of neighbours keyed by hash name
     */
    Map neighbors() const { return _neighbors; }

    /**
     * @brief Return the hash key used to identify this vertex in the graph map.
     * @return Hash name string
     */
    QString hashName() const { return _hashName; }

    /** @brief Return the human-readable name (alias for name()).
     * @return Name string */
    QString toString() const { return _name; }

    /**
     * @brief Create a canonical hash name for a point.
     * @param point Point to hash
     * @return String uniquely identifying the point's position
     */
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
