#include "../include/JSONHandler/ALRPCObjects/V1/Show_request.h"
#include "Show_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "TextAlignmentMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
Show_request& Show_request::operator =(const Show_request& c)
{
  mainField1= c.mainField1 ? new std::string(c.mainField1[0]) : 0;
  mainField2= c.mainField2 ? new std::string(c.mainField2[0]) : 0;
  alignment= c.alignment ? new TextAlignment(c.alignment[0]) : 0;
  statusBar= c.statusBar ? new std::string(c.statusBar[0]) : 0;
  mediaClock= c.mediaClock ? new std::string(c.mediaClock[0]) : 0;
  mediaTrack= c.mediaTrack ? new std::string(c.mediaTrack[0]) : 0;

  return *this;}


Show_request::~Show_request(void)
{
  if(mainField1)
    delete mainField1;
  if(mainField2)
    delete mainField2;
  if(alignment)
    delete alignment;
  if(statusBar)
    delete statusBar;
  if(mediaClock)
    delete mediaClock;
  if(mediaTrack)
    delete mediaTrack;
}


Show_request::Show_request(const Show_request& c)
{
  *this=c;
}


bool Show_request::checkIntegrity(void)
{
  return Show_requestMarshaller::checkIntegrity(*this);
}


Show_request::Show_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SHOW_REQUEST),
      mainField1(0),
    mainField2(0),
    alignment(0),
    statusBar(0),
    mediaClock(0),
    mediaTrack(0)
{
}



bool Show_request::set_mainField1(const std::string& mainField1_)
{
  if(mainField1_.length()>500)  return false;
  delete mainField1;
  mainField1=0;

  mainField1=new std::string(mainField1_);
  return true;
}

void Show_request::reset_mainField1(void)
{
  if(mainField1)
    delete mainField1;
  mainField1=0;
}

bool Show_request::set_mainField2(const std::string& mainField2_)
{
  if(mainField2_.length()>500)  return false;
  delete mainField2;
  mainField2=0;

  mainField2=new std::string(mainField2_);
  return true;
}

void Show_request::reset_mainField2(void)
{
  if(mainField2)
    delete mainField2;
  mainField2=0;
}

bool Show_request::set_alignment(const TextAlignment& alignment_)
{
  if(!TextAlignmentMarshaller::checkIntegrityConst(alignment_))   return false;
  delete alignment;
  alignment=0;

  alignment=new TextAlignment(alignment_);
  return true;
}

void Show_request::reset_alignment(void)
{
  if(alignment)
    delete alignment;
  alignment=0;
}

bool Show_request::set_statusBar(const std::string& statusBar_)
{
  if(statusBar_.length()>500)  return false;
  delete statusBar;
  statusBar=0;

  statusBar=new std::string(statusBar_);
  return true;
}

void Show_request::reset_statusBar(void)
{
  if(statusBar)
    delete statusBar;
  statusBar=0;
}

bool Show_request::set_mediaClock(const std::string& mediaClock_)
{
  if(mediaClock_.length()>500)  return false;
  delete mediaClock;
  mediaClock=0;

  mediaClock=new std::string(mediaClock_);
  return true;
}

void Show_request::reset_mediaClock(void)
{
  if(mediaClock)
    delete mediaClock;
  mediaClock=0;
}

bool Show_request::set_mediaTrack(const std::string& mediaTrack_)
{
  if(mediaTrack_.length()>500)  return false;
  delete mediaTrack;
  mediaTrack=0;

  mediaTrack=new std::string(mediaTrack_);
  return true;
}

void Show_request::reset_mediaTrack(void)
{
  if(mediaTrack)
    delete mediaTrack;
  mediaTrack=0;
}




const std::string* Show_request::get_mainField1(void) const 
{
  return mainField1;
}

const std::string* Show_request::get_mainField2(void) const 
{
  return mainField2;
}

const TextAlignment* Show_request::get_alignment(void) const 
{
  return alignment;
}

const std::string* Show_request::get_statusBar(void) const 
{
  return statusBar;
}

const std::string* Show_request::get_mediaClock(void) const 
{
  return mediaClock;
}

const std::string* Show_request::get_mediaTrack(void) const 
{
  return mediaTrack;
}

