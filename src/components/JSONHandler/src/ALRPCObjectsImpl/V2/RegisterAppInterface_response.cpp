#include "../include/JSONHandler/ALRPCObjects/V2/RegisterAppInterface_response.h"
#include "RegisterAppInterface_responseMarshaller.h"
#include "ButtonCapabilitiesMarshaller.h"
#include "DisplayCapabilitiesMarshaller.h"
#include "HmiZoneCapabilitiesMarshaller.h"
#include "LanguageMarshaller.h"
#include "PresetBankCapabilitiesMarshaller.h"
#include "SoftButtonCapabilitiesMarshaller.h"
#include "SpeechCapabilitiesMarshaller.h"
#include "SyncMsgVersionMarshaller.h"
#include "VehicleTypeMarshaller.h"
#include "VrCapabilitiesMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

using namespace NsAppLinkRPCV2;
RegisterAppInterface_response& RegisterAppInterface_response::operator =(const RegisterAppInterface_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    syncMsgVersion = c.syncMsgVersion ? new SyncMsgVersion(c.syncMsgVersion[0]) : 0;
    language = c.language ? new Language(c.language[0]) : 0;
    hmiDisplayLanguage = c.hmiDisplayLanguage ? new Language(c.hmiDisplayLanguage[0]) : 0;
    displayCapabilities = c.displayCapabilities ? new DisplayCapabilities(c.displayCapabilities[0]) : 0;
    buttonCapabilities = c.buttonCapabilities ? new std::vector<ButtonCapabilities>(c.buttonCapabilities[0]) : 0;
    softButtonCapabilities = c.softButtonCapabilities ? new std::vector<SoftButtonCapabilities>(c.softButtonCapabilities[0]) : 0;
    presetBankCapabilities = c.presetBankCapabilities ? new PresetBankCapabilities(c.presetBankCapabilities[0]) : 0;
    hmiZoneCapabilities = c.hmiZoneCapabilities ? new std::vector<HmiZoneCapabilities>(c.hmiZoneCapabilities[0]) : 0;
    speechCapabilities = c.speechCapabilities ? new std::vector<SpeechCapabilities>(c.speechCapabilities[0]) : 0;
    vrCapabilities = c.vrCapabilities ? new std::vector<VrCapabilities>(c.vrCapabilities[0]) : 0;
    vehicleType = c.vehicleType ? new VehicleType(c.vehicleType[0]) : 0;

    return *this;
}

RegisterAppInterface_response::~RegisterAppInterface_response(void)
{
    if (syncMsgVersion)
    {
        delete syncMsgVersion;
    }
    if (language)
    {
        delete language;
    }
    if (hmiDisplayLanguage)
    {
        delete hmiDisplayLanguage;
    }
    if (displayCapabilities)
    {
        delete displayCapabilities;
    }
    if (buttonCapabilities)
    {
        delete buttonCapabilities;
    }
    if (softButtonCapabilities)
    {
        delete softButtonCapabilities;
    }
    if (presetBankCapabilities)
    {
        delete presetBankCapabilities;
    }
    if (hmiZoneCapabilities)
    {
        delete hmiZoneCapabilities;
    }
    if (speechCapabilities)
    {
        delete speechCapabilities;
    }
    if (vrCapabilities)
    {
        delete vrCapabilities;
    }
    if (vehicleType)
    {
        delete vehicleType;
    }
}

RegisterAppInterface_response::RegisterAppInterface_response(const RegisterAppInterface_response& c)
{
    *this = c;
}

bool RegisterAppInterface_response::checkIntegrity(void)
{
    return RegisterAppInterface_responseMarshaller::checkIntegrity(*this);
}

RegisterAppInterface_response::RegisterAppInterface_response(void)
    : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
    , syncMsgVersion(0)
    , language(0)
    , hmiDisplayLanguage(0)
    , displayCapabilities(0)
    , buttonCapabilities(0)
    , softButtonCapabilities(0)
    , presetBankCapabilities(0)
    , hmiZoneCapabilities(0)
    , speechCapabilities(0)
    , vrCapabilities(0)
    , vehicleType(0)
{
}

