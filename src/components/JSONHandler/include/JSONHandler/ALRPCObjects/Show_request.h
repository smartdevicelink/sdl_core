#ifndef SHOW_REQUEST_INCLUDE
#define SHOW_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "SoftButton.h"
#include "TextAlignment.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
  const std::string* get_mainField3(void) const;
  const std::string* get_mainField4(void) const;
  const TextAlignment* get_alignment(void) const;
  const std::string* get_statusBar(void) const;
  const std::string* get_mediaClock(void) const;
  const std::string* get_mediaTrack(void) const;
  const Image* get_graphic(void) const;
  const std::vector<SoftButton>* get_softButtons(void) const;
  const std::vector<std::string>* get_customPresets(void) const;

  void reset_mainField1(void);
  bool set_mainField1(const std::string& mainField1_);
  void reset_mainField2(void);
  bool set_mainField2(const std::string& mainField2_);
  void reset_mainField3(void);
  bool set_mainField3(const std::string& mainField3_);
  void reset_mainField4(void);
  bool set_mainField4(const std::string& mainField4_);
  void reset_alignment(void);
  bool set_alignment(const TextAlignment& alignment_);
  void reset_statusBar(void);
  bool set_statusBar(const std::string& statusBar_);
  void reset_mediaClock(void);
  bool set_mediaClock(const std::string& mediaClock_);
  void reset_mediaTrack(void);
  bool set_mediaTrack(const std::string& mediaTrack_);
  void reset_graphic(void);
  bool set_graphic(const Image& graphic_);
  void reset_softButtons(void);
  bool set_softButtons(const std::vector<SoftButton>& softButtons_);
  void reset_customPresets(void);
  bool set_customPresets(const std::vector<std::string>& customPresets_);

private:

  friend class Show_requestMarshaller;

  std::string* mainField1;	//!< (500)
  std::string* mainField2;	//!< (500)
  std::string* mainField3;	//!< (500)
  std::string* mainField4;	//!< (500)
  TextAlignment* alignment;
  std::string* statusBar;	//!< (500)
  std::string* mediaClock;	//!< (500)
  std::string* mediaTrack;	//!< (500)
  Image* graphic;
  std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
  std::vector<std::string>* customPresets;	//!<   [%s..%s] (500)
};

#endif
