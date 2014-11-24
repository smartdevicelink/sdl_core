#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileSystem : public QObject
{
    Q_OBJECT

public slots:
    bool write(QString source, QString data);

};

#endif // FILESYSTEM_H
