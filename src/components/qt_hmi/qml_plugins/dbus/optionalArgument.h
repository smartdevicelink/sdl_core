#ifndef OPTIONALARGUMENT_H
#define OPTIONALARGUMENT_H

#include <QDBusArgument>

template<class T>
struct OptionalArgument : public T
{
    bool presence;
    OptionalArgument(const T& value)
        : T(value),
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
    arg << o.presence << static_cast<T>(o);
    arg.endStructure();
    return arg;
}

template<class T>
inline
const QDBusArgument& operator >> (const QDBusArgument& arg, OptionalArgument<T>& o)
{
    T out = static_cast<T>(o);
    arg.beginStructure();
    arg >> o.presence >> out;
    arg.endStructure();
    return arg;
}

#endif // OPTIONALARGUMENT_H
