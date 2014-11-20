#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

class Loger
{
public:
    static void loger(QString message, int c);
    static QObject *rootView;
};

#endif // LOGGER_H
