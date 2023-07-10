#ifndef COMMONEXCEPTION_H
#define COMMONEXCEPTION_H

#include <QException>
#include <QString>

class CommonException : public QException
{
public:
    CommonException(const QString& message, qint32 code = 0) :
        _message(message),
        _code(code) {}

    void raise() const override { throw *this; }
    CommonException *clone() const override { return new CommonException(*this); }

    QString message() const { return _message; }
    qint32 code() const { return _code; }

private:
    QString _message;
    qint32 _code;
};

#endif // COMMONEXCEPTION_H
