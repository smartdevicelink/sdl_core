//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPCV2_HMIAPPLICATION_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_HMIAPPLICATION_INCLUDE

#include <string>
#include <vector>

#include "AppType.h"
#include "Language.h"
#include "Language.h"
#include "TTSChunk.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

///  Data type containing information about application needed by HMI.

  class HMIApplication
  {
  public:
  
    HMIApplication(const HMIApplication& c);
    HMIApplication(void);
  
    bool checkIntegrity(void);
  
    ~HMIApplication(void);
    HMIApplication& operator =(const HMIApplication&);

// getters

    int get_appId(void) const;
    const std::string& get_appName(void) const;
    const std::vector<AppType>* get_appType(void) const;
    const std::string& get_deviceName(void) const;
    const Language& get_hmiDisplayLanguageDesired(void) const;
    const std::string* get_icon(void) const;
    bool get_isMediaApplication(void) const;
    const Language& get_languageDesired(void) const;
    const std::string* get_ngnMediaScreenAppName(void) const;
    const std::vector<TTSChunk>* get_ttsName(void) const;
    const std::vector<std::string>* get_vrSynonyms(void) const;

// setters

    bool set_appId(int appId_);
    bool set_appName(const std::string& appName_);
    void reset_appType(void);
    bool set_appType(const std::vector<AppType>& appType_);
    bool set_deviceName(const std::string& deviceName_);
    bool set_hmiDisplayLanguageDesired(const Language& hmiDisplayLanguageDesired_);
    void reset_icon(void);
    bool set_icon(const std::string& icon_);
    bool set_isMediaApplication(bool isMediaApplication_);
    bool set_languageDesired(const Language& languageDesired_);
    void reset_ngnMediaScreenAppName(void);
    bool set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_);
    void reset_ttsName(void);
    bool set_ttsName(const std::vector<TTSChunk>& ttsName_);
    void reset_vrSynonyms(void);
    bool set_vrSynonyms(const std::vector<std::string>& vrSynonyms_);

  private:

    friend class HMIApplicationMarshaller;

      int appId;
      std::string appName;	//!< (100)
      std::vector<AppType>* appType;	//!<   [%s..%s] 
      std::string deviceName;
      Language hmiDisplayLanguageDesired;
      std::string* icon;
      bool isMediaApplication;
      Language languageDesired;
      std::string* ngnMediaScreenAppName;	//!< (100)
      std::vector<TTSChunk>* ttsName;	//!<   [%s..%s] 
      std::vector<std::string>* vrSynonyms;	//!<   [%s..%s] (40)
  };

}

#endif
