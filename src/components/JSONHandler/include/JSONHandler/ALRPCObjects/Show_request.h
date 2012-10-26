#ifndef SHOW_REQUEST_INCLUDE
#define SHOW_REQUEST_INCLUDE

#include <string>

#include "TextAlignment.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


///  Updates the persistent display. Supported fields depend on display capabilities.

class Show_request : public ALRPCRequest
{
public:

  Show_request(const Show_request& c);
  Show_request(void);
  
  virtual ~Show_request(void);

  Show_request& operator =(const Show_request&);

  bool checkIntegrity(void);

  const std::string* get_mainField1(void) const;
  const std::string* get_mainField2(void) const;
  const TextAlignment* get_alignment(void) const;
  const std::string* get_statusBar(void) const;
  const std::string* get_mediaClock(void) const;
  const std::string* get_mediaTrack(void) const;

  void reset_mainField1(void);
  bool set_mainField1(const std::string& mainField1_);
  void reset_mainField2(void);
  bool set_mainField2(const std::string& mainField2_);
  void reset_alignment(void);
  bool set_alignment(const TextAlignment& alignment_);
  void reset_statusBar(void);
  bool set_statusBar(const std::string& statusBar_);
  void reset_mediaClock(void);
  bool set_mediaClock(const std::string& mediaClock_);
  void reset_mediaTrack(void);
  bool set_mediaTrack(const std::string& mediaTrack_);

private:

  friend class Show_requestMarshaller;

  std::string* mainField1;	//!< (500)
  std::string* mainField2;	//!< (500)
  TextAlignment* alignment;
  std::string* statusBar;	//!< (500)
  std::string* mediaClock;	//!< (500)
  std::string* mediaTrack;	//!< (500)
};

#endif
