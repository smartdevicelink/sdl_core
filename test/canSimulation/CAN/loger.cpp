#include <QVariant>
#include "loger.h"

QObject* Loger::rootView = NULL;


void Loger::loger(QString message, int c)
{
    QVariant returnedValue;

    QMetaObject::invokeMethod(rootView, "logger",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, message),
            Q_ARG(QVariant, c));
}
