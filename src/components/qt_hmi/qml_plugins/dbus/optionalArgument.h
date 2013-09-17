#ifndef OPTIONALARGUMENT_H
#define OPTIONALARGUMENT_H

#include <QDBusArgument>

template<class T>
struct OptionalArgument
{
    bool presence;
    T value;

    OptionalArgument(const T& value)
        : value(value),
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
    arg << o.presence << o.value;
    arg.endStructure();
    return arg;
}

template<class T>
inline
const QDBusArgument& operator >> (const QDBusArgument& arg, OptionalArgument<T>& o)
{
    arg.beginStructure();
    arg >> o.presence >> o.value;
    arg.endStructure();
    return arg;
}

#endif // OPTIONALARGUMENT_H
