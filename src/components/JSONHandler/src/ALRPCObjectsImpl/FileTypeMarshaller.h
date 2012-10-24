#ifndef FILETYPEMARSHALLER_INCLUDE
#define FILETYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/FileType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


//! marshalling class for FileType

class FileTypeMarshaller
{
public:

  static std::string toName(const FileType& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(FileType& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=FileType::INVALID_ENUM;
  }

  static bool checkIntegrity(FileType& e)		{ return e.mInternal!=FileType::INVALID_ENUM; } 
  static bool checkIntegrityConst(const FileType& e)	{ return e.mInternal!=FileType::INVALID_ENUM; } 

  static bool fromString(const std::string& s,FileType& e);
  static const std::string toString(const FileType& e);

  static bool fromJSON(const Json::Value& s,FileType& e);
  static Json::Value toJSON(const FileType& e);

  static const char* getName(FileType::FileTypeInternal e)
  {
     return (e>=0 && e<5) ? mHashTable[e].name : NULL;
  }

  static const FileType::FileTypeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[5];
};

#endif
