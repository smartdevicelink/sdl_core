#ifndef NSAPPLINKRPC_FILETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_FILETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/FileType.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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
  
}

#endif