bool RegisterAppInterface_response::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
    if (!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))
    {
        return false;
    }
    delete syncMsgVersion;
    syncMsgVersion = 0;

    syncMsgVersion = new SyncMsgVersion(syncMsgVersion_);
    return true;
}

void RegisterAppInterface_response::reset_syncMsgVersion(void)
{
    if (syncMsgVersion)
    {
        delete syncMsgVersion;
    }
    syncMsgVersion = 0;
}

bool RegisterAppInterface_response::set_language(const Language& language_)
{
    if (!LanguageMarshaller::checkIntegrityConst(language_))
    {
        return false;
    }
    delete language;
    language = 0;

    language = new Language(language_);
    return true;
}

void RegisterAppInterface_response::reset_language(void)
{
    if (language)
    {
        delete language;
    }
    language = 0;
}

bool RegisterAppInterface_response::set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_)
{
    if (!LanguageMarshaller::checkIntegrityConst(hmiDisplayLanguage_))
    {
        return false;
    }
    delete hmiDisplayLanguage;
    hmiDisplayLanguage = 0;

    hmiDisplayLanguage = new Language(hmiDisplayLanguage_);
    return true;
}

void RegisterAppInterface_response::reset_hmiDisplayLanguage(void)
{
    if (hmiDisplayLanguage)
    {
        delete hmiDisplayLanguage;
    }
    hmiDisplayLanguage = 0;
}

