#ifndef OPTIONALARGUMENT_H
#define OPTIONALARGUMENT_H

#include <QDBusArgument>

template<class T>
struct OptionalArgument
{
    T val;
    bool presence;
    OptionalArgument(const T& value)
        : val(value),
          presence(true)
    { }
    OptionalArgument()
        : presence(false)
    { }
};

template<class T>
inline
QDBusArgument& operator << (QDBusArgument& arg, const OptionalArgument<T>& o)
{
    arg.beginStructure();
    arg << o.presence << o.val;
    arg.endStructure();
    return arg;
}

template<class T>
inline
const QDBusArgument& operator >> (const QDBusArgument& arg, OptionalArgument<T>& o)
{
    arg.beginStructure();
    arg >> o.presence >> o.val;
    arg.endStructure();
    return arg;
}

Q_DECLARE_METATYPE(OptionalArgument<int>)
Q_DECLARE_METATYPE(OptionalArgument<QString>)
Q_DECLARE_METATYPE(OptionalArgument<bool>)
Q_DECLARE_METATYPE(OptionalArgument<double>)

#endif // OPTIONALARGUMENT_H
