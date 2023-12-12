#include "Kanoop/commonexception.h"

CommonException::CommonException(const QString& message, qint32 code) :
    _message(message),
    _code(code)
{
}
