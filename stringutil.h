#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>

class StringUtil
{
public:
    static QString toString(bool value) { return value ? "true" : "false"; }
};

#endif // STRINGUTIL_H
