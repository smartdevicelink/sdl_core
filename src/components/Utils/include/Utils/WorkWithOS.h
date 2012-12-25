/**
* \file WorkWithOS.h
* \brief class source file.
* \author PVyshnevska
*/

#ifndef WORKWITHSYSTEM_INCLUDE
#define WORKWITHSYSTEM_INCLUDE

#include <string.h>
#include <vector>

namespace WorkWithOS
{
    unsigned long int getAvailableSpace();
    std::string createDirectory(const std::string & directoryName);
    bool checkIfDirectoryExists(const std::string & directoryName);
    bool checkIfFileExists(const std::string & fileName);
    bool createFileAndWrite(const std::string & fileName, const std::vector<unsigned char>& fileData);
    std::string getFullPath(const std::string & fileName);
    bool deleteFile(const std::string & fileName);
    std::vector<std::string> listFilesInDirectory(const std::string & directoryName);
}

#endif // WORKWITHSYSTEM_INCLUDE    