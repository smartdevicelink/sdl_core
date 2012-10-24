#ifndef SLIDER_REQUEST_INCLUDE
#define SLIDER_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Creates a full screen or pop-up overlay (depending on platform) with a single user controlled slider.

class Slider_request : public ALRPCRequest
{
public:

  Slider_request(const Slider_request& c);
  Slider_request(void);
  
  virtual ~Slider_request(void);

  Slider_request& operator =(const Slider_request&);

  bool checkIntegrity(void);

  const unsigned int* get_numTicks(void) const;
  unsigned int get_position(void) const;
  const std::string* get_sliderHeader(void) const;
  const std::vector<std::string>* get_sliderFooter(void) const;
  unsigned int get_timeout(void) const;

  void reset_numTicks(void);
  bool set_numTicks(unsigned int numTicks_);
  bool set_position(unsigned int position_);
  void reset_sliderHeader(void);
  bool set_sliderHeader(const std::string& sliderHeader_);
  void reset_sliderFooter(void);
  bool set_sliderFooter(const std::vector<std::string>& sliderFooter_);
  bool set_timeout(unsigned int timeout_);

private:

  friend class Slider_requestMarshaller;

  unsigned int* numTicks;	//!<  (2,26)
  unsigned int position;	//!<  (1,26)
  std::string* sliderHeader;	//!< (500)
  std::vector<std::string>* sliderFooter;	//!<   [%s..%s] (500)
  unsigned int timeout;	//!<  (0,65535)
};

#endif
