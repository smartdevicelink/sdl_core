#include "Application.h"

namespace NsHMIEmulator
{
    /**
     * \brief Application constructor
     * \param name
     */
    Application::Application(const std::string &name)
        :mName(name)
    {
    }

    /**
     * \brief get app name
     * \return app name
     */
    const std::string &Application::getName() const
    {
        return mName;
    }

    /**
     * \brief set app icon
     * \param icon
     */
    void Application::setIcon(const std::string &icon)
    {
        mIcon = icon;
    }

    /**
     * \brief get app icon
     * \return app icon
     */
    const std::string &Application::getIcon() const
    {
        return mIcon;
    }

    /**
     * \brief set device name
     * \param name device name
     */
    void Application::setDeviceName(const std::string &name)
    {
        mDeviceName = name;
    }

    /**
     * \brief get device name
     * \return device name
     */
    const std::string &Application::getDeviceName() const
    {
        return mDeviceName;
    }

    /**
     * \brief set if app is a media app
     * \param isMedia is app a media app
     */
    void Application::setIsMedia(bool isMedia)
    {
        m_bIsMedia = isMedia;
    }

    /**
     * \brief get if app is a media app
     * \return is app a media app
     */
    bool Application::getIsMedia() const
    {
        return m_bIsMedia;
    }

    /**
     * \brief set app desired language
     * \param language
     */
    void Application::setLanguageDesired(const NsSmartDeviceLinkRPC::Language &language)
    {
        mLanguageDesired = language;
    }

    /**
     * \brief get app desired languages
     * \return app desired languages
     */
    const NsSmartDeviceLinkRPC::Language &Application::getLanguageDesired() const
    {
        return mLanguageDesired;
    }

    /**
     * \brief set app VR synonyms
     * \param synonyms
     */
    void Application::setVrSynonyms(const Synonyms &synonyms)
    {
        mVrSynonyms = synonyms;
    }

    /**
     * \brief get app VR synonyms
     * \return app VR synonyms
     */
    const Synonyms &Application::getVrSynonyms() const
    {
        return mVrSynonyms;
    }

    /**
     * \brief Application copy constructor
     */
    Application::Application(const Application &)
    {
    }

}
