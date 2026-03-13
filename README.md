# KanoopCommonQt

Foundational Qt utility library providing date/time, string, geometry, network, XML, serialization, and logging primitives. Used as a base dependency across all EPC and Kanoop Qt projects.

**[API Documentation](https://StevePunak.github.io/KanoopCommonQt/)** | [Class List](https://StevePunak.github.io/KanoopCommonQt/annotated.html) | [Class Hierarchy](https://StevePunak.github.io/KanoopCommonQt/hierarchy.html) | [Files](https://StevePunak.github.io/KanoopCommonQt/files.html)

## Requirements

- C++11
- Qt 6.7.0+ (Network, Xml)
- CMake 3.16+

## Building

KanoopCommonQt is typically built as part of the [meta-qt-mains](https://github.com/epcpower/meta-qt-mains) superproject. To build standalone:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
cmake --build build --parallel
```

## Modules

| Module | Headers | Description |
|--------|---------|-------------|
| **core** | 25 | Date/time, string utilities, entity metadata, exceptions, enums, logging |
| **geometry** | 14 | Angles, points, lines, rectangles, triangles, flat geometry helpers |
| **serialization** | 8 | JSON/XML serialization interfaces and utilities |
| **geo** | 4 | Geographic coordinates, distance calculations, geo fencing |
| **utility** | 4 | Logging base class, publishing helpers |
| **clientserver** | 3 | TCP client/server abstractions |

## API Documentation

Full Doxygen documentation is generated automatically on every CI build and published to GitHub Pages:

**https://StevePunak.github.io/KanoopCommonQt/**

To generate locally:

```bash
# Via CMake (requires Doxygen + Graphviz)
cmake --build build --target doc_KanoopCommonQt

# Or directly
doxygen Doxyfile
```

Open `docs/html/index.html` in a browser to browse the local copy.

## CI

[![CI](https://github.com/StevePunak/KanoopCommonQt/actions/workflows/ci.yaml/badge.svg)](https://github.com/StevePunak/KanoopCommonQt/actions/workflows/ci.yaml)

Builds on every push using GitHub Actions (`ubuntu-latest`, Qt 6.10.1, Ninja). Doxygen docs are deployed to GitHub Pages on merges to `main`.

## Project Structure

```
KanoopCommonQt/
  include/Kanoop/         Public headers organized by module
  src/                    Implementation files
  docs/                   Generated documentation (not committed)
  Doxyfile                Doxygen configuration
  CMakeLists.txt          Build configuration
```

## License

[MIT](LICENSE)
