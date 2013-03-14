#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include "JSONHandler/SDLRPCObjects/Language.h"

namespace NsHMIEmulator
{
    typedef std::vector<std::string> Synonyms;

    class Application
    {
    public:

        /**
         * \brief Application constructor
         * \param name
         */
        Application(const std::string& name);

        /**
         * \brief get app name
         * \return app name
         */
        const std::string& getName() const;

        /**
         * \brief set app icon
         * \param icon
         */
        void setIcon(const std::string& icon);

        /**
         * \brief get app icon
         * \return app icon
         */
        const std::string& getIcon() const;

        /**
         * \brief set device name
         * \param name device name
         */
        void setDeviceName(const std::string& name);

        /**
         * \brief get device name
         * \return device name
         */
        const std::string& getDeviceName() const;

        /**
         * \brief set if app is a media app
         * \param isMedia is app a media app
         */
        void setIsMedia(bool isMedia);

        /**
         * \brief get if app is a media app
         * \return is app a media app
         */
        bool getIsMedia() const;

        /**
         * \brief set app desired language
         * \param language
         */
        void setLanguageDesired(const NsSmartDeviceLinkRPC::Language& language);

        /**
         * \brief get app desired languages
         * \return app desired languages
         */
        const NsSmartDeviceLinkRPC::Language& getLanguageDesired() const;

        /**
         * \brief set app VR synonyms
         * \param synonyms
         */
        void setVrSynonyms(const Synonyms& synonyms);

        /**
         * \brief get app VR synonyms
         * \return app VR synonyms
         */
        const Synonyms& getVrSynonyms() const;

    private:

        /**
         * \brief Application copy constructor
         */
        Application(const Application&);

        const std::string mName;
        std::string mIcon;
        std::string mDeviceName;
        bool m_bIsMedia;
        NsSmartDeviceLinkRPC::Language mLanguageDesired;
        Synonyms mVrSynonyms;
    };
}

#endif // APPLICATION_H
