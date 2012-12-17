#include "../include/JSONHandler/ALRPCObjects/V2/PutFile_request.h"
#include "PutFile_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "FileTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
PutFile_request& PutFile_request::operator =(const PutFile_request& c)
{
  syncFileName= c.syncFileName;
  fileType= c.fileType;
  persistentFile= c.persistentFile ? new bool(c.persistentFile[0]) : 0;
//  fileData= c.fileData;

  return *this;
}


PutFile_request::~PutFile_request(void)
{
  if(persistentFile)
    delete persistentFile;
}


PutFile_request::PutFile_request(const PutFile_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool PutFile_request::checkIntegrity(void)
{
  return PutFile_requestMarshaller::checkIntegrity(*this);
}


PutFile_request::PutFile_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      persistentFile(0)
{
}



bool PutFile_request::set_syncFileName(const std::string& syncFileName_)
{
  if(syncFileName_.length()>500)  return false;
  syncFileName=syncFileName_;
  return true;
}

bool PutFile_request::set_fileType(const FileType& fileType_)
{
  if(!FileTypeMarshaller::checkIntegrityConst(fileType_))   return false;
  fileType=fileType_;
  return true;
}

bool PutFile_request::set_persistentFile(bool persistentFile_)
{
  delete persistentFile;
  persistentFile=0;

  persistentFile=new bool(persistentFile_);
  return true;
}

void PutFile_request::reset_persistentFile(void)
{
  if(persistentFile)
    delete persistentFile;
  persistentFile=0;
}






const std::string& PutFile_request::get_syncFileName(void) const 
{
  return syncFileName;
}

const FileType& PutFile_request::get_fileType(void) const 
{
  return fileType;
}

const bool* PutFile_request::get_persistentFile(void) const 
{
  return persistentFile;
}



