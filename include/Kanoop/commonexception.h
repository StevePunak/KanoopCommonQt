/**
 *  CommonException
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

/**
 * @brief Base exception class for KanoopCommonQt, extending QException with a message and error code.
 */
class KANOOP_EXPORT CommonException : public QException
{
public:
    /**
     * @brief Construct a CommonException with a message and optional error code.
     * @param message Human-readable description of the error
     * @param code Optional numeric error code (default 0)
     */
    CommonException(const QString& message, qint32 code = 0);

    /** @brief Re-throw this exception. */
    void raise() const override { throw *this; }

    /** @brief Create a heap-allocated copy of this exception. */
    CommonException *clone() const override { return new CommonException(*this); }

    /**
     * @brief Return the error message.
     * @return Human-readable error description
     */
    virtual QString message() const { return _message; }

    /**
     * @brief Return the numeric error code.
     * @return Error code supplied at construction
     */
    virtual qint32 code() const { return _code; }

private:
    QString _message;
    qint32 _code;
};

#endif // COMMONEXCEPTION_H
