/**
 * @brief A 2D polygon defined by an ordered list of vertices.
 */
#ifndef POLYGON_H
#define POLYGON_H
#include "point.h"
#include "line.h"

/**
 * @brief Represents a 2D polygon as an ordered list of Point vertices.
 *
 * Provides area, centroid, containment, and movement operations.
 */
class KANOOP_EXPORT Polygon
{
public:
    /** @brief Default constructor — creates an empty (invalid) polygon. */
    Polygon() {}

    /**
     * @brief Construct a rectangle-shaped polygon from a line expanded by a width.
     * @param line Centre line of the polygon
     * @param expandBy Half-width of the resulting rectangular polygon
     * @return Rectangular Polygon centred on the line
     */
    static Polygon fromLine(const Line& line, int expandBy);

    /**
     * @brief Append a vertex to the polygon.
     * @param point Vertex to add
     */
    void appendPoint(const Point& point) { _points.append(point); }

    /**
     * @brief Compute the area of the polygon using the shoelace formula.
     * @return Signed area (positive for counter-clockwise winding)
     */
    double area() const;

    /**
     * @brief Move all vertices by a bearing and distance.
     * @param bearing Direction of movement in degrees
     * @param distance Distance to move
     */
    void move(double bearing, double distance);

    /**
     * @brief Rotate all vertices around a centroid by an angle.
     * @param centroid Centre of rotation
     * @param angle Rotation angle in degrees
     */
    void rotate(const Point& centroid, double angle);

    /**
     * @brief Compute the centroid (geometric centre) of the polygon.
     * @return Centroid Point
     */
    Point centroid() const;

    /**
     * @brief Test whether a point lies inside the polygon.
     * @param point Point to test
     * @return true if the point is inside the polygon boundary
     */
    bool contains(const Point& point) const;

    /**
     * @brief Return the edges of the polygon as a list of lines.
     * @return Line list connecting consecutive vertices
     */
    Line::List lines() const;

    /**
     * @brief Return the bounding rectangle of this polygon.
     * @return Smallest Rectangle enclosing all vertices
     */
    Rectangle boundingRectangle() const;

    /**
     * @brief Test whether this polygon has at least 3 vertices.
     * @return true if the polygon has enough vertices to be valid
     */
    bool isValid() const { return _points.count() > 2; }

private:
    /** @brief Build the polygon from a list of edges. */
    void createFromLines(const Line::List &lines);
    /** @brief Return the minimum coordinate for winding calculations. */
    double minimumXY() const;

    Point::List _points;
};

#endif // POLYGON_H
