#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

class Loger
{
public:
    /**
     * @brief loger
     * @param message - message to be displayed in log
     * @param c - color of the message 0 - red, 1 - green, 2 - blue
     * @return returned true if successfuly redirected parameters to QML
     */
    static bool loger(QString message, int c);

    /**
     * @brief rootView
     * To get access to QML from C++ rootView must be main QObject of QML view
     */
    static QObject *rootView;
};

#endif // LOGGER_H
