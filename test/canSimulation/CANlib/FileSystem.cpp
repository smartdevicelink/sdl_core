#include "FileSystem.h"
#include "color.h"

bool FileSystem::write(const QString &source, const QString &data)
{

    if (source.isEmpty()) {
        emit log("File path is empty!!!", RED);
        return false;
    }

    QByteArray array = source.toLocal8Bit();
    char* buffer = array.data();

    emit log(buffer, RED);

    QFile file(buffer);

    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        emit log("File can not be open!!!", RED);
        return false;
    } else {
        emit log("File opened for write!!!", RED);
    }

    QTextStream out(&file);
    out << data;
    file.close();

    // File opened filled with data and closed

    return true;
}