bool RegisterAppInterface_response::set_displayCapabilities(const DisplayCapabilities& displayCapabilities_)
{
    if (!DisplayCapabilitiesMarshaller::checkIntegrityConst(displayCapabilities_))
    {
        return false;
    }
    delete displayCapabilities;
    displayCapabilities = 0;

    displayCapabilities = new DisplayCapabilities(displayCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_displayCapabilities(void)
{
    if (displayCapabilities)
    {
        delete displayCapabilities;
    }
    displayCapabilities = 0;
}

bool RegisterAppInterface_response::set_buttonCapabilities(const std::vector<ButtonCapabilities>& buttonCapabilities_)
{
    unsigned int i = buttonCapabilities_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!ButtonCapabilitiesMarshaller::checkIntegrityConst(buttonCapabilities_[i]))
        {
            return false;
        }
    }
    delete buttonCapabilities;
    buttonCapabilities = 0;

    buttonCapabilities = new std::vector<ButtonCapabilities>(buttonCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_buttonCapabilities(void)
{
    if (buttonCapabilities)
    {
        delete buttonCapabilities;
    }
    buttonCapabilities = 0;
}

bool RegisterAppInterface_response::set_softButtonCapabilities(const std::vector<SoftButtonCapabilities>& softButtonCapabilities_)
{
    unsigned int i = softButtonCapabilities_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!SoftButtonCapabilitiesMarshaller::checkIntegrityConst(softButtonCapabilities_[i]))
        {
            return false;
        }
    }
    delete softButtonCapabilities;
    softButtonCapabilities = 0;

    softButtonCapabilities = new std::vector<SoftButtonCapabilities>(softButtonCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_softButtonCapabilities(void)
{
    if (softButtonCapabilities)
    {
        delete softButtonCapabilities;
    }
    softButtonCapabilities = 0;
}

bool RegisterAppInterface_response::set_presetBankCapabilities(const PresetBankCapabilities& presetBankCapabilities_)
{
    if (!PresetBankCapabilitiesMarshaller::checkIntegrityConst(presetBankCapabilities_))
    {
        return false;
    }
    delete presetBankCapabilities;
    presetBankCapabilities = 0;

    presetBankCapabilities = new PresetBankCapabilities(presetBankCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_presetBankCapabilities(void)
{
    if (presetBankCapabilities)
    {
        delete presetBankCapabilities;
    }
    presetBankCapabilities = 0;
}

bool RegisterAppInterface_response::set_hmiZoneCapabilities(const std::vector<HmiZoneCapabilities>& hmiZoneCapabilities_)
{
    unsigned int i = hmiZoneCapabilities_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!HmiZoneCapabilitiesMarshaller::checkIntegrityConst(hmiZoneCapabilities_[i]))
        {
            return false;
        }
    }
    delete hmiZoneCapabilities;
    hmiZoneCapabilities = 0;

    hmiZoneCapabilities = new std::vector<HmiZoneCapabilities>(hmiZoneCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_hmiZoneCapabilities(void)
{
    if (hmiZoneCapabilities)
    {
        delete hmiZoneCapabilities;
    }
    hmiZoneCapabilities = 0;
}

bool RegisterAppInterface_response::set_speechCapabilities(const std::vector<SpeechCapabilities>& speechCapabilities_)
{
    unsigned int i = speechCapabilities_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!SpeechCapabilitiesMarshaller::checkIntegrityConst(speechCapabilities_[i]))
        {
            return false;
        }
    }
    delete speechCapabilities;
    speechCapabilities = 0;

    speechCapabilities = new std::vector<SpeechCapabilities>(speechCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_speechCapabilities(void)
{
    if (speechCapabilities)
    {
        delete speechCapabilities;
    }
    speechCapabilities = 0;
}

bool RegisterAppInterface_response::set_vrCapabilities(const std::vector<VrCapabilities>& vrCapabilities_)
{
    unsigned int i = vrCapabilities_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!VrCapabilitiesMarshaller::checkIntegrityConst(vrCapabilities_[i]))
        {
            return false;
        }
    }
    delete vrCapabilities;
    vrCapabilities = 0;

    vrCapabilities = new std::vector<VrCapabilities>(vrCapabilities_);
    return true;
}

void RegisterAppInterface_response::reset_vrCapabilities(void)
{
    if (vrCapabilities)
    {
        delete vrCapabilities;
    }
    vrCapabilities = 0;
}

bool RegisterAppInterface_response::set_vehicleType(const VehicleType& vehicleType_)
{
    if (!VehicleTypeMarshaller::checkIntegrityConst(vehicleType_))
    {
        return false;
    }
    delete vehicleType;
    vehicleType = 0;

    vehicleType = new VehicleType(vehicleType_);
    return true;
}

void RegisterAppInterface_response::reset_vehicleType(void)
{
    if (vehicleType)
    {
        delete vehicleType;
    }
    vehicleType = 0;
}

const SyncMsgVersion* RegisterAppInterface_response::get_syncMsgVersion(void) const
{
    return syncMsgVersion;
}

const Language* RegisterAppInterface_response::get_language(void) const
{
    return language;
}

const Language* RegisterAppInterface_response::get_hmiDisplayLanguage(void) const
{
    return hmiDisplayLanguage;
}

const DisplayCapabilities* RegisterAppInterface_response::get_displayCapabilities(void) const
{
    return displayCapabilities;
}

const std::vector<ButtonCapabilities>* RegisterAppInterface_response::get_buttonCapabilities(void) const
{
    return buttonCapabilities;
}

const std::vector<SoftButtonCapabilities>* RegisterAppInterface_response::get_softButtonCapabilities(void) const
{
    return softButtonCapabilities;
}

const PresetBankCapabilities* RegisterAppInterface_response::get_presetBankCapabilities(void) const
{
    return presetBankCapabilities;
}

const std::vector<HmiZoneCapabilities>* RegisterAppInterface_response::get_hmiZoneCapabilities(void) const
{
    return hmiZoneCapabilities;
}

const std::vector<SpeechCapabilities>* RegisterAppInterface_response::get_speechCapabilities(void) const
{
    return speechCapabilities;
}

const std::vector<VrCapabilities>* RegisterAppInterface_response::get_vrCapabilities(void) const
{
    return vrCapabilities;
}

const VehicleType* RegisterAppInterface_response::get_vehicleType(void) const
{
    return vehicleType;
}

