# CLAUDE.md — KanoopCommonQt

Guidance for Claude Code sessions working with this library. Read this before touching code in this submodule.

## Overview

KanoopCommonQt is the foundational utility library underneath the rest of the Kanoop Qt stack. It provides date/time, string, file, path, network, XML, crypto, logging, threading, and exception primitives. It contains **no GUI code** (that's KanoopGuiQt), **no database code** (KanoopDatabaseQt), and **no protocol code** (KanoopProtocolQt).

- Trunk branch: **`master`** (not `main`).
- Public include path: `<Kanoop/...>` — for example `#include <Kanoop/log.h>`, `#include <Kanoop/utility/abstractthreadclass.h>`.
- Export macro: **`KANOOP_EXPORT`** — every public class or free function exposed in a header must be marked with it. Missing exports compile on Linux but fail to link on Windows MinGW.
- Namespaces: most utilities are global; `Log` (logging types) and `KANOOP` (`DataRole`, `EnumToStringMap`, model roles) are the two namespaces in regular use.
- Exception type: throw `CommonException` from `<Kanoop/commonexception.h>`. Catch by reference.

## Library conventions

- Static utility classes (`StringUtil`, `FileUtil`, `PathUtil`, `DateTimeUtil`, `CryptoUtil`) — never instantiate; call the static methods.
- Time durations: use `TimeSpan`, not `int milliseconds` or `std::chrono`. Construct via the named factories (`TimeSpan::fromSeconds`, `TimeSpan::fromMilliseconds`).
- Timing diffs: `TimeSpan::absDiff(QDateTime, QDateTime)` over `QDateTime::currentDateTimeUtc()`.
- Logging: never `qDebug()`. Use `Log::logText(LVL_DEBUG, ...)` or, inside a `LoggingBaseClass` subclass, the member `logText(LVL_DEBUG, ...)`. The `LVL_*` macros already expand to `__FILE__, __LINE__, Log::LogLevel::X` — do not add file/line yourself.
- Hashing: never `QCryptographicHash`. Use `CryptoUtil::fileMd5String`, `CryptoUtil::fileSha256String`, `CryptoUtil::md5String`.
- Threading: subclass `AbstractThreadClass` (see below) — never raw `QThread` + `moveToThread` + `QObject` worker.

## Code style

- **Class names**: PascalCase. **Methods**: camelCase. **Members**: underscore-prefixed camelCase (`_foo`). **Statics**: PascalCase, no prefix.
- Opening brace on same line for `if`/`for`/`while`/`try`; on new line for function/method definitions.
- No space before parens in control flow: `if(cond) {`, not `if (cond) {`.
- Explicit boolean comparison: `if(isReady() == false)` and `if(ok == true)`, not `if(!isReady())`.
- Includes ordered: same-library (`"..."`), then dependency libs (`<Kanoop/...>`), then Qt/system (`<QString>`).
- Class layout: `Q_OBJECT`, `public:` (ctors → operators → JSON serialize → getters/setters → `isValid` → variant helpers → static parsers), `public slots:`, `protected:`, `signals:`, `protected slots:`.
- File-local **helper functions** used by one class → `static` private members of the class, not anonymous namespace or file-scope statics. File-local **constants** in an anonymous namespace are fine.
- Doxygen: single-line `/** @brief ... */`; multi-line with `@brief` on the line after `/**`. **Never** document member variables.
- Single-exit functions preferred; avoid early returns.

## Commonly used classes

### `Log` and `LVL_*` — `<Kanoop/log.h>`
Singleton logger with file/console/syslog/consumer outputs, severity levels, and named categories.
```cpp
#include <Kanoop/log.h>
Log::logText(LVL_DEBUG, QString("Connecting to %1").arg(host));
Log::logText(LVL_WARNING, "fallback path engaged");
Log::registerCategory("ParameterQuery");           // once at startup
Log::setLevel(Log::Info);                          // global filter
```
Pitfalls: `LVL_DEBUG` is *not* a level — it's a three-arg macro pack. Pass it as the first arg, then the message. Configure the logger before worker threads start.

### `TimeSpan` — `<Kanoop/timespan.h>`
Signed nanosecond-resolution duration; .NET-style API.
```cpp
TimeSpan timeout = TimeSpan::fromSeconds(30);
TimeSpan elapsed = TimeSpan::absDiff(t1, t2);
double ms = elapsed.totalMilliseconds();
if(elapsed > TimeSpan::fromSeconds(5)) { /* slow path */ }
```
Pitfalls: default-constructed `TimeSpan()` is zero, not invalid — there is no invalid state. Construct from POSIX `timespec` is supported.

### `CommonException` — `<Kanoop/commonexception.h>`
Heap-copyable Qt exception with message + numeric code.
```cpp
throw CommonException("Connection failed", 42);
try { /* ... */ } catch(CommonException& e) { Log::logText(LVL_ERROR, e.message()); }
```
Pitfalls: catch by reference (`QException` requirement). Subclass for typed exceptions.

### `EntityMetadata` — `<Kanoop/entitymetadata.h>`
Type-tagged, role-keyed data container — the bridge between domain objects and KanoopGuiQt models.
```cpp
EntityMetadata md(MyEntityTypes::Device, deviceObj, deviceUuid);
int type = md.type();
QUuid id = md.uuid();
QVariant payload = md.data(KANOOP::DataRole);
```
Pitfalls: type IDs are application-defined integers — define an enum/constants per project. Register heavy types via `EntityMetadata::registerMetadata()` before use.

### `DateTimeUtil` — `<Kanoop/datetimeutil.h>`
Static parse/format helpers for ISO-8601 Zulu, MySQL, and compact formats.
```cpp
QDateTime t = DateTimeUtil::fromISOString("2026-05-15T18:20:40.000Z");
QString iso = DateTimeUtil::toISOString(QDateTime::currentDateTimeUtc());
QString sql = DateTimeUtil::toStandardString(t, true);   // with ms
```
Pitfalls: `fromISOString` forces UTC. On parse failure returns an invalid `QDateTime` — check `isValid()`.

### `StringUtil` — `<Kanoop/stringutil.h>`
Conversions, hex dumps, fuzzy matching.
```cpp
bool b = StringUtil::toBool("true");
QString hex = StringUtil::toString(bytes, " ");                  // "DE AD BE EF"
QString dump = StringUtil::toHexTable(bytes, true, true, 16);    // with offsets + ASCII
```
Pitfalls: `toString(double)` trims trailing zeros by default; pass `forcePadding=true` to keep them.

### `FileUtil` / `PathUtil` — `<Kanoop/fileutil.h>`, `<Kanoop/pathutil.h>`
One-line file I/O and safe path joining.
```cpp
QByteArray data;
if(FileUtil::readAllBytes(path, data) == false) { /* error */ }
FileUtil::writeAllBytes(out, data);
QString full = PathUtil::combine(dir, "subdir", "file.txt");
```
Pitfalls: `PathUtil::combine` is the value-add — use it instead of manual `/` concatenation. Other `PathUtil` helpers are deprecated in favor of `QFileInfo`.

### `CryptoUtil` — `<Kanoop/cryptoutil.h>`
Hash helpers — always use these instead of `QCryptographicHash`.
```cpp
QString md5  = CryptoUtil::fileMd5String(path);
QString sha  = CryptoUtil::fileSha256String(path);
QString b64  = CryptoUtil::md5String(QByteArray("payload"));
```

### `AbstractThreadClass` — `<Kanoop/utility/abstractthreadclass.h>`
The standard threading base. Owns a `QThread` internally; subclasses run their work in `threadStarted()`/`run()`-style loops and check `isStopping()` to terminate cleanly.
```cpp
class MyWorker : public AbstractThreadClass {
public:
    MyWorker() : AbstractThreadClass("MyWorker") {}
protected:
    void threadStarted() override {
        while(isStopping() == false) {
            // do work
        }
    }
};
MyWorker w;
w.start();
// ... later
w.stop();                      // blocks until threadStarted() returns
```
Pitfalls: subclass code **must** poll `isStopping()` regularly or `stop()` will block until the internal timeout. Public emit-wrapper methods + private `Q_SIGNAL` signals is the standard pattern for cross-thread notifications — never reach for `QThread` + `moveToThread` directly.

## Common gotchas

- **LVL_\* macros are three args.** `Log::logText(LVL_DEBUG, "msg")` passes four arguments to the function — the macro expands to file/line/level. Do not also pass `__FILE__` etc. yourself.
- **`AbstractThreadClass::stop()` is cooperative.** A worker that doesn't poll `isStopping()` will not stop cleanly. Same applies to long blocking calls inside the worker — wrap them with cancellation.
- **Singleton thread-safety.** `Log::systemLog()` and other singletons in this library are internally thread-safe at call time but **configuration races** (e.g., `setFilename`) with logging from worker threads are easy to introduce. Configure during startup, before launching workers.
- **No Doxygen comments on member variables.** Linter and pre-PR review will flag them. Use `Q_PROPERTY` with a docstring above the macro if you genuinely need it documented.
- **Declspec on every new public class.** Linux exports everything by default; Windows MinGW does not. Missing `KANOOP_EXPORT` compiles locally and fails CI on Windows.

## Working in this repo

- When this library is consumed as a git submodule, the submodule will typically be in **detached HEAD** state pointing at a pinned commit. Check out a working branch before committing.
- Conventional-commits message format is used: `<type>(<scope>): <description>` with types `fix`, `feat`, `doc`, `refactor`, `test`, `chore`. Add a `Co-Authored-By` trailer if pair-programming.
- Submodule pointer changes belong in the consuming repo, not here.
