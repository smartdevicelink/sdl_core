#include "../include/JSONHandler/SDLRPCObjects/V2/PutFile_request.h"
#include "PutFile_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "FileTypeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;
PutFile_request& PutFile_request::operator =(const PutFile_request& c)
{
  syncFileName= c.syncFileName;
  fileType= c.fileType;
  persistentFile= c.persistentFile ? new bool(c.persistentFile[0]) : 0;
  fileData= c.fileData ? new std::string(c.fileData[0]) : 0;

  return *this;
}


PutFile_request::~PutFile_request(void)
{
  if(persistentFile)
    delete persistentFile;
  if(fileData)
    delete fileData;
}


PutFile_request::PutFile_request(const PutFile_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool PutFile_request::checkIntegrity(void)
{
  return PutFile_requestMarshaller::checkIntegrity(*this);
}


PutFile_request::PutFile_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      persistentFile(0),
    fileData(0)
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

bool PutFile_request::set_fileData(const std::string& fileData_)
{
  delete fileData;
  fileData=0;

  fileData=new std::string(fileData_);
  return true;
}

void PutFile_request::reset_fileData(void)
{
  if(fileData)
    delete fileData;
  fileData=0;
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

const std::string* PutFile_request::get_fileData(void) const 
{
  return fileData;
}

