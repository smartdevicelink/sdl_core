#ifndef PUBSUBDATA_INCLUDE
#define PUBSUBDATA_INCLUDE


#include "PublishedData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



class PubSubData
{
public:

  PubSubData(const PubSubData& c);
  PubSubData(void);

  bool checkIntegrity(void);
// getters

  const PublishedData& get_data(void) const;

// setters

  bool set_data(const PublishedData& data_);

private:

  friend class PubSubDataMarshaller;

  PublishedData data;
};

#endif
