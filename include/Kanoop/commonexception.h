/**
 *  AddressHelper
 *
 *  A base class for Qt exceptions
 *
 *  Stephen Punak, September 17 2023
 */
#ifndef COMMONEXCEPTION_H
#define COMMONEXCEPTION_H

#include <QException>
#include <QString>
#include <Kanoop/kanoopcommon.h>

class KANOOP_EXPORT CommonException : public QException
{
public:
    CommonException(const QString& message, qint32 code = 0);

    void raise() const override { throw *this; }
    CommonException *clone() const override { return new CommonException(*this); }

    QString message() const { return _message; }
    qint32 code() const { return _code; }

private:
    QString _message;
    qint32 _code;
};

#endif // COMMONEXCEPTION_H
