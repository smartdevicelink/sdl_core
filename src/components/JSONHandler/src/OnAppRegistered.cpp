#include "JSONHandler/OnAppRegistered.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnAppRegistered::OnAppRegistered()
:RPC2Notification( RPC2Marshaller::METHOD_ONAPPREGISTERED )
{
    mVrSynonyms = 0;
}
 
OnAppRegistered::~OnAppRegistered()
{
    if (mVrSynonyms)
        delete mVrSynonyms;
}

void OnAppRegistered::setAppName(const std::string & appName)
{
    mAppName = appName;
}

void OnAppRegistered::setAppIcon(const std::string & appIcon)
{
    mAppIcon = appIcon;
}
        
void OnAppRegistered::setVrSynonyms(const std::vector<std::string> & vrSynonyms)
{
    delete mVrSynonyms;
    mVrSynonyms = 0;
    mVrSynonyms = new std::vector<std::string>(vrSynonyms);
}
        
void OnAppRegistered::setIsMediaApplication(bool isMediaApplication)
{
    mIsMediaApplication = isMediaApplication;
}
        
void OnAppRegistered::setLanguageDesired(const Language& languageDesired)
{
    mLanguageDesired = languageDesired;
}

std::string OnAppRegistered::getAppName(void) const
{
    return mAppName;
}

std::string OnAppRegistered::getAppIcon(void) const
{
    return mAppIcon;
}
        
const std::vector<std::string>* OnAppRegistered::getVrSynonyms(void) const
{
    return mVrSynonyms;
}
        
bool OnAppRegistered::getIsMediaApplication(void) const
{
    return mIsMediaApplication;
}
        
const Language& OnAppRegistered::getLanguageDesired(void) const
{
    return mLanguageDesired;
}