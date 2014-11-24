#include "FileSystem.h"
#include "loger.h"
#include "color.h"

bool FileSystem::write(QString source, QString data)
{

    if (source.isEmpty())
        return false;

    Loger::loger("File not empty!!!", RED);

    QByteArray array = source.toLocal8Bit();
    char* buffer = array.data();

    Loger::loger(buffer, RED);
    Loger::loger("Destination file name is log.txt!!!", RED);

    QFile file(buffer);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        Loger::loger("File can not be open!!!", RED);
        return false;
    } else {
        Loger::loger("File opened for write!!!", RED);
    }


    QTextStream out(&file);
    out << data;
    file.close();


    Loger::loger("File opened filled with data and closed!!!", RED);
    return true;
}


