#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileSystem : public QObject
{
    Q_OBJECT

public:
    FileSystem();

public slots:
    bool write(QString source, QString data);

private:

};

#endif // FILESYSTEM_H
