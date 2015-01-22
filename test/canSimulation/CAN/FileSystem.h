#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileSystem : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief write
     * @param source - Path to "log.txt" file
     * @param data - Text log messages to be saved
     * @return return - true if data was successfuly saved
     *      otherway returned false
     */
    bool write(const QString &source, const QString &data);

};

#endif // FILESYSTEM_H
