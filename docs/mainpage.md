# KanoopCommonQt {#mainpage}

Foundational Qt utility library providing date/time, string, geometry, network, XML, serialization, and logging primitives.

## Modules

| Module | Description |
|--------|-------------|
| **Core** | TimeSpan, DateRange, StringUtil, FloatUtil, DateTimeUtil, EntityMetadata, enums |
| **Geometry** | Point, Rectangle, Circle, Polygon, Triangle, Line, Angle, FlatGeo |
| **Geo** | GeoCoordinate, GeoCircle — WGS-84 distance, azimuth, containment |
| **Serialization** | JSON and XML serialization/deserialization interfaces |
| **Utility** | LoggingBaseClass, AbstractThreadClass, publishing helpers |
| **Client/Server** | TcpServer (with optional SSL/TLS), UdpServer |

## Quick Start

```cpp
#include <Kanoop/timespan.h>
#include <Kanoop/geometry/point.h>
#include <Kanoop/geo/geocoordinate.h>

// Duration arithmetic
TimeSpan duration = TimeSpan::fromMinutes(5) + TimeSpan::fromSeconds(30);

// 2D geometry
Point a(10, 20), b(30, 40);
double dist = a.distanceTo(b);

// Geodetic distance (metres)
GeoCoordinate nyc(40.7128, -74.0060);
GeoCoordinate london(51.5074, -0.1278);
double km = nyc.distanceTo(london) / 1000.0;
```

## Building

```bash
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
cmake --build build --parallel
```

## Links

- [GitHub Repository](https://github.com/StevePunak/KanoopCommonQt)
- [Class List](annotated.html)
- [Class Hierarchy](hierarchy.html)
- [File List](files.html)
