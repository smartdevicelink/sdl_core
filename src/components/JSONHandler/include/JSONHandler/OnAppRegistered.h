#ifndef ON_APP_REGISTERED_CLASS
#define ON_APP_REGISTERED_CLASS

#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ALRPCObjects/Language.h"
#include <vector>

namespace RPC2Communication
{
    class OnAppRegistered : public RPC2Notification
    {
    public:
        OnAppRegistered();
        ~OnAppRegistered();

        void setAppName(const std::string & appName);
        void setAppIcon(const std::string & appIcon);
        void setVrSynonyms(const std::vector<std::string> & vrSynonyms);
        void setIsMediaApplication(bool isMediaApplication);
        void setLanguageDesired(const Language& languageDesired);

        std::string getAppName(void) const;
        std::string getAppIcon(void) const;
        const std::vector<std::string>* getVrSynonyms(void) const;
        bool getIsMediaApplication(void) const;
        const Language& getLanguageDesired(void) const;
    
    private:
        std::string mAppName;
        std::string mAppIcon;
        std::vector<std::string>* mVrSynonyms;
        bool mIsMediaApplication;
        Language mLanguageDesired;

        friend class OnAppRegisteredMarshaller;
    };
}

#endif